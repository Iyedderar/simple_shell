#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t my_getline(char **lineptr, size_t *n, int fd) {
    // Check if the lineptr and n pointers are valid
    if (lineptr == NULL || n == NULL) {
        return -1;
    }

    ssize_t bytesRead = 0;
    size_t size = *n;
    char *line = *lineptr;

    // Initialize a buffer to read data
    char buffer[128]; // Adjust the buffer size as needed

    // Allocate memory for the line buffer if it's not already allocated
    if (line == NULL) {
        size = 128; // Initial buffer size
        line = (char *)malloc(size);
        if (line == NULL) {
            return -1; // Allocation failed
        }
    }

    char c;
    size_t i = 0;

    while (1) {
        ssize_t result = read(fd, &c, 1);

        if (result == -1) {
            free(line);
            return -1; // Error while reading
        } else if (result == 0) {
            break; // End of file
        }

        if (i >= size - 1) {
            // Resize the buffer if necessary
            size *= 2;
            char *new_line = (char *)realloc(line, size);
            if (new_line == NULL) {
                free(line);
                return -1; // Allocation failed
            }
            line = new_line;
        }

        line[i++] = c;

        if (c == '\n') {
            break; // End of line
        }

        bytesRead++;
    }

    line[i] = '\0';

    *lineptr = line;
    *n = size;

    return bytesRead;
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int file_descriptor = STDIN_FILENO; // Read from standard input

    printf("Enter a line of text (Ctrl+D to quit):\n");

    while ((read = my_getline(&line, &len, file_descriptor)) != -1) {
        printf("Line read (%zd characters):\n%s", read, line);
    }

    free(line);

    return 0;
}
