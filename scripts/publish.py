#!/usr/bin/env python3
"""Merge learner evidence; update managed README and static dashboard data."""
import argparse
from datetime import datetime,timezone
import json
import os
from pathlib import Path
import re
import sys
ROOT=Path(__file__).resolve().parents[1]

def merge(course,paths,require_matrix=False):
    ids=[p['id'] for p in course['projects']]
    if not paths:
        return json.loads((ROOT/'progress.json').read_text()) if (ROOT/'progress.json').exists() else {'schema':1,'commit':'not-run','generated_at':None,'lanes':[],'results':[{'id':pid,'status':'not_started','checks':0,'seconds':0} for pid in ids]}
    reports=[json.loads(Path(path).read_text()) for path in paths]
    lanes=[];commits=set()
    for r in reports:
        if r.get('schema')!=1 or r.get('mode')!='learner':raise ValueError('only learner reports can set progress')
        if type(r.get('sanitized')) is not bool or r.get('compiler') not in ('gcc','clang'):raise ValueError('invalid test lane')
        lane=(r['compiler'],r['sanitized'])
        if lane in lanes:raise ValueError('duplicate test lane')
        lanes.append(lane);commits.add(r.get('commit'))
        results=r.get('results',[])
        if [x.get('id') for x in results]!=ids:raise ValueError('report must contain the exact complete project sequence')
        for x in results:
            if x.get('status') not in ('not_started','passed','failed'):raise ValueError('invalid project status')
            if type(x.get('checks')) is not int or not 0<=x['checks']<=1000000:raise ValueError('invalid check count')
            if not isinstance(x.get('seconds'),(int,float)) or not 0<=x['seconds']<=100000:raise ValueError('invalid elapsed time')
    if len(commits)!=1 or not all(isinstance(x,str) and x for x in commits):raise ValueError('mixed or missing commit identity')
    if require_matrix and set(lanes)!={('gcc',False),('gcc',True),('clang',False),('clang',True)}:raise ValueError('full GCC/Clang normal/sanitizer matrix required')
    results=[]
    for i,pid in enumerate(ids):
        rows=[r['results'][i] for r in reports];states=[x['status'] for x in rows]
        status='passed' if all(x=='passed' for x in states) else 'not_started' if all(x=='not_started' for x in states) else 'failed'
        results.append({'id':pid,'status':status,'checks':sum(x['checks'] for x in rows),'seconds':round(sum(x['seconds'] for x in rows),3)})
    return {'schema':1,'commit':next(iter(commits)),'generated_at':datetime.now(timezone.utc).isoformat(),'lanes':[{'compiler':cc,'sanitized':san} for cc,san in lanes],'results':results}

def publish(course,progress):
    owner_repo=os.environ.get('GITHUB_REPOSITORY',course['owner']+'/'+course['repo'])
    if not re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+',owner_repo):raise ValueError('invalid repository slug')
    owner,repo=owner_repo.split('/')
    course['owner'],course['repo']=owner,repo
    lookup={r['id']:r for r in progress['results']};passed=sum(r['status']=='passed' for r in progress['results'])
    lines=[f'**Verified implementations: {passed}/{len(course["projects"])}** · Evidence lanes: {len(progress["lanes"])} · Commit: `{progress["commit"]}`','']
    for p in course['projects']:
        r=lookup[p['id']];lines.append(f'- [{"x" if r["status"]=="passed" else " "}] [{p["n"]:02d} · {p["title"]}](projects/{p["id"]}/README.md) — {r["status"].replace("_"," ")}')
    readme=ROOT/'README.md';text=readme.read_text()
    a='<!-- PROGRESS:START -->';b='<!-- PROGRESS:END -->'
    if text.count(a)!=1 or text.count(b)!=1:raise ValueError('README managed markers missing or ambiguous')
    start=text.index(a)+len(a);end=text.index(b)
    readme.write_text(text[:start]+'\n'+'\n'.join(lines)+'\n'+text[end:])
    (ROOT/'progress.json').write_text(json.dumps(progress,indent=2)+'\n')
    # All Markdown/code are rendered as text in the browser, never trusted HTML.
    for p in course['projects']:
        p.update(lookup[p['id']])
        p['guide']=(ROOT/f'projects/{p["id"]}/README.md').read_text()
        p['notes_template']=(ROOT/f'projects/{p["id"]}/NOTES.md').read_text()
        p['learner_files']={str(f.relative_to(ROOT)):f.read_text() for f in sorted((ROOT/f'projects/{p["id"]}').rglob('*')) if f.suffix in ('.c','.h')}
        p['reference_files']={str(f.relative_to(ROOT)):f.read_text() for f in sorted((ROOT/f'reference/{p["id"]}').rglob('*')) if f.suffix in ('.c','.h')}
        p['lab']=(ROOT/f'projects/{p["id"]}/LAB.md').read_text() if (ROOT/f'projects/{p["id"]}/LAB.md').exists() else ''
    data={'course':course,'progress':progress,'docs':{str(p.relative_to(ROOT)):p.read_text() for p in sorted((ROOT/'docs').glob('*.md'))},'start':(ROOT/'START-HERE.md').read_text(),'tests':(ROOT/'tests/cases.py').read_text(),'modules':{str(p.relative_to(ROOT)):p.read_text() for p in sorted((ROOT/'reference/modules').glob('*.h'))}}
    (ROOT/'site/data').mkdir(exist_ok=True);(ROOT/'site/data/course.json').write_text(json.dumps(data,ensure_ascii=False)+'\n');(ROOT/'site/.nojekyll').touch()
    # A downloadable snapshot also works without a local HTTP server.
    html=(ROOT/'site/index.html').read_text()
    css=(ROOT/'site/style.css').read_text()
    js=(ROOT/'site/app.js').read_text()
    embedded=json.dumps(data,ensure_ascii=False).replace('<',r'\u003c')
    html=html.replace('<link rel="stylesheet" href="style.css">','<style>'+css+'</style>')
    html=html.replace('<script src="app.js" defer></script>','')
    html=html.replace('<link rel="icon" type="image/svg+xml" href="favicon.svg">','')
    scripts='<script>window.COURSE_DATA='+embedded+';</script><script>'+js.replace('</script',r'<\/script')+'</script>'
    html=html.replace('</body>',scripts+'</body>')
    (ROOT/'course-preview.html').write_text(html)
    print(f'Published {len(course["projects"])} project views; learner progress {passed}/{len(course["projects"])}; evidence lanes {len(progress["lanes"])}')

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--reports',nargs='*',default=[]);ap.add_argument('--require-matrix',action='store_true');a=ap.parse_args()
    c=json.loads((ROOT/'course.json').read_text());p=merge(c,a.reports,a.require_matrix);publish(c,p)
if __name__=='__main__':
    try:main()
    except (ValueError,OSError,KeyError,json.JSONDecodeError) as e:print('publish error:',e,file=sys.stderr);sys.exit(1)
