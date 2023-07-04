# Summary

A signal is a notification that some kind of event has occurred, and may be sent to a process by the kernel, by another process, or by itself. There is a range of standard signal types, each of which has a unique number and purpose.

Signal delivery is typically `asynchronous`, meaning that the point at which the signal interrupts execution of the process is unpredictable. In some cases (e.g., hardware-generated signals), signals are delivered `synchronously`, meaning that delivery occurs predictably and reproducibly at a certain point in the execution of a program.

**By default, a signal either is ignored, terminates a process (with or without a core dump), stops a running process, or restarts a stopped process.** The particular default action depends on the signal type. Alternatively, a program can use `signal()` or `sigaction()` to explicitly ignore a signal or to establish a programmer-defined signal handler function that is invoked when the signal is delivered. **For portability reasons, establishing a signal handler is best performed using `sigaction()`.**

A process (with suitable permissions) can send a signal to another process using `kill()`. Sending the null `signal (0)` is a way of determining if a particular process ID is in use.

Each process has a signal mask, which is the set of signals whose delivery is currently blocked. Signals can be added to and removed from the signal mask using `sigprocmask().`

If a signal is received while it is blocked, then it remains pending until it is unblocked. Standard signals can’t be queued; that is, a signal can be marked as pending (and thus later delivered) only once. A process can use the `sigpending()` system call to retrieve a signal set (a data structure used to represent multiple different signals) identifying the signals that it has pending.

The `sigaction()` system call provides more control and flexibility than `signal()` when setting the disposition of a signal. First, we can specify a set of additional signals to be blocked when a handler is invoked. In addition, various flags can be used to control the actions that occur when a signal handler is invoked. For example, there are flags that select the older unreliable signal semantics (not blocking the signal causing invocation of a handler, and having the disposition of the signal reset to its default before the handler is called).

Using `pause()`, a process can suspend execution until a signal arrives.

## Exercises

#### 20-1 As noted in Section 20.3, `sigaction()` is more portable than `signal()` for establishing a signal handler. Replace the use of signal() by sigaction() in the program in Listing 20-7 (sig_receiver.c).

```

$ ./sig_receiver 40 &
[2] 22273
$ ./sig_receiver: PID is 22273
./sig_receiver: sleeping for 40 seconds
./sig_receiver: pending signals are: 
                2 (Interrupt)
                10 (User defined signal 1)
./sig_receiver: signal 10 caught 1 time


$ ./sig_sender 22273 1000000 10 2
./sig_sender: sending signal 10 to process 22273 1000000 times
./sig_sender: exiting
```

#### 20-2 `TODO:` Write a program that shows that when the disposition of a pending signal is changed to be `SIG_IGN`, the program never sees (catches) the signal.

#### 20-3 `TODO:` Write programs that verify the effect of the `SA_RESETHAND` and `SA_NODEFER` flags when establishing a signal handler with `sigaction()`.

#### 20-4 `TODO:` Implement the `siginterrupt()` function described in Section 21.5 using `sigaction()`.