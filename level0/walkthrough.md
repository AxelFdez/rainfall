Read also: [./Resources/README.md](./Resources/README.md)

# Source
<details>
  <summary>Explication</summary>

  On se connecte en ssh au level0 puis on récupère le fichier pour le décompiler (avec scp).
  https://dogbolt.org/?id=3f5fe131-7680-47ca-885c-58fb74eb4a92#Snowman=204889 
  
  ```
  level0@RainFall:~$ ls -la
  total 737
  dr-xr-x---+ 1 level0 level0 	60     Mar  6  2016 .
  dr-x--x--x  1 root   root  		340    Sep 23  2015 ..
  -rw-r--r--  1 level0 level0		220    Apr  3  2012 .bash_logout
  -rw-r--r--  1 level0 level0   	3530   Sep 23  2015 .bashrc
  -rwsr-x---+ 1 level1 users  	747441 Mar  6  2016 level0
  -rw-r--r--  1 level0 level0		675    Apr  3  2012 .profile
  ```
  
  Pour rappel:
  ```
  ssh level0@127.0.0.1 -p 4242
  
  scp -P 4242 level0@127.0.0.1:/home/user/level0/level0 level0
  ```

</details>

```c
int main(int argc, char *argv[])
{
  char execv_args[2];
  int uid;
  int gid;
 
  if (atoi(&argv[1]) == 423) {
    execv_args[0] = strdup("/bin/sh");
    execv_args[1] = 0;
    gid = getegid();
    uid = geteuid();
    setresgid(gid,gid,gid);
    setresuid(uid,uid,uid);
    execv("/bin/sh", &execv_args);
  }
  else {
    fwrite("No !\n",1,5,stderr);
  }
  return 0;
}
```

# Explication

En regardant le code, on comprend vite qu’il faut mettre l’argument 423 pour que le programme
lance sh (avec les permissions de l'utilisateur du programme donc level1). Ce qui nous permet
d’obtenir le flag :

```
level0@RainFall:~$ ./level0 423
$ whoami
level1
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
