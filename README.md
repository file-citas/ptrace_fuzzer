Ptrace Fuzzer
=============


Build
-----

```
autoreconf -i
./configure
make
```


Dependencies
------------

* libelf
* libdistorm3: https://code.google.com/p/distorm/source/checkout
* easyloggingpp: https://github.com/easylogging/easyloggingpp

Usage
-----

```
./ptrfuzz a.out a b c 
```
