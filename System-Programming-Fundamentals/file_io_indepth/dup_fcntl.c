/* dup_fcntl - Implementation dup() and dup2() system calls using fcntl()*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Prototype
int dup(int oldfd);
int dup2(int oldfd, int newfd);

int main(int argc, char *argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage:\t%s file [dup | dup2 x]\n\tx: arg for new fd\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int oldfd, newfd;
    int openFlags = O_RDWR | O_CREAT | O_APPEND;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    oldfd = open(argv[1], openFlags, filePerms);

    if (strcmp(argv[2], "dup2") == 0)
    {
        newfd = dup2(oldfd, atoi(argv[3]));
    }
    else {
        newfd = dup(oldfd);
    }

    if (newfd == -1)
    {
        fprintf(stderr, "Failed to dup fd\n");
        close(oldfd);
        close(newfd);
        exit(EXIT_FAILURE);
    }

    // Close old file descriptor
    close(oldfd);

    // Write to file using newfd
    if(write(newfd, "Hello World\n", 12) != 12)
    {
        fprintf(stderr, "Partial/Failed write\n");
        close(newfd);
        exit(EXIT_FAILURE);
    }
    close(newfd);

    exit(EXIT_SUCCESS);

}

int dup(int oldfd)
{
    // Check if oldfd is a valid fd
    if (fcntl(oldfd, F_GETFL) == -1)
    {
        errno = EBADF;
        return -1;
    }

    return fcntl(oldfd, F_DUPFD, 0);
}

int dup2(int oldfd, int newfd)
{
    // Check if oldfd is valid fd
    if (fcntl(oldfd, F_GETFL) == -1)
    {
        errno = EBADF;
        return -1;
    }

    // Check if oldfd == newfd, in which case fcntl does not have to be called
    if (oldfd == newfd)
    {
        return newfd;
    }

    return fcntl(oldfd, F_DUPFD, newfd);
}