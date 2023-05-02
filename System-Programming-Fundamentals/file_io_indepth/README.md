# Summary

In the course of this chapter, we introduced the concept of atomicity, which is crucial to the correct operation of some system calls. In particular, the `open() O_EXCL` flag allows the caller to ensure that it is the creator of a file, and the `open() O_APPEND` flag ensures that multiple processes appending data to the same file don’t overwrite each other’s output.

The `fcntl()` system call performs a variety of file control operations, including changing open file status flags and duplicating file descriptors. Duplicating file descriptors is also possible using `dup()` and `dup2()`.

We looked at the relationship between file descriptors, open file descriptions, and file i-nodes, and noted that different information is associated with each of these three objects. Duplicate file descriptors refer to the same open file description, and thus share open file status flags and the file offset.

## Exercises
- Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?
  ```
  $ echo "my favourite color is orange" > example.txt
  $ ./append example.txt
  $ cat example.txt
  my favourite color is orange
  What is your favourite color?
  ```
  The data appears at the end of the file, this is because the file is opened with `O_APPEND` flag which means before each write(), the file offset is positioned at the end of the file, as if with lseek(). The modification of the file offset and the write operation are performed as a single atomic step. This helps prevent race conditions where multiple processes are trying to write to the same file.
  
- 