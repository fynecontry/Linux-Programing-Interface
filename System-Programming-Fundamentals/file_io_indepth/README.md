# Summary

In the course of this chapter, we introduced the concept of atomicity, which is crucial to the correct operation of some system calls. In particular, the `open() O_EXCL` flag allows the caller to ensure that it is the creator of a file, and the `open() O_APPEND` flag ensures that multiple processes appending data to the same file don’t overwrite each other’s output.

The `fcntl()` system call performs a variety of file control operations, including changing open file status flags and duplicating file descriptors. Duplicating file descriptors is also possible using `dup()` and `dup2()`.

We looked at the relationship between file descriptors, open file descriptions, and file i-nodes, and noted that different information is associated with each of these three objects. Duplicate file descriptors refer to the same open file description, and thus share open file status flags and the file offset.

## Exercises
#### Question 5-1
Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?
  ```
  $ echo "my favourite color is orange" > example.txt
  $ ./append example.txt
  $ cat example.txt
  my favourite color is orange
  What is your favourite color?
  ```
  The data appears at the end of the file, this is because the file is opened with `O_APPEND` flag which means before each write(), the file offset is positioned at the end of the file, as if with lseek(). The modification of the file offset and the write operation are performed as a single atomic step. This helps prevent race conditions where multiple processes are trying to write to the same file.

#### Question 5-2
This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary. Write a program that takes up to three command-line arguments:

  `$ atomic_append filename num-bytes [x]`

  This program should open the specified filename (creating it if necessary) and append num-bytes bytes to the file by using write() to write a byte at a time. By default, the program should open the file with the O_APPEND flag, but if a third command-line argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run two instances of this program at the same time without the x argument to write 1 million bytes to the same file:

  `$ atomic_append f1 1000000 & atomic_append f1 1000000`

  Repeat the same steps, writing to a different file, but this time specifying the x argument:

  `$ atomic_append f2 1000000 x & atomic_append f2 1000000 x`

  List the sizes of the files f1 and f2 using ls –l and explain the difference.
  ```
   ls -l f1 f2
   -rw------- 1 ... ... 2000000 May  4 05:18 f1
   -rw------- 1 ... ... 1470421 May  4 05:18 f2
  ```
f1 and f2 have different sizes as a result of how they both append to file when writing, in the case of f1 the atomicity is guaranteed by using just write() even when one of the process is interrupted between code execution. However in f2, if the first process executing the code is interrupted between the lseek() and write() calls by a second process doing the same thing, then both processes will set their file offset to the same location before writing, and when the first process is rescheduled, it will overwrite the data already written by the second process, causing a race condition because the results depend on the order of scheduling of the two processes.