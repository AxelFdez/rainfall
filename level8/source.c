#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char input[128];
    int *auth = NULL;
    char *service = NULL;

    while (1) {
        printf("%p, %p\n", auth, (void*)service);
        if (!fgets(input, 128, stdin)) {
            break;
        }

        if (strncmp(input, "auth ", 5) == 0) {
            auth = malloc(4);
            auth[0] = 0;
            if (strlen(input + 5) <= 30) {
                strcpy(auth, input + 5);
            }
        } else if (strncmp(input, "reset", 5) == 0) {
            free(auth);
        } else if (strncmp(input, "service", 7) == 0) {
            service = strdup(input + 7);
        } else if (strncmp(input, "login", 5) == 0) {
            if (auth[32] != 0) {
                system("/bin/sh");
            } else {
                fwrite("Password:\n", 1, 10, stdout);
            }
        }
    }

    return 0;
}