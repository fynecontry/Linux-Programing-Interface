# Summary

## Summary of I/O Buffering
The diagram provides an overview of the buffering employed (for output files) by the stdio library and the kernel, along with the mechanisms for controlling each type of buffering. Traveling downward through the middle of this diagram, we see the transfer of user data by the stdio library functions to the stdio buffer, which is maintained in user memory space. When this buffer is filled, the stdio library invokes the write() system call, which transfers the data into the kernel buffer cache (maintained in kernel memory). Eventually, the kernel initiates a disk operation to transfer the data to the disk.

The left side of the diagram shows the calls that can be used at any time to explicitly force a flush of either of the buffers. The right side shows the calls that can be used to make flushing automatic, either by disabling buffering in the stdio library or by making file output system calls synchronous, so that each `write()` is immediately flushed to the disk.

![image](https://user-images.githubusercontent.com/27024731/235504584-e311058f-d801-4e5d-9d66-d356fbb26950.png)


Buffering of input and output data is performed by the kernel, and also by the stdio library. In some cases, we may wish to prevent buffering, but we need to be aware of the impact this has on application performance. Various system calls and library functions can be used to control kernel and stdio buffering and to perform one-off buffer flushes.


## Exercises

- Using the time built-in command of the shell, try timing the operation of the program in Listing 4-1 (copy.c) on your system.
  - Experiment with different file and buffer sizes. You can set the buffer size using the `–DBUF_SIZE=nbytes` option when compiling the program.
      
    `with BUF_SIZE 1`
    ```
    $ time ./copy some_text.txt some_new_text.txt

    real    0m0.342s
    user    0m0.076s
    sys     0m0.257s
    ```
    
    `with default BUF_SIZE 1024`
    ```
    $ time ./copy some_text.txt some_new_text.txt

    real    0m0.007s
    user    0m0.007s
    sys     0m0.000s
    ```
    
    `with BUF_SIZE 4096`
    ```
    $ time ./copy some_text.txt some_new_text.txt

    real    0m0.006s
    user    0m0.006s
    sys     0m0.000s
    ```
  - Modify the open() system call to include the O_SYNC flag. How much difference does this make to the speed for various buffer sizes?
    
    `with BUF_SIZE 1024`
    ```
    $ time ./copy some_text.txt some_new_text.txt

    real    0m0.008s
    user    0m0.005s
    sys     0m0.004s
    ```
    
    `with BUF_SIZE 4096`
    ```
    $ time ./copy some_text.txt some_new_text.txt

    real    0m0.008s
    user    0m0.006s
    sys     0m0.002s
    ```
- Time the operation of the `filebuff/write_bytes.c` program (provided in the source code distribution for this book) for various buffer sizes and file systems.
    ```
    $ time ./write_bytes some_text.txt 100000000 1024

    real    0m0.300s
    user    0m0.052s
    sys     0m0.236s

    $ time ./write_bytes some_text.txt 100000000 4096

    real    0m0.073s
    user    0m0.009s
    sys     0m0.064s

    $ time ./write_bytes some_text.txt 100000000 16384

    real    0m0.062s
    user    0m0.008s
    sys     0m0.054s
    ```
- What is the effect of the following statements?

  - fflush(fp); `forces the data referred by fd to be write incase of an output stream and discarded incase of an input stream`
  - fsync(fileno(fp)); `causes buffered data and metadata associated with the file descriptor returned by fileno(fp) stream to be flushed to disk. Calling fsync() forces the file to the synchronized I/O file integrity completion state.`

-  Explain why the output of the following code differs depending on whether standard output is redirected to a terminal or to a disk file.

   - printf("If I had more time, \n");
   - write(STDOUT_FILENO, "I would have written you a shorter letter.\n", 43);

    I/O system calls like `write()` transfer data directly to the kernel buffer cache, while the stdio library like `printf()` waits until the stream’s user-space buffer is full before calling `write()` to transfer that buffer to the kernel buffer cache. 

    Under normal operations, the output will be:
    ```
    I would have written you a shorter letter.
    If I had more time, 
    ```
    However output may differ is `setvbuf()` function is enabled with modes: `_IOLBF` and `_IOFBF`
    