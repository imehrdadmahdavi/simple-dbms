# Simple Database Management System

This is a small program written in C to manage a database. It is intended to show C's low level programming capabilities such as direct memory access and memory allocation. As a result of simplicity, this program is not very efficient and doesn’t store very much.

## Demo
```shell
$ ./dbms db.dat c
$ ./dbms db.dat s 0 mehrdad name@domain.com
$ ./dbms db.dat s 1 pedram another.name@domain.com
$ ./dbms db.dat s 2 ashkan more.name@domain.com
$ ./dbms db.dat g 1
1 pedram another.name@domain.com
$ ./dbms db.dat l
0 mehrdad name@domain.com
1 pedram another.name@domain.com
2 ashkan more.name@domain.com
$ ./dbms db.dat d 1
$ ./dbms db.dat l
0 mehrdad name@domain.com
2 ashkan more.name@domain.com
```
