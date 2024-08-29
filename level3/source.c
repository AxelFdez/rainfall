void v(void)

{
  char tab[520];

  fgets(tab,0x200, 0);
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