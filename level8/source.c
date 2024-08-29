#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char local_90[128];
    char local_8b[2];
    char acStack_89[125];
    int *auth = NULL;
    char *service = NULL;

    while (1) {
        printf("%p, %p\n", (void*)auth, (void*)service);
        if (!fgets(local_90, 128, stdin)) {
            return 0;
        }

        if (strncmp(local_90, "auth ", 5) == 0) {
            auth = malloc(4);
            *auth = 0;
            if (strlen(local_8b) < 30) {
                strcpy((char *)auth, local_8b);
            }
        } else if (strncmp(local_90, "reset", 5) == 0) {
            free(auth);
        } else if (strncmp(local_90, "service", 7) == 0) {
            service = strdup(acStack_89);
        } else if (strncmp(local_90, "login", 5) == 0) {
            if (auth && auth[8] == 0) {
                fwrite("Password:\n", 1, 10, stdout);
            } else {
                system("/bin/sh");
            }
        }
    }

    return 0;
}