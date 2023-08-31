# Summary

Because signals are not queued, a signal handler must sometimes be coded to deal with the possibility that multiple events of a particular type have occurred, even though only one signal was delivered. The issue of reentrancy affects how we can update global variables and limits the set of functions that we can safely call from a signal handler.

Instead of returning, a signal handler can terminate in a variety of other ways, including calling `_exit()`, terminating the process by sending a signal (`kill(), raise(), or abort()`), or performing a nonlocal goto. Using `sigsetjmp()` and `siglongjmp()` provides a program with explicit control of the treatment of the process signal mask when a nonlocal goto is performed.

We can use `sigaltstack()` to define an alternate signal stack for a process. This is an area of memory that is used instead of the standard process stack when invoking a signal handler. An alternate signal stack is useful in cases where the standard stack has been exhausted by growing too large (at which point the kernel sends a `SIGSEGV` signal to the process).

The `sigaction() SA_SIGINFO` flag allows us to establish a signal handler that receives additional information about a signal. This information is supplied via a `siginfo_t` structure whose address is passed as an argument to the signal handler.

When a signal handler interrupts a blocked system call, the system call fails with the error `EINTR`. We can take advantage of this behavior to, for example, set a timer on a blocking system call. Interrupted system calls can be manually restarted if desired. Alternatively, establishing the signal handler with the `sigaction() SA_RESTART` flag causes many (but not all) system calls to be automatically restarted.

## Exercise

#### 21-1. `TODO:` Implement abort().