#include "systemcalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool do_system(const char *cmd) {
    int ret = system(cmd);
    return WIFEXITED(ret) && WEXITSTATUS(ret) == 0;
}

bool do_exec(int count, ...) {
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return false;
    } else if (pid == 0) {
        execvp(command[0], command);
        // execvp will only return if there's an error
        perror("Execvp failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    va_end(args);
}

bool do_exec_redirect(const char *outputfile, int count, ...) {
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return false;
    } else if (pid == 0) {
        FILE *output = freopen(outputfile, "w", stdout);
        if (output == NULL) {
            perror("freopen failed");
            exit(EXIT_FAILURE);
        }

        execvp(command[0], command);
        // execvp will only return if there's an error
        perror("Execvp failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    va_end(args);
}
