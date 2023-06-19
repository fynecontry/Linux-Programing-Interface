#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function prototypes
int setenv2(const char *name, const char *value, int overwrite);
int unsetenv2(char *name);
void displayENV();

extern char **environ;

int main(int argc, char *argv[])
{
    char **ep;

    setenv2("USER", "Gabe", 0);       /* Ensures value of name is not changed if it exists, since overwrite is 0 */
    setenv2("USER", "Fynecontry", 1); /* Ensures value of name is set or overwritten even if it exists */

    displayENV();

    unsetenv2("USER");

    printf("===================================\nenv list after unsetenv2 operation\n===================================\n");

    displayENV();

    exit(EXIT_SUCCESS);
}

// Sets environmental variables using args name and value
int setenv2(const char *name, const char *value, int overwrite)
{
    if (name == 0 || value == 0)
    {
        errno = EINVAL;
        fprintf(stderr, "name or value is NULL");
        return -1;
    }

    if (getenv(name) != 0 && overwrite == 0)
    {
        return 0;
    }

    // Make a copy of string pointed to by name
    char *str = malloc(strlen(name) + strlen(value) + 2);
    if (str == 0)
    {
        errno = ENOMEM;
        fprintf(stderr, "Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    sprintf(str, "%s=%s", name, value);

    if (putenv(str) != 0)
    {
        errno = ENOMEM;
        fprintf(stderr, "Failed to add to environ.");
        return -1;
    }

    return 0;
}

// Remove name variable, if present in environ
int unsetenv2(char *name)
{
    if (name == 0)
    {
        errno = EINVAL;
        fprintf(stderr, "Invalid name is NULL");
        return -1;
    }

    // Search if variable exist in environ
    if (getenv(name) == 0)
    {
        return 0;
    }

    /*
    Check for multiple definitions of an environment variable,
    Removing them all (which is glibc implementation version of unsetenv())
    */

    for (char **ep = environ; *ep != NULL; ep++)
    {
        if (strncmp(*ep, name, strlen(name)) == 0)
        {
            if (putenv(name) != 0)
            {
                fprintf(stderr, "Failed to remove %s variable", name);
                return -1;
            }
        }
    }

    return 0;
}

// Displays process env list
void displayENV()
{
    for (char **ep = environ; *ep != NULL; ep++)
        puts(*ep);
}