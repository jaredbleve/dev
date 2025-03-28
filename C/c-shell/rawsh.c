#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// Manual string comparison
int str_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

// Manual null-terminated input reader using read()
int read_input(char *buf, int max_len) {
    int i = 0;
    char c;
    while (i < max_len - 1) {
        ssize_t n = read(0, &c, 1);
        if (n <= 0) break;
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}

// Manual tokenizer
int tokenize(char *input, char *args[], int max_args) {
    int count = 0;
    int in_token = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            input[i] = '\0';
            in_token = 0;
        } else if (!in_token) {
            if (count < max_args) {
                args[count++] = &input[i];
                in_token = 1;
            }
        }
    }
    args[count] = 0;
    return count;
}

int main() {
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];

    while (1) {
        // Prompt
        write(1, "rawsh> ", 7);

        // Read input
        int len = read_input(input, MAX_CMD_LEN);
        if (len == 0) break; // EOF

        // Tokenize
        int argc = tokenize(input, args, MAX_ARGS);

        if (argc == 0) continue;

        // Handle built-in "exit"
        if (str_eq(args[0], "exit")) break;

        // Fork and execute
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            _exit(1); // use _exit to avoid flushing buffers
        } else {
            waitpid(pid, 0, 0);
        }
    }

    return 0;
}

