# Summary
System calls allow processes to request services from the kernel. Even the simplest system calls have a significant overhead by comparison with a user-space function call, since the system must temporarily switch to kernel mode to execute the system call, and the kernel must verify system call arguments and transfer data between user memory and kernel memory.

- The standard C library provides a multitude of library functions that perform a wide range of tasks. Some library functions employ system calls to do their work; others perform tasks entirely within user space. On Linux, the usual standard C library implementation that is used is glibc.

- Most system calls and library functions return a status indicating whether a call has succeeded or failed. Such status returns should always be checked.

- We introduced a number of functions that we have implemented for use in the example programs in this book. The tasks performed by these functions include diagnosing errors and parsing command-line arguments.

- We discussed various guidelines and techniques that can help us write portable system programs that run on any standards-conformant system.

- When compiling an application, we can define various feature test macros that control the definitions exposed by header files. This is useful if we want to ensure that a program conforms to some formal or implementation-defined standard(s).

- We can improve the portability of system programs by using the system data types defined in various standards, rather than native C types. SUSv3 specifies a wide range of system data types that an implementation should support and that an application should employ.


## Exercise

#### When using the Linux-specific reboot() system call to reboot the system, the second argument, magic2, must be specified as one of a set of magic numbers (e.g., LINUX_REBOOT_MAGIC2). What is the significance of these numbers? (Converting them to hexadecimal provides a clue.)

According to `man 2 reboot`
```
The reboot() call reboots the system, or enables/disables the reboot keystroke (abbreviated CAD, since the default is Ctrl-Alt-Delete; it can be changed using loadkeys(1)).

This system call fails (with the error EINVAL) unless magic equals LINUX_REBOOT_MAGIC1 (that is, 0xfee1dead) and magic2 equals LINUX_REBOOT_MAGIC2 (that is, 672274793). However, since 2.1.17 also LINUX_REBOOT_MAGIC2A (that is, 85072278) and since 2.1.97 also LINUX_REBOOT_MAGIC2B (that is, 369367448) and since 2.5.71 also LINUX_REBOOT_MAGIC2C (that is, 537993216) are permitted as values for magic2. (The hexadecimal values of these constants are meaningful.)
```

Converting the `LINUX_REBOOT_MAGIC` numbers to hex denotes Linus Torvalds and daughters birthdays - [source](https://stackoverflow.com/questions/4808748/magic-numbers-of-the-linux-reboot-system-call)