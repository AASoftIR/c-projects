"""Regression checks for honest progress and starter isolation."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('publish',ROOT/'scripts/publish.py');pub=importlib.util.module_from_spec(spec);spec.loader.exec_module(pub)
COURSE=json.loads((ROOT/'course.json').read_text())
class ProgressTests(unittest.TestCase):
    def reports(self,d,first='passed'):
        paths=[]
        for cc in ['gcc','clang']:
            for san in [False,True]:
                r={'schema':1,'mode':'learner','compiler':cc,'sanitized':san,'commit':'abc123','results':[{'id':p['id'],'status':first if p['n']==1 else 'not_started','checks':12 if p['n']==1 else 0,'seconds':.1} for p in COURSE['projects']]}
                path=Path(d)/f'{cc}-{san}.json';path.write_text(json.dumps(r));paths.append(path)
        return paths
    def test_all_lanes_required_for_pass(self):
        with tempfile.TemporaryDirectory() as d:
            paths=self.reports(d);r=pub.merge(COURSE,paths,True)
            self.assertEqual(r['results'][0]['status'],'passed');self.assertEqual(r['results'][0]['checks'],48)
            self.assertEqual(r['results'][1]['status'],'not_started')
            with self.assertRaises(ValueError):pub.merge(COURSE,paths[:3],True)
    def test_failure_regresses_pass(self):
        with tempfile.TemporaryDirectory() as d:
            paths=self.reports(d);data=json.loads(paths[2].read_text());data['results'][0]['status']='failed';paths[2].write_text(json.dumps(data))
            self.assertEqual(pub.merge(COURSE,paths,True)['results'][0]['status'],'failed')
    def test_reference_never_counts(self):
        with tempfile.TemporaryDirectory() as d:
            paths=self.reports(d);data=json.loads(paths[0].read_text());data['mode']='reference';paths[0].write_text(json.dumps(data))
            with self.assertRaises(ValueError):pub.merge(COURSE,paths)
    def test_mixed_commits_duplicate_lanes_and_bad_ids(self):
        with tempfile.TemporaryDirectory() as d:
            paths=self.reports(d)
            with self.assertRaises(ValueError):pub.merge(COURSE,paths+[paths[0]])
            data=json.loads(paths[0].read_text());data['commit']='different';paths[0].write_text(json.dumps(data))
            with self.assertRaises(ValueError):pub.merge(COURSE,paths)
            paths=self.reports(d);data=json.loads(paths[0].read_text());data['results'].pop();paths[0].write_text(json.dumps(data))
            with self.assertRaises(ValueError):pub.merge(COURSE,paths)
    def test_real_starter_pass_and_regression_in_copy(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d)/'repo';shutil.copytree(ROOT,root,ignore=shutil.ignore_patterns('build','__pycache__'))
            def report():
                proc=subprocess.run([sys.executable,'scripts/course.py','report','--output','report.json'],cwd=root,capture_output=True,text=True,timeout=45)
                return proc,json.loads((root/'report.json').read_text())
            proc,r=report();self.assertEqual(proc.returncode,0);self.assertTrue(all(x['status']=='not_started' for x in r['results']))
            (root/'projects/01-byte-lab/NOTES.md').write_text('Notes do not count as code.')
            proc,r=report();self.assertEqual(r['results'][0]['status'],'not_started')
            learner=root/'projects/01-byte-lab/main.c';learner.write_text((root/'reference/01-byte-lab/main.c').read_text())
            proc,r=report();self.assertEqual(proc.returncode,0,proc.stdout);self.assertEqual(r['results'][0]['status'],'passed')
            learner.write_text('int main(void) { return 0; }\n')
            proc,r=report();self.assertEqual(proc.returncode,1);self.assertEqual(r['results'][0]['status'],'failed')
if __name__=='__main__':unittest.main()
