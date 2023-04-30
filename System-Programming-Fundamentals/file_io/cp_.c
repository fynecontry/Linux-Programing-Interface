/* Like the cp command, 
./cp_ copies a regular file that contains holes (sequences of null bytes)
also creates corresponding holes in the target file.
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open input and output files */
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead, numWrite;
    char buf[BUF_SIZE];

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
    {
        fprintf(stderr, "Error opening %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw- */

    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
    {
        fprintf(stderr, "Error opening %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* Transfer data until we encounter end of input or an error */
    int holes;
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        for (ssize_t i = 0; i < numRead; i++)
        {
            /* count holes for preservation */
            if (buf[i] == '\0')
            {
              holes++;  
            }
            else
            {
                lseek(outputFd, holes, SEEK_CUR);
                numWrite = write(outputFd, &buf[i], 1);
                if (numWrite == -1)
                {
                    fprintf(stderr, "Failed to write byte\n");
                    close(inputFd);
                    close(outputFd);
                    exit(EXIT_FAILURE);
                }
                holes = 0;
            }
        }
    }

    close(inputFd);
    close(outputFd);

    exit(EXIT_SUCCESS);
}