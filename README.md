OS X FSEvents
=============

Detects when a file changed. Tested in OS X 10.9 (Mavericks) and OS X 10.10 (Yosemite).

Compile the C version with:
```
gcc -framework CoreServices main.c -o main_c
```

Compile the C++ version with:
```
gcc -framework CoreServices -lstdc++ main.cpp -o main_cpp
```

Execute the C version with:
```
./main_c /path/to/listen
```

Execute the C++ version with:
```
./main_cpp /path/to/listen
```

### Differences between C & C++ versions
There is no difference, both work in the same way.
