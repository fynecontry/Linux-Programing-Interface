# Summary

The `stat()` system call retrieves information about a file (metadata), most of which is drawn from the file i-node. This information includes file ownership, file permissions, and file timestamps.

A program can update a file’s last access time and last modification time using `utime()`, `utimes()`, and various similar interfaces.

Each file has an associated user ID (owner) and group ID, as well as a set of permission bits. For permissions purposes, file users are divided into three categories: owner (also known as user), group, and other. Three permissions may be granted to each category of user: read, write, and execute. The same scheme is used with directories, although the permission bits have slightly different meanings. The chown() and chmod() system calls change the ownership and permissions of a file. The umask() system call sets a mask of permission bits that are always turned off when the calling process creates a file.

Three additional permission bits are used for files and directories. The set-user-ID and set-group-ID permission bits can be applied to program files to create programs that cause the executing process to gain privilege by assuming a different effective user or group identity (that of the program file). For directories residing on file systems mounted using the nogrpid (sysvgroups) option, the set-group-ID permission bit can be used to control whether new files created in the directory inherit their group ID from the process’s effective group ID or from the parent directory’s group ID. When applied to directories, the sticky permission bit acts as the restricted deletion flag.

I-node flags control the various behaviors of files and directories. Although originally defined for ext2, these flags are now supported on several other file systems.

## Exercises
#### Question 15-1
Use shell commands or write programs to verify or answer the following:

a) Removing all owner permissions from a file denies the file owner access, even though group and other do have access.
```
$ echo "hello world" > myFile
$ chmod o+w myFile 
$ ls -l myFile 
-rw-rw-rw- 1 ... ... 12 May  9 07:15 myFile
$ cat myFile 
hello world
$ chmod u-rw myFile 
$ cat myFile 
cat: myFile: Permission denied
```
b) On a directory with read permission but not execute permission, the names of files in the directory can be listed, but the files themselves can’t be accessed, regardless of the permissions on them.
```
$ ls -l .
total 4
drwxrwxr-x 2 ... ... 4096 May  9 07:23 mydir
$ cat mydir/myFile2
this is file2
$ chmod u-x mydir/
$ ls -l .
total 4
drw-rwxr-x 2 fyne-dev fyne-dev 4096 May  9 07:23 mydir
$ cat mydir/myFile2
cat: mydir/myFile2: Permission denied
$ ls mydir/
ls: cannot access 'mydir/myFile3': Permission denied
ls: cannot access 'mydir/myFile2': Permission denied
ls: cannot access 'mydir/myFile': Permission denied
myFile  myFile2  myFile3
```

#### Question 15-2
Do you expect any of a file’s three timestamps to be changed by the `stat()` system call? If not, explain why.
- The `st_atime`, `st_mtime`, and `st_ctime` fields contain, respectively, the times of last file access, last file modification, and last status change. The `stat()` system call is used to retrieve information about a file, such as its metadata and attributes, mostly drawn from the file i-node. However the `stat()` system call does not modify a file's timestamps retrieved from the inode it only provides them to the caller as part of the returned `stat()` structure.

#### Question 15-3
On a system running Linux 2.6, modify the program in Listing 15-1 (t_stat.c) so that the file timestamps are displayed with nanosecond accuracy.
```
TODO: Get a Linux 2.6 image and modify t_stat.c
```

#### Question 15-4
The access() system call checks permissions using the process’s real user and group IDs. Write a corresponding function that performs the checks according to the process’s effective user and group IDs.
```
TODO: Implement using `seteuid()` and `getuid()` wrapped in a function and `stat().st_mode`
```

#### Question 15-5
As noted in Section 15.4.6, `umask()` always sets the process umask and, at the same time, returns a copy of the old umask. How can we obtain a copy of the current process umask while leaving it unchanged?

Store the current umask of a process by first changing it and then set the umask again to the old umask
```
mode_t oldUmask = umask(0);
umask(oldUmask);
```

#### Question 15-6
The chmod a+rX file command enables read permission for all categories of user, and likewise enables execute permission for all categories of user if file is a directory or execute permission is enabled for any of the user categories for file, as demonstrated in the following example:
```
$ ls -ld dir file prog
dr--------  2 mtk users    48 May  4 12:28 dir
-r--------  1 mtk users 19794 May  4 12:22 file
-r-x------  1 mtk users 19336 May  4 12:21 prog
$ chmod a+rX dir file prog
$ ls -ld dir file prog
dr-xr-xr-x  2 mtk users    48 May  4 12:28 dir
-r--r--r--  1 mtk users 19794 May  4 12:22 file
-r-xr-xr-x  1 mtk users 19336 May  4 12:21 prog
```
`TODO:` Write a program that uses stat() and chmod() to perform the equivalent of chmod a+rX.