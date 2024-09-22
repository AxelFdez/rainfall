Read also: [./Resources/README.md](./Resources/README.md)

# Source

https://dogbolt.org/?id=4b673d9a-5976-420c-b0e4-b4225e0977c3

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char ptr[132]; // [esp+18h] [ebp-90h] BYREF
  FILE *v5; // [esp+9Ch] [ebp-Ch]

  v5 = fopen("/home/user/end/.pass", "r");
  memset(ptr, 0, sizeof(ptr));
  if ( !v5 || argc != 2 )
	return -1;
  fread(ptr, 1u, 0x42u, v5);
  ptr[65] = 0;
  ptr[atoi(argv[1])] = 0;
  fread(&ptr[66], 1u, 0x41u, v5);
  fclose(v5);
  if ( !strcmp(ptr, argv[1]) )
	execl("/bin/sh", "sh", 0);
  else
	puts(&ptr[66]);
  return 0;
}
```

# Explication 

Alors :
 - Le programme prend 1 argument.
 - La ligne lue est limitée à 65 caractères ou en fonction du nombre indiqué dans `atoi`.  
   DONC si on met `0`, strcmp va cmp aucun caractère `!strcmp("", "0")` une chaîne vide peut
   être aussi considérée comme `0` `!strcmp("", "")`.

```
./bonus3 ""
$ cat /home/user/end/.pass
```

Le flag est donc:
```
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
