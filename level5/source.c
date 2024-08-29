void o(void)
{
  system("/bin/sh");
  exit(1);
}

void n(void)
{
  char s [520];

  fgets(s,0x200,1);
  printf(s);
  exit(1);
}

void main(void)
{
  n();
  return;
}