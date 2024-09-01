#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int language;  // Supposons que cette variable est définie ailleurs.

void greetuser(void) {
    char *greeting;

    if (language == 1) {
        // Salutation en langue 1
        strcpy(greeting, "Hyvää päivää!");  // Supposons que c'est une salutation en finnois.
    } else if (language == 2) {
        // Salutation en langue 2
        strcpy(greeting, "Goedemiddag!");  // Supposons que c'est une salutation en néerlandais.
    } else {
        // Salutation par défaut (langue 0)
        strcpy(greeting, "Hello!");
    }
	strcat(greeting, local_60);
    puts(greeting);  // Affiche la salutation.
}

int main(int argc, char *argv[]) {
    int iVar2;
    char local_60[10];
    char acStack_38[36];
    char *local_14;

    if (argc == 3) {
        memset(local_60, 0, sizeof(local_60));
        strncpy((char *)local_60, argv[1], 0x28);  // Copie jusqu'à 40 caractères de argv[1]
        strncpy(acStack_38, argv[2], 0x20);       // Copie jusqu'à 32 caractères de argv[2]

        local_14 = getenv("LANG");  // Récupère la variable d'environnement LANG
        if (local_14 != NULL) {
            if (memcmp(local_14, "\x3d\x08\x04\x87", 2) == 0) {
                language = 1;
            } else if (memcmp(local_14, "\x40\x08\x04\x87", 2) == 0) {
                language = 2;
            }
        }

        greetuser();
    } else {
        return 1;
    }
    return 0;
}
