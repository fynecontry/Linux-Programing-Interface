# Summary
In order to perform I/O on a regular file, we must first obtain a file descriptor using `open()`. I/O is then performed using `read()` and `write()`. After performing all I/O, we should free the file descriptor and its associated resources using `close()`. These system calls can be used to perform I/O on all types of files.

The fact that all file types and device drivers implement the same I/O interface allows for universality of I/O, meaning that a program can typically be used with any type of file without requiring code that is specific to the file type.

For each open file, the kernel maintains a file offset, which determines the location at which the next read or write will occur. The file offset is implicitly updated by reads and writes. Using `lseek()`, we can explicitly reposition the file offset to any location within the file or past the end of the file. Writing data at a position beyond the previous end of the file creates a hole in the file. Reads from a file hole return bytes containing zeros.

The `ioctl()` system call is a catchall for device and file operations that don’t fit into the standard file I/O model. In order to perform I/O on a regular file, we must first obtain a file descriptor using `open()`. I/O is then performed using `read()` and `write()`. After performing all I/O, we should free the file descriptor and its associated resources using `close()`. These system calls can be used to perform I/O on all types of files.

The fact that all file types and device drivers implement the same I/O interface allows for universality of I/O, meaning that a program can typically be used with any type of file without requiring code that is specific to the file type.

For each open file, the kernel maintains a file offset, which determines the location at which the next read or write will occur. The file offset is implicitly updated by reads and writes. Using `lseek()`, we can explicitly reposition the file offset to any location within the file or past the end of the file. Writing data at a position beyond the previous end of the file creates a hole in the file. Reads from a file hole return bytes containing zeros.

The `ioctl()` system call is a catchall for device and file operations that don’t fit into the standard file I/O model.

## Exercises
 The tee command reads its standard input until end-of-file, writing a copy of the input to standard output and to the file named in its command-line argument. (We show an example of the use of this command when we discuss FIFOs in Section 44.7.) Implement tee using I/O system calls. By default, tee overwrites any existing file with the given name. Implement the –a command-line option (tee –a file), which causes tee to append text to the end of a file if it already exists. (Refer to Appendix B for a description of the getopt() function, which can be used to parse command-line options.)
```
$ df -h | ./tee_ output.txt
Filesystem      Size  Used Avail Use% Mounted on
tmpfs           787M   20M  767M   3% /run
/dev/sda4       121G   25G   90G  22% /
tmpfs           3.9G   73M  3.8G   2% /dev/shm
tmpfs           5.0M  4.0K  5.0M   1% /run/lock
tmpfs           3.9G     0  3.9G   0% /run/qemu
/dev/sda5        96M   32M   65M  33% /boot/efi
tmpfs           787M  108K  787M   1% /run/user/1000
```