void n()
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void m()

{
  puts("Nope");
  return;
}

void main(int argc,char** argv)

{
  char *dest;
  dest = (char *)malloc(64);

  strcpy(dest, argv[1]);
  m();
  return;
}