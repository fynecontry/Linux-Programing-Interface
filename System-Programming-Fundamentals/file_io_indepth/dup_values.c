/* dup_values.c is a program that confirms
 duplicated file descriptors share a file offset value and open file status flags.*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int oldfd;
    int openFlags = O_RDWR | O_CREAT | O_APPEND;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    oldfd = open("f1", openFlags, filePerms);
    if (oldfd == -1)
    {
        fprintf(stderr, "Failed to open\n");
        exit(EXIT_FAILURE);
    }

    if (write(oldfd, "Hello ", 6) != 6)
    {
        fprintf(stderr, "Partial/Failed write\n");
        close(oldfd);
        exit(EXIT_FAILURE);
    }

    int newfd = dup(oldfd);
    if (newfd == -1)
    {
        fprintf(stderr, "Failed to dup\n");
        exit(EXIT_FAILURE);
    }

    if (write(newfd, "World\n", 6) != 6)
    {
        fprintf(stderr, "Partial/Failed write\n");
        close(oldfd);
        close(newfd);
        exit(EXIT_FAILURE);
    }

    /* Demonstrate that both file descriptors share the same file offset value */
    off_t oldfdOffset = lseek(oldfd, 0, SEEK_CUR);
    off_t newfdOffset = lseek(oldfd, 0, SEEK_CUR);

    int oldfdFlags = fcntl(oldfd, F_GETFL);
    int newfdFlags = fcntl(newfd, F_GETFL);

    if (oldfdOffset == newfdOffset)
    {
        printf("Both file descriptors share the same offset %li\n", newfdOffset);
    }

    if (oldfdFlags == newfdFlags)
    {
        printf("Both file descriptors share the same flags\n");
    }

    close(oldfd);
    close(newfd);

    exit(EXIT_SUCCESS);
}