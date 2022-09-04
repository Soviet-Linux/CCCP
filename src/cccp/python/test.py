from ctypes import cdll

VERSION = 0.01

libspm = cdll.LoadLibrary('/usr/lib/libspm.so')
print(libspm)
lib_version  = libspm.version();

print(f"[+] Libspm C Version => {lib_version}")
print(f"[+] CCCP Python Front-end Version => {VERSION}")