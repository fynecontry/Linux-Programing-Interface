#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1
#define min(m,n) ((m) < (n) ? (m) : (n))

// Prototype
void writeToFile(int fd, ssize_t numBytes);
void seekWrite(int fd, ssize_t numBytes);

int main(int argc, char *argv[])
{
    bool appendMode = true;
    if ((argc < 3 || argc > 4) || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "%s filename num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open file depending on x flag
    int fd;
    if (argc == 4)
    {
        appendMode = false;
        fd = open(argv[1], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    }
    else
    {
        fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    }

    if (fd == -1)
    {
        fprintf(stderr, "Error opening %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Write to file
    ssize_t numBytes = atoi(argv[2]);
    if (appendMode)
    {
        writeToFile(fd, numBytes);
    }
    else
    {
        seekWrite(fd, numBytes);
    }

    return 0;
}

void writeToFile(int fd, ssize_t numBytes)
{
    //char *buf[BUF_SIZE];
    size_t thisWrite, totWritten;
    for (totWritten = 0; totWritten < numBytes;
         totWritten += thisWrite)
    {
        thisWrite = min(BUF_SIZE, numBytes - totWritten);

        if (write(fd, "A", thisWrite) != thisWrite)
        {
            fprintf(stderr, "Partial write or Write failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void seekWrite(int fd, ssize_t numBytes)
{
    //char *buf[BUF_SIZE];
    size_t thisWrite, totWritten;
    for (totWritten = 0; totWritten < numBytes;
         totWritten += thisWrite)
    {
        thisWrite = min(BUF_SIZE, numBytes - totWritten);

        if (lseek(fd, 0, SEEK_END) == -1)
        {
            fprintf(stderr, "Could not seek to end of file\n");
            exit(EXIT_FAILURE);
        }

        if (write(fd, "S", thisWrite) != thisWrite)
        {
            fprintf(stderr, "Partial write or Write failed\n");
            exit(EXIT_FAILURE);
        }
    }
}