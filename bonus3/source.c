#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char buffer[66] = {0}; // Buffer de taille suffisante pour stocker les données lues.
    char message[66] = {0}; // Autre buffer pour stocker les messages du fichier.
    FILE *file;

    file = fopen("/home/user/end/.pass", "r"); // Ouverture du fichier .pass
    if (file == NULL || argc != 2) {
        return -1; // Retourne une erreur si le fichier ne s'ouvre pas ou si le nombre d'arguments est incorrect.
    }

    fread(buffer, 1, 66, file); // Lecture de 66 caractères du fichier.
    buffer[atoi(argv[1])] = 0; // Utilise la valeur convertie du deuxième argument pour placer un caractère nul quelque part dans le buffer.

    fread(message, 1, 65, file); // Lecture de la prochaine partie du fichier.
    fclose(file); // Fermeture du fichier.

    if (strcmp(buffer, argv[1]) == 0) {
        execl("/bin/sh", "sh", NULL); // Si le contenu de buffer correspond au second argument, exécute un shell.
    } else {
        puts(message); // Sinon, affiche le message lu du fichier.
    }

    return 0; // Retourne 0 en cas de succès.
}
