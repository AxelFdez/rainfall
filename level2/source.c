#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void) {
    char buffer[76];
    unsigned int adresse;

    fflush(stdout);
    gets(buffer);

    if ((adresse & 0xb0000000) == 0xb0000000) {
    	printf("(%p)\n", adresse);
        exit(1);
    }
    puts(buffer);
    strdup(buffer);
}

int main(void) {
    p();
    return 0;
}