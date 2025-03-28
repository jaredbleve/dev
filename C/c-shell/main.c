#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_CMD_LEN];

    while (1) {
        // Prompt
        printf("csh> ");
        fflush(stdout);

        // Read input
        if (!fgets(input, MAX_CMD_LEN, stdin)) {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        // Tokenize
        char *args[MAX_ARGS];
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Handle empty input
        if (args[0] == NULL) continue;

        // Built-in: exit
        if (strcmp(args[0], "exit") == 0) break;

        // Fork and execute
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
