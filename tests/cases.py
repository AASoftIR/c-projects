"""Black-box contract tests. Extend these when you extend a project's contract."""
import ctypes
import json
import os
from pathlib import Path
import random
import selectors
import socket
import subprocess
import sys
import tempfile

class Checks:
    def __init__(self, binary, sanitize=False):
        self.binary = str(binary)
        self.count = 0
        self.sanitize = sanitize
        self.lines = []
    def check(self, condition, name, detail=''):
        self.count += 1
        if not condition:
            raise AssertionError(f'{name}: {detail}')
        self.lines.append(f'PASS {name}')
    def run(self, args=(), data=b'', code=0, output=None, name='case', timeout=8):
        p = subprocess.run([self.binary, *map(str,args)],input=data,capture_output=True,timeout=timeout)
        self.check(p.returncode == code, name+' exit', f'got {p.returncode}, expected {code}; stderr={p.stderr.decode(errors="replace")}')
        if output is not None:
            if isinstance(output,str): output=output.encode()
            self.check(p.stdout == output,name+' stdout',f'got {p.stdout!r}, expected {output!r}')
        self.check(b'runtime error:' not in p.stderr and b'ERROR: AddressSanitizer' not in p.stderr,name+' sanitizer',p.stderr.decode(errors='replace'))
        return p

