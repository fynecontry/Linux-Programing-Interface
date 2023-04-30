/*
The tee command implemented using I/O system calls.
./tee_.c reads from standard input until end-of-file,
writing a copy of the input to standard output and to the file named in
its command-line argument
*/

#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int opt;
    bool append;

    // Check if -a is parsed to CLI
    while ((opt = getopt(argc, argv, "a")) != -1)
    {
        switch (opt)
        {
            case 'a':
                append = true;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-a] out_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc)
    {
        fprintf(stderr, "Usage: %s [-a] out_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ssize_t numRead, numWrite;
    char buf[BUF_SIZE];
    int outputFd;
    mode_t filePerms;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    // if -a is parsed open output file descriptor in append mode
    if (append)
    {
        outputFd = open(argv[optind], O_WRONLY | O_CREAT | O_APPEND, filePerms);
    }
    else
    {
        outputFd = open(argv[optind], O_WRONLY | O_CREAT | O_TRUNC, filePerms);
    }

    if (outputFd == -1)
    {
        fprintf(stderr,"Error opening %s\n", argv[optind]);
        exit(EXIT_FAILURE);
    }

    // read from STDIN_FILENO
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        // Write to file
        numWrite = write(outputFd, buf, numRead);
        if (numWrite == -1)
        {
            fprintf(stderr, "Error writing to file\n");
            close(outputFd);
            exit(EXIT_FAILURE);
        }

        // Write to stdout
        numWrite = write(STDOUT_FILENO, buf, numRead);
        if (numWrite == -1)
        {
            fprintf(stderr, "Error writing to stdout\n");
            close(outputFd);
            exit(EXIT_FAILURE);
        }
    }

    if (close(outputFd) == -1)
    {
        fprintf(stderr,"Error closing %s file descriptor\n", argv[optind]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}