# Simple Database Management System

This is a small program written in C to manage a database. It is intended to show C's low level programming capabilities such as direct memory access and memory allocation. As a result of simplicity, this program is not very efficient and doesn’t store very much.

## Demo
```shell
# create a database
$ ./dbms db.dat c

# add records into the database (set)
$ ./dbms db.dat s 0 mehrdad name@domain.com
$ ./dbms db.dat s 1 pedram another.name@domain.com
$ ./dbms db.dat s 2 ashkan more.name@domain.com

# get individual records by id
$ ./dbms db.dat g 1
1 pedram another.name@domain.com

# list all the records in the database
$ ./dbms db.dat l
0 mehrdad name@domain.com
1 pedram another.name@domain.com
2 ashkan more.name@domain.com

# remove one record from the database and list again
$ ./dbms db.dat d 1
$ ./dbms db.dat l
0 mehrdad name@domain.com
2 ashkan more.name@domain.com
```
