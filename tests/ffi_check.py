import ctypes as C
import sys
lib=C.CDLL(sys.argv[1]);fn=lib.course_utf8_count
fn.argtypes=[C.POINTER(C.c_ubyte),C.c_size_t,C.POINTER(C.c_size_t)];fn.restype=C.c_int
for text in ['', 'سلام', 'A\x00B', '📚']:
    raw=text.encode();data=(C.c_ubyte*max(1,len(raw))).from_buffer_copy(raw or b'\0');out=C.c_size_t(999)
    assert fn(data,len(raw),C.byref(out))==0 and out.value==len(text)
for raw in [b'\x80',b'\xc0\xaf',b'\xed\xa0\x80',b'\xf4\x90\x80\x80',b'\xe2\x82']:
    data=(C.c_ubyte*len(raw)).from_buffer_copy(raw);out=C.c_size_t()
    assert fn(data,len(raw),C.byref(out))!=0
out=C.c_size_t()
assert fn(None,0,C.byref(out))==0 and out.value==0
assert fn(None,1,C.byref(out))!=0
assert fn(None,0,None)!=0
print('ffi ABI passed')
