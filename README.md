# AAShell-OS-Project

AAShell este un mini-shell interactiv dezvoltat în C, realizat în cadrul unui proiect de echipă pentru cursul de Sisteme de Operare. Acesta a fost lucrat împreună cu colega mea, Alexandra Tunaru, așa cum se specifică în mesajul de bun venit:
```bash
void welcome_message() {
    printf("\n");
    printf(PURPLE "      Welcome to AAShell!\n" RESET);
    printf(BABY_PINK "      Authors: Alexandra IT and Andra MA | Group 232.\n" RESET);
    printf(BABY_PINK "      Type your commands below. For help, type 'help'.\n" RESET);
    printf(ORANGE "      To exit, type 'exit'\n\n" RESET);
    printf("\n");
}
```
AAShell oferă o experiență personalizată pentru gestionarea comenzilor Linux, imită funcționalitățile unui shell standard și include suport pentru execuția comenzilor de sistem, gestionarea proceselor în fundal, utilizarea pipe-urilor, operatorilor logici (&&, ||), redirecționarea intrării/ieșirii și un istoric al comenzilor. Este destinat utilizatorilor care doresc un control mai mare asupra proceselor și comenzilor din terminalul lor.

## Funcționalități

AAShell include următoarele funcționalități principale:

### 1. Istoricul comenzilor

- **Adăugare în istoric**: Comenzile introduse de utilizator sunt salvate pentru referințe viitoare.
- **Afișare istoric**: Permite vizualizarea comenzilor anterioare executate.
- **Comenzi relevante**: Utilizatorul poate naviga prin istoricul comenzilor și poate reexecuta comenzi anterioare.

### 2. Execuția comenzilor

- **Comenzi simple**: Executarea comenzilor de bază în sistemul Linux.
- **Pipe-uri (`|`)**: Permite execuția a două comenzi conectate prin pipe.
- **Redirecționare (`>`, `<`)**: Suportă redirecționarea intrării/ieșirii pentru comenzi.
- **Comenzi logice (`&&`, `||`)**: Permite execuția comenzilor în funcție de rezultatele anterioare.
- **Execuția comenzii cu status**: Afișează statusul comenzii executate și codul de ieșire.

### 3. Gestionarea proceselor în fundal

- **Listare procese în fundal**: Afișează procesele care rulează în fundal.
- **Adăugare și aducere în prim-plan**: Permite aducerea proceselor din fundal în prim-plan.
- **Oprirea proceselor din fundal**: Omoară procesele care rulează în fundal.

### 4. Suspendare și reluare procese

- **Suspendare (`SIGTSTP`)**: Permite suspendarea unui proces activ folosind `Ctrl+Z`.
- **Reluarea proceselor suspendate (`SIGCONT`)**: Permite reluarea proceselor suspendate și aducerea acestora în prim-plan cu comanda `fg`.

### 5. Gestionarea semnalelor și controlul execuției

- **Suspendarea proceselor**: La apăsarea `Ctrl+Z`, procesul curent este suspendat.
- **Reluarea procesului suspendat**: Se poate relua procesul suspendat folosind comanda `fg`.

## Structuri și Variabile Globale

- `BackgroundProcess`: Structură utilizată pentru stocarea proceselor din fundal.
- `background_processes`: Listă care stochează procesele în fundal.
- `background_count`: Contor pentru numărul de procese din fundal.
- `history`: Listă care salvează istoricul comenzilor.
- `history_count`: Contor pentru numărul de comenzi salvate în istoric.
- `current_pid`: PID-ul procesului curent.
- `suspended_pid`: PID-ul procesului suspendat.

## Interfață și Mesaje Interactive

- `print_prompt`: Afișează prompt-ul shell-ului într-un format personalizat.
- `welcome_message`: Mesaj de bun venit la pornirea shell-ului.

## Cum se compilează și rulează

Pentru a compila și rula shell-ul personalizat:

### 1. Compilează proiectul:
```bash
gcc shell_so.c -o aashell
```

### 2. Rulează shell-ul personalizat:
```bash
./aashell
```
### 3. Pentru a ieși:
``` bash
exit
```


### Exemplu de utilizare
1. Execuția comenzilor
Introducerea comenzilor va rula aplicațiile respective:
``` bash

$ ls
$ pwd
$ echo "Hello, World!"

```
2. Utilizarea pipe-urilor
Pentru a conecta două comenzi prin pipe:

```bash

$ cat file.txt | grep "error"
```
3. Redirecționarea intrării și ieșirii
Redirecționează ieșirea unei comenzi într-un fișier:

```bash
$ ls > output.txt
```
4. Comenzi logice
Execută o comandă doar dacă prima a avut succes:

```bash
$ mkdir new_folder && cd new_folder
```
5. Gestionarea proceselor în fundal
Rularea unui proces în fundal:
```bash
$ sleep 100 &
```
Vizualizarea proceselor din fundal:
```bash
$ jobs
```
Adăugarea unui proces din fundal în prim-plan:
```bash
$ fg %1
```
6. Suspendarea și reluarea proceselor
Suspendarea procesului curent:
```bash
Ctrl+Z
```
Reluarea unui proces suspendat:
```bash
$ fg
```
