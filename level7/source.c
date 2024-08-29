int m() {
    printf("%s", &c);
    return 0;
}

int main(int argc, char **argv) {

    char *array1 = malloc(8);  // Alloue un espace pour deux pointeurs ou entiers.
    array1[0] = 1;
    array1[1] = (char)malloc(8);  // Alloue de l'espace pour une chaîne.

    char *array2 = malloc(8);  // Alloue un espace pour deux pointeurs ou entiers.
    array2[0] = 2;
    array2[1] = (char)malloc(8);  // Alloue de l'espace pour une chaîne.

    // Copie les arguments dans les espaces alloués.
    strcpy((char *)array1[1], argv[1]);
    strcpy((char *)array2[1], argv[2]);

    // Lit le mot de passe depuis un fichier.
    char c[68];  // Buffer pour stocker le mot de passe.
    int file = fopen("/home/user/level8/.pass", "r");
    fgets(c, sizeof(c), file);
	puts("~~");

    return 0;
}