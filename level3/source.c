#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m;

void v(void)
{
  char tab[520];

  fgets(tab,512, stdin);
  printf(tab);
  if (m == 0x40) {
    fwrite("Wait what?!\n", 1, 0xc, 1);
    system("/bin/sh");
  }
  return;
}

void main(void)
{
  v();
  return;
}