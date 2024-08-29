void p(void) {
    char buffer[76];

    fflush(1);
    gets(buffer);

    if (((unsigned int)__builtin_return_address(0) & 0xb0000000) == 0xb0000000) {
    	printf("(%p)\n", __builtin_return_address(0));
        exit(1);
    }

    puts(buffer);
    strdup(buffer);
}

int main(void) {
    p();
    return 0;
}