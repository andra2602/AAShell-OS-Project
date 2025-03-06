## AAShell-OS-Project

AAShell is an interactive mini-shell developed in C as part of a team project for the Operating Systems course. It was created together with my colleague, Alexandra Tunaru, as mentioned in the welcome message:

```c
void welcome_message() {
    printf("\n");
    printf(PURPLE "      Welcome to AAShell!\n" RESET);
    printf(BABY_PINK "      Authors: Alexandra IT and Andra MA | Group 232.\n" RESET);
    printf(BABY_PINK "      Type your commands below. For help, type 'help'.\n" RESET);
    printf(ORANGE "      To exit, type 'exit'\n\n" RESET);
    printf("\n");
}
```

AAShell provides a customized experience for managing Linux commands, mimicking the functionalities of a standard shell while including support for executing system commands, managing background processes, using pipes, logical operators (`&&`, `||`), input/output redirection, and command history. It is designed for users who want greater control over processes and commands in their terminal.

## Features

AAShell includes the following main features:

### 1. Command History

- **Add to history**: User-entered commands are saved for future reference.
- **Display history**: Allows viewing previously executed commands.
- **Relevant commands**: Users can navigate through the command history and re-execute past commands.

### 2. Command Execution

- **Simple commands**: Execute basic Linux system commands.
- **Pipes (`|`)**: Allows executing two commands connected via a pipe.
- **Redirection (`>`, `<`)**: Supports input/output redirection for commands.
- **Logical commands (`&&`, `||`)**: Enables command execution based on previous results.
- **Command execution status**: Displays the status and exit code of executed commands.

### 3. Background Process Management

- **List background processes**: Displays processes running in the background.
- **Add and bring to foreground**: Allows moving background processes to the foreground.
- **Stopping background processes**: Kills processes running in the background.

### 4. Suspending and Resuming Processes

- **Suspend (`SIGTSTP`)**: Allows suspending an active process using `Ctrl+Z`.
- **Resume suspended processes (`SIGCONT`)**: Allows resuming suspended processes and bringing them to the foreground using the `fg` command.

### 5. Signal Handling and Execution Control

- **Suspending processes**: Pressing `Ctrl+Z` suspends the current process.
- **Resuming a suspended process**: The suspended process can be resumed using the `fg` command.

## Structures and Global Variables

- `BackgroundProcess`: Structure used to store background processes.
- `background_processes`: List storing background processes.
- `background_count`: Counter for the number of background processes.
- `history`: List saving the command history.
- `history_count`: Counter for the number of saved history commands.
- `current_pid`: The PID of the current process.
- `suspended_pid`: The PID of the suspended process.

## Interface and Interactive Messages

- `print_prompt`: Displays the shell prompt in a customized format.
- `welcome_message`: Welcome message displayed when the shell starts.

## How to Compile and Run

To compile and run the custom shell:

### 1. Compile the project:
```bash
gcc shell_so.c -o aashell
```

### 2. Run the custom shell:
```bash
./aashell
```

### 3. To exit:
```bash
exit
```

### Usage Examples

1. **Executing Commands**  
   Entering commands will run the respective applications:
   ```bash
   $ ls
   $ pwd
   $ echo "Hello, World!"
   ```

2. **Using Pipes**  
   Connecting two commands via a pipe:
   ```bash
   $ cat file.txt | grep "error"
   ```

3. **Input and Output Redirection**  
   Redirect the output of a command to a file:
   ```bash
   $ ls > output.txt
   ```

4. **Logical Commands**  
   Execute a command only if the first one succeeds:
   ```bash
   $ mkdir new_folder && cd new_folder
   ```

5. **Managing Background Processes**  
   Running a process in the background:
   ```bash
   $ sleep 100 &
   ```
   Viewing background processes:
   ```bash
   $ jobs
   ```
   Bringing a background process to the foreground:
   ```bash
   $ fg %1
   ```

6. **Suspending and Resuming Processes**  
   Suspending the current process:
   ```bash
   Ctrl+Z
   ```
   Resuming a suspended process:
   ```bash
   $ fg
   ```

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