def execute(pid,c):
    n=int(pid[:2])
    rng=random.Random(51017)
    if n==1:
        for x in [0,1,255,256,2147483648,4294967295]+[rng.randrange(2**32) for _ in range(20)]:
            c.run([x],output=f'dec={x} hex={x:08X} bits={x:032b} ones={x.bit_count()}\n',name=f'uint32 {x}')
        for x in ['-1','+1',' 1','4294967296','12x','']:
            c.run([x],code=2,name=f'reject {x!r}')
    elif n==2:
        for data in [b'',b'\x00\xff\x7f',bytes(range(256)),rng.randbytes(77)]:
            want=''.join(f'{i:08x}:' + ''.join(f' {b:02x}' for b in data[i:i+16])+'\n' for i in range(0,len(data),16))
            c.run(data=data,output=want,name=f'binary {len(data)}')
    elif n==3:
        for text in ['', 'hello', 'سلام علی‌رضا', '📚 C\x00!', '\U0010ffff']:
            raw=text.encode();c.run(data=raw,output=f'bytes={len(raw)} codepoints={len(text)}\n',name=f'UTF-8 {text!r}')
        for raw in [b'\x80',b'\xc0\xaf',b'\xed\xa0\x80',b'\xf4\x90\x80\x80',b'\xe2\x82',b'\xc2x',b'\xf5\x80\x80\x80']:
            c.run(data=raw,code=2,name=f'invalid {raw.hex()}')
    elif n==4:
        for values in [[1],[-2,0,4,-2],[2**63-1,-2**63,0],[rng.randrange(-10000,10000) for _ in range(200)]]:
            c.run(values,output=' '.join(map(str,sorted(values)))+'\n',name=f'sort {len(values)}')
        for args in [[],['1x'],['9223372036854775808']]:c.run(args,code=2,name='invalid vector')
    elif n==5:
        c.run(data=b'Cat cat DOG! cat\nDog. 123',output='cat=3\ndog=2\n',name='counts and case')
        c.run(data=b'',output=b'',name='empty')
        c.run(data='سلام! abc ABC'.encode(),output='abc=2\n',name='ASCII semantics')
        words=[''.join(rng.choices('abcde',k=rng.randrange(1,12))) for _ in range(500)]
        from collections import Counter
        d=Counter(words);c.run(data=' '.join(words).encode(),output=''.join(f'{k}={d[k]}\n' for k in sorted(d)),name='random map')
        c.run(data=b'a'*4096,output='a'*4096+'=1\n',name='long token')
    elif n==6:
        valid=[(b'',0,0,0),(b'\n',1,1,1),(b'a,b\n1,2\n',2,4,2),(b'"a,b","a""b"\n"two\nlines",x',2,4,2),(b',,\n',1,3,3),(b'""',1,1,1),(b'a,b,',1,3,3)]
        for raw,rows,fields,maximum in valid:c.run(data=raw,output=f'rows={rows} fields={fields} max_fields={maximum}\n',name=f'CSV {raw!r}')
        for raw in [b'"oops',b'ab"c',b'"x"y',b'a\r\n'] :c.run(data=raw,code=2,name='malformed CSV')
    elif n==7:
        c.run(['cat'],data=b'cat\ndog\nbobcat',output='1:cat\n3:bobcat\n',name='literal and no final LF')
        c.run(['cat','-i'],data=b'CAT\ncAt\n',output='1:CAT\n2:cAt\n',name='ignore case')
        c.run(['x'],data=b'cat\n',code=1,output=b'',name='no match')
        c.run([''],data=b'\na\n',output='1:\n2:a\n',name='empty pattern')
        c.run(['x','--bad'],code=2,name='bad option')
    elif n==8:
        c.run([1,1,'reset',1],name='alignment reset',output=f'0\n{ctypes.alignment(ctypes.c_longdouble)}\nreset\n0\n')
        c.run([4096],output='0\n',name='exact fit')
        for args in [[4096,1],[0],[-1],[4097],['x']]:c.run(args,code=2,name='invalid or exhausted')
    elif n==9:
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'reading.log'
            for entry in ['Nader chapter','سلام کتاب']:c.run([p,'add',entry],output='added\n',name='append entry')
            c.run([p,'list'],output='1:Nader chapter\n2:سلام کتاب\n',name='reopen persistence')
            c.run([p,'add','bad\nentry'],code=2,name='record injection')
            c.run([p,'add','x'*4097],code=2,name='entry bound')
            c.run([Path(d)/'missing','list'],code=2,name='missing file')
        if Path('/dev/full').exists():c.run(['/dev/full','add','x'],code=2,name='write failure')
    elif n==10:
        for expr,want in [('2+3*4','14'),('-(2+3)/2','-2.5'),('1e2 + .5','100.5'),(' --3 ','3'),('1+2+3+4','10')]:c.run([expr],output=want+'\n',name=expr)
        for expr in ['','2x','(1+2','1/0','1e999','nan','2**3','('*200+'1'+')'*200]:c.run([expr],code=2,name='invalid calc')
    elif n==11:
        c.run(['-c','printf hello'],output='hello',name='exec')
        c.run(['-c','printf hello | wc -c'],output='5\n',name='pipeline')
        c.run(['-c','seq 1 100000 | wc -l'],output='100000\n',name='pipe capacity',timeout=12)
        c.run(['-c','true | false'],code=1,name='last status')
        c.run(['-c','false | true'],code=0,name='all children reaped')
        c.run(['-c','definitely_no_such_course_command'],code=127,name='missing command')
        for cmd in ['| true','true |','true | | false']:c.run(['-c',cmd],code=2,name='bad pipeline')
        c.run(data=b'printf yes\nexit\nprintf no\n',output='yes',name='interactive exit')
    elif n==12:
        c.run(data=b'GET /health HTTP/1.1\r\nHost: localhost\r\n\r\n',output='method=GET target=/health version=HTTP/1.1 headers=1\n',name='valid GET')
        c.run(data=b'GET /search?q=cat HTTP/1.1\r\nHost: x\r\nContent-Length: 0\r\nX-A: hi\r\n\r\n',output='method=GET target=/search?q=cat version=HTTP/1.1 headers=3\n',name='framed empty request')
        for raw in [b'GET / HTTP/1.1\nHost: x\n\n',b'POST / HTTP/1.1\r\nHost: x\r\n\r\n',b'GET / HTTP/1.1\r\n\r\n',b'GET / HTTP/1.1\r\nHost: x\r\nHost: y\r\n\r\n',b'GET / HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n',b'GET / HTTP/1.1\r\nHost: x\r\nContent-Length: 1\r\n\r\nx',b'GET / HTTP/1.1\r\nHost: x\r\nBad Name: x\r\n\r\n',b'GET / HTTP/1.1\r\nHost: x\r\nX: \x00\r\n\r\n',b'x'*9000]:c.run(data=raw,code=2,name='reject ambiguous request')
    elif n in (13,20):
        server_tests(n,c)
    elif n==14:
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'data.kv'
            c.run([p,'get','missing'],code=1,name='missing database')
            c.run([p,'put','name','Alireza'],output='ok\n',name='put')
            c.run([p,'put','name','C learner'],output='ok\n',name='overwrite')
            c.run([p,'get','name'],output='C learner\n',name='replay newest')
            c.run([p,'del','name'],output='ok\n',name='delete')
            c.run([p,'get','name'],code=1,name='deleted')
            with p.open('ab') as f:f.write(b'P\tpartial')
            c.run([p,'get','partial'],code=1,name='ignore torn tail')
            c.run([p,'put','safe','yes'],output='ok\n',name='repair tail before append')
            c.run([p,'get','safe'],output='yes\n',name='read repaired log')
            c.run([p,'put','x','bad\nvalue'],code=2,name='prevent record injection')
            c.run([p,'put','x'*256,'v'],code=2,name='key bound')
            p.write_bytes(b'corrupt\n');c.run([p,'get','safe'],code=2,name='reject complete corrupt record')
        if Path('/dev/full').exists():c.run(['/dev/full','put','x','y'],code=2,name='write failure',timeout=3)
    elif n==15:
        c.run([2,'a','b','a','c','b'],output='hit=1 miss=4 cache=b,c\n',name='promotion and eviction')
        c.run([1,'a','a','b','a'],output='hit=1 miss=3 cache=a\n',name='capacity one')
        c.run([3],output='hit=0 miss=0 cache=\n',name='empty')
        c.run([0],code=2,name='zero capacity')
        c.run([10001],code=2,name='capacity limit')
    elif n==16:
        for threads,count in [(1,0),(1,1),(8,3),(7,100000),(64,1000000)]:c.run([threads,count],output=f'sum={count*(count+1)//2}\n',name=f'partition {threads}/{count}')
        for args in [(0,1),(65,1),(2,-1),(2,10000001)]:c.run(args,code=2,name='invalid worker bounds')
    elif n==17:
        c.run(data=b'PUSH 2\nPUSH 3\nADD\nPUSH 4\nMUL\nPRINT\n',output='20\n',name='arithmetic VM')
        c.run(data=b'PUSH 7\nPUSH 2\nSUB\nPRINT\nPUSH 2\nDIV\nPRINT\n',output='5\n2\n',name='operand order')
        bad=[b'ADD\n',b'PRINT\n',b'PUSH 1\nPUSH 0\nDIV\n',b'PUSH 9223372036854775807\nPUSH 1\nADD\n',b'PUSH -9223372036854775808\nPUSH -1\nDIV\n',b'PUSH -9223372036854775808\nPUSH -1\nMUL\n',b'PUSH -9223372036854775808\nPUSH 1\nSUB\n',b'PUSH 1 junk\n',b'NOP\n',b'PUSH 0\n'*1025]
        for data in bad:c.run(data=data,code=2,name='VM safety')
        for _ in range(20):
            a,b=rng.randrange(-100000,100000),rng.randrange(-100000,100000)
            c.run(data=f'PUSH {a}\nPUSH {b}\nMUL\nPRINT\n'.encode(),output=f'{a*b}\n',name='random signed multiplication')
    elif n==18:
        with tempfile.TemporaryDirectory() as d:
            p=Path(d)/'corpus';p.write_text('cat cat dog\nDOG bird\n\nCats cat\nسلام\n')
            for word,want,code in [('cat','1\n4\n',0),('DOG','1\n2\n',0),('absent','',1)]:c.run([p,word],output=want,code=code,name=f'index {word}')
            for word in ['two words','سلام','cat!','']:c.run([p,word],code=2,name='query contract')
            c.run([Path(d)/'missing','cat'],code=2,name='missing corpus')
            lines=[' '.join(rng.choices(['cat','dog','bird','c'],k=10)) for _ in range(100)]
            p.write_text('\n'.join(lines));want=''.join(f'{i}\n' for i,l in enumerate(lines,1) if 'cat' in l.split())
            c.run([p,'cat'],output=want,name='random postings oracle')
    elif n==19:
        if c.sanitize:
            c.run(output='ffi harness passed\n',name='sanitized ABI harness')
        else:
            p=subprocess.run([sys.executable,str(Path(__file__).with_name('ffi_check.py')),c.binary],capture_output=True,timeout=8)
            c.check(p.returncode==0,'ctypes ABI',p.stderr.decode(errors='replace'))
            c.check(p.stdout==b'ffi ABI passed\n','ctypes values')
    return c.count

