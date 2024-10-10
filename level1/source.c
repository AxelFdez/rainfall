#include <stdio.h>
#include <stdlib.h>

void run(void)
{
  fwrite("Good... Wait what?\n", 1, 0x13, 2);
  system("/bin/sh");
  return;
}

void main(void)

{
  char buffer[76]; // 80 alloue + 4 padding (alignement)

  gets(buffer);
  return;
}