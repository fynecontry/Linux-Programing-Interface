#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Text to append file with
#define TEXT "What is your favourite color?\n"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s file_to _write_in\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open file parsed in append mode
    int fd;
    fd = open(argv[1], O_APPEND | O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "Error opening %s in append mode\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Seek to the begin of file
    off_t offset_location;
    if(lseek(fd, 0, SEEK_SET) == -1)
    {
        fprintf(stderr, "Could not seek to beginning of file\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write to file
    if ( write(fd, TEXT, sizeof(TEXT)) == -1)
    {
        fprintf(stderr, "Write failed\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    exit(EXIT_SUCCESS);    
}