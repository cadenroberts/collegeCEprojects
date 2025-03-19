#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(void) {
    char input[4096];
    ssize_t bytes_read = read(STDIN_FILENO, input, 4095);
    if (bytes_read < 0) {
        dprintf(STDERR_FILENO, "Operation Failed\n");
        return 1;
    }
    input[bytes_read] = '\0';
    char *command = strtok(input, "\n");
    if (strcmp(command, "get") == 0) {
        char *filename = strtok(NULL, "\n");
        if (filename == NULL) {
            dprintf(STDERR_FILENO, "Invalid Command\n");
            return 1;
        }
        int fd = open(filename, O_RDONLY);
        if (fd < 0) {
            dprintf(STDERR_FILENO, "Invalid Command\n");
            return 1;
        }
        char buffer[4096];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, 4096)) > 0) {
            if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
                dprintf(STDERR_FILENO, "Operation Failed\n");
                close(fd);
                return 1;
            }
        }
        if (bytes_read < 0) {
            dprintf(STDERR_FILENO, "Operation Failed\n");
            close(fd);
            return 1;
        }
        close(fd);
        return 0;
    } else if (strcmp(command, "set") == 0) {
        char *filename = strtok(NULL, "\n");
        if (filename == NULL) {
            dprintf(STDERR_FILENO, "Invalid Command\n");
            return 1;
        }
        char *content_length_str = strtok(NULL, "\n");
        if (content_length_str == NULL) {
            dprintf(STDERR_FILENO, "Invalid Command\n");
            return 1;
        }
        size_t content_length = atoi(content_length_str);
        char *content = strtok(NULL, "");
        if (content == NULL || strlen(content) < content_length) {
            dprintf(STDERR_FILENO, "Invalid Command\n");
            return 1;
        }
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            dprintf(STDERR_FILENO, "Operation Failed\n");
            return 1;
        }
        if (write(fd, content, content_length) != (long) content_length) {
            dprintf(STDERR_FILENO, "Operation Failed\n");
            close(fd);
            return 1;
        }
        close(fd);
        dprintf(STDOUT_FILENO, "OK\n");
        return 0;
    } else {
        dprintf(STDERR_FILENO, "Invalid Command\n");
        return 1;
    }
    return 0;
}
