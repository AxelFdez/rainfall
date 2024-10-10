#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int language;

void greetuser(char *buffer) {
    char greeting[64];

    if (language == 1) {
        strcpy(greeting, "Hyvää päivää!");
    } else if (language == 2) {
        strcpy(greeting, "Goedemiddag!");
    } else {
        strcpy(greeting, "Hello ");
    }
	strcat(greeting, buffer);
    puts(greeting);
}

int main(int argc, char *argv[]) {
    char buffer[76];
    char *environment = NULL;

    if (argc == 3) {
        memset(buffer, 0, sizeof(buffer));
        strncpy(buffer, argv[1], 0x28);  // Copie jusqu'à 40 caractères de argv[1]
        strncpy(buffer + 40, argv[2], 0x20);       // Copie jusqu'à 32 caractères de argv[2]

        environment = getenv("LANG");  // Récupère la variable d'environnement LANG
        if (environment != NULL) {
            if (memcmp(environment, "fi", 2) == 0) {
                language = 1;
            } else if (memcmp(environment, "nl", 2) == 0) {
                language = 2;
            }
        }

        greetuser(buffer);
    } else {
        return 1;
    }
    return 0;
}
