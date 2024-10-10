#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int var1 = atoi(argv[1]);
	char buffer[40];
    int magicNumber = 0x574f4c46;  // "FLOW" in hex, assuming little-endian
    int result;

    if (var1 < 10) {
        // Copying data into the buffer, interpreting argv[2] as an address to a memory block
        memcpy(buffer, argv[2], (var1 * sizeof(int)) * 4);

        if (var1 == magicNumber) {
            execl("/bin/sh", "sh", NULL);  // If the magic number condition is met, execute a shell
        }
        return(0);
    }
    return(1);
}
