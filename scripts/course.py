#!/usr/bin/env python3
"""Dependency-free build/test driver. Reports learner progress, never reference progress."""
import argparse
from datetime import datetime, timezone
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys
import time
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from cases import Checks, execute
COURSE=json.loads((ROOT/'course.json').read_text())
HASHES=json.loads((ROOT/'tests/starter-hashes.json').read_text())

def project(value):
    for p in COURSE['projects']:
        if value in (p['id'],p['id'].split('-',1)[1],str(p['n']),f"{p['n']:02d}"):return p
    raise ValueError('unknown project '+value)

def sources(p,mode):
    directory=ROOT/('reference' if mode=='reference' else 'projects')/p['id']
    return sorted(directory.rglob('*.c'))

def started(p):
    folder=ROOT/'projects'/p['id']
    code=sorted(list(folder.rglob('*.c'))+list(folder.rglob('*.h')))
    return len(code)!=1 or not code or hashlib.sha256(code[0].read_bytes()).hexdigest()!=HASHES[p['id']]

def build(p,mode,sanitize,cc):
    out=ROOT/'build'/mode/('sanitized' if sanitize else 'normal')/p['id'];out.mkdir(parents=True,exist_ok=True)
    ffi=p['n']==19
    binary=out/('libcourse.so' if ffi and not sanitize else 'main')
    flags=['-std=c17','-D_POSIX_C_SOURCE=200809L','-Wall','-Wextra','-Wpedantic','-Werror','-g','-O1' if sanitize else '-O2','-I',str(ROOT/'include')]
    if mode=='reference':flags+=['-I',str(ROOT/'reference/modules')]
    if sanitize:flags+=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']
    if ffi and not sanitize:flags+=['-fPIC','-shared']
    inputs=sources(p,mode)
    if ffi and sanitize:inputs+=[ROOT/'tests/ffi_harness.c']
    cmd=shlex.split(cc)+flags+list(map(str,inputs))+['-o',str(binary),'-pthread','-lm']
    proc=subprocess.run(cmd,capture_output=True,text=True,timeout=40)
    return binary,proc.returncode, '$ '+shlex.join(cmd)+'\n'+proc.stdout+proc.stderr

def test_one(p,mode,sanitize,cc):
    start=time.monotonic();log='';count=0;state='failed'
    try:
        binary,rc,log=build(p,mode,sanitize,cc)
        if rc:raise RuntimeError('compilation failed')
        checks=Checks(binary,sanitize)
        try:count=execute(p['id'],checks)
        finally:log+='\n'+'\n'.join(checks.lines)+'\n';count=checks.count
        state='passed'
    except (AssertionError,RuntimeError,ValueError,OSError,subprocess.TimeoutExpired) as e:
        log+=f'\nFAIL {e}\n'
    result={'id':p['id'],'status':state,'checks':count,'seconds':round(time.monotonic()-start,3),'sanitized':sanitize}
    logs=ROOT/'build/logs'/mode;logs.mkdir(parents=True,exist_ok=True);(logs/(p['id']+('.sanitized' if sanitize else '')+'.log')).write_text(log)
    print(f"{state.upper():6} {p['id']:20} {count:3} checks  {result['seconds']:6.2f}s")
    if state=='failed':print(log[-2500:])
    return result

def main():
    ap=argparse.ArgumentParser();ap.add_argument('command',choices=['list','build','test','report']);ap.add_argument('project',nargs='?',default='all');ap.add_argument('--mode',choices=['learner','reference'],default='learner');ap.add_argument('--sanitize',action='store_true');ap.add_argument('--cc',default=os.environ.get('CC','gcc'));ap.add_argument('--output');args=ap.parse_args()
    selected=COURSE['projects'] if args.project=='all' else [project(args.project)]
    if args.command=='list':
        for p in selected:print(f"{p['id']:20} {p['hours']:2}h  {p['title']}")
        return 0
    if args.command=='build':
        if args.project=='all':ap.error('build requires one project; use test all to build and test the course')
        binary,rc,log=build(selected[0],args.mode,args.sanitize,args.cc);print(log);print(binary);return rc
    if args.command=='report' and args.mode!='learner':ap.error('progress reports are learner-only')
    results=[]
    for p in selected:
        if args.command=='report' and not started(p):
            results.append({'id':p['id'],'status':'not_started','checks':0,'seconds':0,'sanitized':args.sanitize});continue
        results.append(test_one(p,args.mode,args.sanitize,args.cc))
    report={'schema':1,'mode':args.mode,'compiler':args.cc,'sanitized':args.sanitize,'generated_at':datetime.now(timezone.utc).isoformat(),'commit':os.environ.get('GITHUB_SHA','local'),'results':results}
    target=Path(args.output) if args.output else ROOT/'build'/f"{args.mode}-{'sanitized' if args.sanitize else 'normal'}.json"
    target.parent.mkdir(parents=True,exist_ok=True);target.write_text(json.dumps(report,indent=2)+'\n')
    return int(any(r['status']=='failed' for r in results))
if __name__=='__main__':sys.exit(main())