def server_tests(n,c):
    with tempfile.TemporaryDirectory() as d:
        corpus=Path(d)/'docs';corpus.write_text('cat dog cat\nDOG\ncat bird\n')
        args=[c.binary,'0']+([str(corpus)] if n==20 else [])
        p=subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        try:
            sel=selectors.DefaultSelector();sel.register(p.stdout,selectors.EVENT_READ)
            ready=sel.select(5);sel.close()
            c.check(bool(ready),'server startup','did not announce port within 5s')
            line=p.stdout.readline().decode();c.check(line.startswith('READY='),'port announcement',line)
            port=int(line.split('=')[1]);c.check(0<port<65536,'bound ephemeral port')
            def request(raw, split=False):
                with socket.create_connection(('127.0.0.1',port),timeout=5) as s:
                    if split:
                        for part in [raw[:9],raw[9:20],raw[20:]]:s.sendall(part)
                    else:s.sendall(raw)
                    s.shutdown(socket.SHUT_WR);out=b''
                    while True:
                        block=s.recv(65536)
                        if not block:break
                        out+=block
                head,body=out.split(b'\r\n\r\n',1)
                length=int(next(l.split(b':',1)[1] for l in head.split(b'\r\n') if l.lower().startswith(b'content-length:')))
                c.check(len(body)==length,'response framing')
                return int(head.split()[1]),json.loads(body)
            def get(target):return request(f'GET {target} HTTP/1.1\r\nHost: localhost\r\n\r\n'.encode(),True)
            code,body=get('/health');c.check(code==200 and body=={'status':'ok'},'health JSON')
            code,_=get('/missing');c.check(code==404,'404 route')
            for raw in [b'GET / HTTP/1.1\r\n\r\n',b'POST /health HTTP/1.1\r\nHost: x\r\n\r\n',b'GET / HTTP/1.1\r\nHost: x\r\nContent-Length: 5\r\n\r\n']:
                code,_=request(raw);c.check(code==400,'reject malformed HTTP')
            for _ in range(8):c.check(get('/health')[0]==200,'repeated connections')
            if n==20:
                for target,want in [('/search?q=cat',[1,3]),('/search?q=DOG',[1,2]),('/search?q=%63at',[1,3]),('/search?q=unknown',[])]:
                    code,body=get(target);c.check(code==200 and body=={'documents':want},'query '+target,str(body))
                for target in ['/search','/search?q=','/search?q=cat+dog','/search?q=%00','/search?q=%xx','/search?q=%','/search?q=cat&x=1']:
                    c.check(get(target)[0]==400,'query rejection '+target)
        finally:
            p.terminate()
            try:_,err=p.communicate(timeout=3)
            except subprocess.TimeoutExpired:p.kill();_,err=p.communicate()
            c.check(b'runtime error:' not in err and b'ERROR: AddressSanitizer' not in err,'server sanitizer',err.decode(errors='replace'))
    if n==20:c.run(['0','/definitely/no/such/corpus'],code=2,name='startup missing corpus')
