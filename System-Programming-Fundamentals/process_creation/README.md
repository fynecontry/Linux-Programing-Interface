# Summary

The `fork()` system call creates a new process (the child) by making an almost exact duplicate of the calling process (the parent).
The parent and child processes are executing the same program text, **but they have separate copies of the stack, data, and heap segments**. The child’s stack, data, and heap segments are initially exact duplicates of the corresponding parts of the parent’s memory. After the `fork()`, each process can modify the variables in its stack, data, and heap segments without affecting the other process.


It is important to realize that after a fork(), it is indeterminate which of the two processes is next scheduled to use the CPU. In poorly written programs, this indeterminacy can lead to errors known as **race conditions**

### After `fork()`, should `child()` run first of `parent()`
- To see the argument for the **“children first after fork()”** behavior, consider what happens with copy-on-write semantics when the child of a `fork()` performs an immediate `exec()`. In this case, as the parent carries on after the `fork()` to modify data and stack pages, the kernel duplicates the to-be-modified pages for the child. Since the child performs an `exec()` as soon as it is scheduled to run, this duplication is wasted. According to this argument, it is better to schedule the child first, so that by the time the parent is next scheduled, no page copying is required.
- The argument for switching back to **“parent first after fork()”** in Linux 2.6.32 was based on the observation that, after a `fork()`, the parent’s state is already active in the CPU and its memory-management information is already cached in the hardware memory management unit’s `translation look-aside buffer (TLB)`. Therefore, running the parent first should result in better performance. This was informally verified by measuring the time required for kernel builds under the two behaviors.
- In conclusion, it is worth noting that the performance differences between the two behaviors are rather small, and won’t affect most applications.

### File Sharing Between Parent and Child
When a `fork()` is performed, the child receives duplicates of all of the parent’s file descriptors. The duplicated file descriptors in the child refer to the same open file descriptions as the corresponding descriptors in the parent. Consequently, these attributes of an open file are shared between the parent and child. For example, if the child updates the file offset, this change is visible through the corresponding descriptor in the parent. 

Sharing of open file attributes between the parent and child processes is frequently useful. For example, if the parent and child are both writing to a file, sharing the file offset ensures that the two processes don’t overwrite each other’s output. It does not, however, prevent the output of the two processes from being randomly intermingled.
If sharing of open file attributes in this manner is not required, then an application should be designed so that, after a `fork()`, the parent and child use different file descriptors, with each process closing unused descriptors (i.e., those used by the other process) immediately after forking. (If one of the processes performs an `exec()`, the `close-on-exec` flag can also be useful.)

![image](https://github.com/fynecontry/Linux-Programing-Interface/assets/27024731/64a289b1-e750-4fce-93bd-d08a102d3000)


### Why duplicating the parent's virtual memory pages into the new child process would be wasteful:
- A `fork()` is often followed by an immediate `exec()`, which replaces the process’s text with a new program and reinitializes the process’s data, heap, and stack segments.
- Modern UNIX implementations, including Linux, use two techniques to avoid such wasteful copying:
  - The kernel marks the text segment of each process as read-only, so that a process can’t modify its own code. This means that the parent and child can share the same text segment. The fork() system call creates a text segment for the child by building a set of per-process page-table entries that refer to the same physical memory page frames already used by the parent.
  - For the pages in the data, heap, and stack segments of the parent process, the kernel employs a technique known as `copy-on-write`. Initially, the kernel sets things up so that the page-table entries for these segments refer to the same physical memory pages as the corresponding page-table entries in the parent, and the pages themselves are marked read-only. After the `fork()`, the kernel traps any attempts by either the parent or the child to modify one of these pages, and makes a duplicate copy of the about-to-be-modified page.
  ![image](https://github.com/fynecontry/Linux-Programing-Interface/assets/27024731/28987609-8567-428c-b856-1c0e45e7132c)


### Two features distinguish the `vfork()` system call from `fork()` and make it more efficient:
- No duplication of virtual memory pages or page tables is done for the child process. Instead, the child shares the parent’s memory until it either performs a successful `exec()` or calls `_exit()` to terminate.
- Execution of the parent process is suspended until the child has performed an `exec()` or `_exit()`.

## Exercises
24-1. After a program executes the following series of fork() calls, how many new processes will result (assuming that none of the calls fails)?
> Depending on how calls are made, if are the calls are made in initial parent process, then 3 child processes will be created.

24-4. Experiment with the program in (`fork_whos_on_first.c`) on other UNIX implementations to determine how these implementations schedule the parent and child processes after a `fork()`.

**On a Linux box** 
```
$ ./fork_whos_on_first 4
0 child
0 parent
1 child
1 parent
2 child
2 parent
3 child
3 parent

$ ./fork_whos_on_first 6
0 parent
0 child
1 parent
1 child
2 parent
2 child
3 parent
3 child
4 parent
4 child
5 child
5 parent
```

24-5. Suppose that in the program in [fork_sig_sync_bak.c](https://github.com/fynecontry/Linux-Programing-Interface/blob/main/System-Programming-Fundamentals/process_creation/fork_sig_sync.c), the child process also needed to wait on the parent to complete some actions. What changes to the program would be required in order to enforce this?
```
$ ./fork_sig_sync_bak 
[16:34:49 14730] Parent about to wait for signal
[16:34:49 14731] Child started - doing some work
[16:34:51 14731] Child about to signal parent
[16:34:51 14731] Child then suspends operations, waiting for signal from parent
[16:34:51 14730] Parent got signal
[16:34:53 14730] Parent about to send its last signal Child
[16:34:53 14731] Child received signal to continue
```