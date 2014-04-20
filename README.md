OS X FSEvents
=============

Detects when a file changed. Only tested in OS X 10.9 (Mavericks).

Compile it with:
```
gcc -framework CoreServices main.c -o main
```

Execute it with:
```
./main /path/to/listen
```
