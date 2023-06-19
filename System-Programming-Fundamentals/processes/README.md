# Summary

Each process has a unique process ID and maintains a record of its parent’s process ID.

The virtual memory of a process is logically divided into a number of segments: `text, (initialized and uninitialized) data, stack, and heap`.

The stack consists of a series of frames, with a new frame being added as a function is invoked and removed when the function returns. Each frame contains the local variables, function arguments, and call linkage information for a single function invocation.

The command-line arguments supplied when a program is invoked are made available via the `argc` and `argv` arguments to `main()`. By convention, `argv[0]` contains the name used to invoke the program.

Each process receives a copy of its parent’s environment list, a set of name-value pairs. The global variable environ and various library functions allow a process to access and modify the variables in its environment list.

The `setjmp()` and `longjmp()` functions provide a way to perform a nonlocal goto from one function to another (unwinding the stack). In order to avoid problems with compiler optimization, we may need to declare variables with the volatile modifier when making use of these functions. Nonlocal gotos can render a program difficult to read and maintain, and should be avoided whenever possible.

## Exercises

#### 6-1 Compile the program in [mem_segments.c](https://github.com/fynecontry/Linux-Programing-Interface/tree/main/System-Programming-Fundamentals/processes/mem_segments.c), and list its size using ls –l. Although the program contains an array (mbuf) that is around 10 MB in size, the executable file is much smaller than this. Why is this?
```
$ ./mem_segments && ls -lh
The square of 9973 is 99460729
total 20K
-rwxrwxr-x 1 fyne-dev fyne-dev  16K Jun 15 09:07 mem_segments
-rw-rw-r-- 1 fyne-dev fyne-dev 1.1K Jun 15 09:06 mem_segments.c
-rw-rw-r-- 1 fyne-dev fyne-dev    0 Jun 15 09:10 README.md
```
> The size of the executeable is much smaller because although the program contains an array (mbuf) that is around 10 MB in size, `static char mbuf[10240000]` is an uninitialized data segement. And with uninitialized data segement (bss segement) is not necessary to allocate space for the uninitialized data. Instead, the executable merely needs to record the location and size required for the uninitialized data segment, and this space is allocated by the program loader at run time.

#### 6-2 `TODO:` Write a program to see what happens if we try to longjmp() into a function that has already returned.

#### 6-3 Implement `setenv()` and `unsetenv()` using `getenv()`, `putenv()`, and, where necessary, code that directly modifies environ. Your version of `unsetenv()` should check to see whether there are multiple definitions of an environment variable, and remove them all (which is what the glibc version of `unsetenv()` does).
> $ ./modify_env

