// la linia 291 se implementeaza functionalitatile importante
// --> execute_with_status
// execvp = se inlocuieste procesul curent cu un alt program specificat de utilizator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h> // Pentru redirectionari


#define MAX_BACKGROUND_PROCESSES 100 //structura pt a stoca procesele din fundal
#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100
#define HISTORY_SIZE 10


//culori
#define RESET "\033[0m"
#define PINK "\033[95m"
#define PURPLE "\033[35m"
#define BABY_BLUE "\033[94m"
#define BABY_PINK "\033[38;5;218m"
#define ORANGE "\033[38;5;214m"


typedef struct {
    pid_t pid;
    char command[MAX_COMMAND_LENGTH];
} BackgroundProcess;

BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
int background_count = 0;

char *history[HISTORY_SIZE];  
int history_count = 0;
int current_pid = -1; 
int suspended_pid = -1;



void print_prompt() {
    char cwd[MAX_COMMAND_LENGTH];
    char *username = getenv("USER"); //obtinem numele utilizatorului
    if (getcwd(cwd, sizeof(cwd)) == NULL) { //se obtine calea absoluta a directorului de lucru si se stocheaza in cwd
        perror("getcwd error");
        return;
    }

    printf(BABY_PINK "AAShell > " RESET);
}

void welcome_message() {
    printf("\n");
    printf(PURPLE "      Welcome to AAShell!\n" RESET);
    printf(BABY_PINK "      Authors: Alexandra IT and Andra MA | Group 232.\n" RESET);
    printf(BABY_PINK "      Type your commands below. For help, type 'help'.\n" RESET);
    printf(ORANGE "      To exit, type 'exit'\n\n" RESET);
    printf("\n");
}

void display_help() {
    printf(PURPLE "Supported Commands:\n" RESET);
    printf(BABY_BLUE "  history   " RESET "- Show command history\n");
    printf(BABY_BLUE "  jobs      " RESET "- List background processes\n");
    printf(BABY_BLUE "  fg [n]    " RESET "- Bring background job [n] to foreground\n");
    printf(BABY_BLUE "  kill [n]  " RESET "- Kill background job [n]\n");
    printf(BABY_BLUE "  exit      " RESET "- Exit the shell\n");
    printf(ORANGE "Built-in commands (pwd, ls, cd, etc.) and system commands are also supported.\n" RESET);
}


void add_to_history(const char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count] = strdup(command); //cu strdup se creeaza o copie exacta a unui sir de caractere si 
                                                //se aloca dinamic memoria necesara acestei copii
        history_count++;
    } else {
        // daca s-a ajuns la capacitate maxima se va sterge cea mai veche comanda
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(command);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}



void read_command(char *command) {
    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("fgets error");
        exit(EXIT_FAILURE);
    }
    // se elimina \n
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }
}

void parse_command(char *command, char **args) {
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
    args[i] = NULL; // Ne asiguram ca ultimul argument este NULL pt execvp 
}

void signal_handler(int sig) {
    // daca semnalul primit este SIGTSTP si exista un proces activ
    if (sig == SIGTSTP && current_pid > 0) {
        kill(current_pid, SIGSTOP);     // se suspenda procesul curent
        printf("\nProcess %d suspended\n", current_pid);

       // procesul suspendat este adaugat in lista de procese de fundal
        if (background_count < MAX_BACKGROUND_PROCESSES) {
            // background_processes este definit la linia 29
            background_processes[background_count].pid = current_pid;
            snprintf(background_processes[background_count].command, MAX_COMMAND_LENGTH, "Suspended process %d", current_pid);
            background_count++;
        } else {
            printf("Maximum background process limit reached.\n");
        }

        suspended_pid = current_pid; // se stocheaza pid-ul procesului suspendat
        current_pid = -1; // se reseteaza pid-ul procesului curent
    }
}


void execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        signal(SIGTSTP, SIG_DFL); 
        if (execvp(args[0], args) < 0) {
            printf(ORANGE "Command not found: %s\n" RESET, args[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        current_pid = pid;
        waitpid(pid, NULL, WUNTRACED); 
        if (WIFSTOPPED(current_pid)) {
            printf("\nProcess %d suspended\n", current_pid);
        }
        current_pid = -1; 
    }
}


//ieisrea primei comenzi este intrarea celei de-a doua comenzi
// primul proces copil (cmd1) va scrie rezultatul executiei sale in pipe
// al doilea proces copil(cmd2) citeste din pipe si folosese datele ca intrare
// procesul parinte inchide pipe-ul si asteapta finalizarea ambelor procese copil
void execute_pipe(char *cmd1, char *cmd2) {
    // pipe_fd[0] = capatul de citire al pipe-ului
    // pipe_fd[1] = capatul de scrierea al pipe-ului
    int pipe_fd[2];

    //pipe creeaza un canal de comunicare unidirectional intre doua procese
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Primul copil: se executa cmd1
        // SIG_DFL = signal default
        signal(SIGTSTP, SIG_DFL); // restabileste comportamentul implicit al SIGTSTP
        close(pipe_fd[0]); // inchide capatul de citire al pipe-ului, procesul nu va citi din pipe
        // STDOUT_FILENO = standard output file descriptor
        dup2(pipe_fd[1], STDOUT_FILENO); //redirectioneaza ieisrea standard catre capatul de scriere al pipe-ului
        close(pipe_fd[1]); // se inchide capatul de scriere al pipe-ului

        char *args[MAX_ARGS];
        parse_command(cmd1, args); //sparge cmd1 in argumente
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Al doilea copil: executa cmd2
        signal(SIGTSTP, SIG_DFL); 
        close(pipe_fd[1]); //inchide capatul de scriere al pipe-ului
        dup2(pipe_fd[0], STDIN_FILENO); //redirectioneaza intrarea standard catre capatul de citire
        close(pipe_fd[0]); //inchide capatul de citire al pipe-ului

        char *args[MAX_ARGS];
        parse_command(cmd2, args);
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    }

    // Procesul parinte
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, WUNTRACED);
    waitpid(pid2, NULL, WUNTRACED);
}



void execute_redirection(char *command) {
    char *args[MAX_ARGS];
    char *input_file = NULL;
    char *output_file = NULL;

    char *input_redirect = strchr(command, '<');
    char *output_redirect = strchr(command, '>');

    if (input_redirect) {
        *input_redirect = '\0'; // termina comanda inainte de simbolul '<'
        input_file = strtok(input_redirect + 1, " "); // obtine numele fisierului
    }

    if (output_redirect) {
        *output_redirect = '\0';
        output_file = strtok(output_redirect + 1, " ");
    }

    parse_command(command, args); // sparge partea ramasa din command in argumente utilizabile pt executia comenzii(ls, cat etc)

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // daca este specificata redirectionarea intrarii
        if (input_file) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("open input file failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO); //redirectioneaza intrarea standard catre fd
            close(fd);
        }

        if (output_file) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open output file failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (execvp(args[0], args) < 0) {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    } else {
        // Procesul parinte
        current_pid = pid;
        waitpid(pid, NULL, WUNTRACED);
        current_pid = -1;
    }
}


// executa o singura comanda sau o comanda cu functionalitati speciale
// | Pipe uri
// Redirectionari <, >
// Executii in fundal &
// Comenzi simple (ls, pwd etc)
int execute_with_status(char *command) {
    // PIPE-URI
    char *pipe_pos = strchr(command, '|');
    if (pipe_pos != NULL) {
        *pipe_pos = '\0';
        char *cmd1 = command;
        char *cmd2 = pipe_pos + 1;
        execute_pipe(cmd1, cmd2); //linia 164
        return 0; 
    }

    // REDIRECTIONARI <, >
    if (strchr(command, '<') || strchr(command, '>')) {
        execute_redirection(command); //linia 232
        return 0;
    }

    // EXECUTII IN FUNDAL &
    int background = 0;
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '&') {
        background = 1; // marcam executia in fundal
        command[len - 1] = '\0'; // Se elimina & din comanda
        while (len > 1 && command[len - 2] == ' ') { // si spatiul de dinaintea &
            command[len - 2] = '\0';
            len--;
        }
    }

    char *args[MAX_ARGS];
    parse_command(command, args);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // in procesul copil se va executa comanda utilizatorului
        signal(SIGTSTP, SIG_DFL); 
        if (execvp(args[0], args) == -1) {
            perror("execvp error");
            exit(EXIT_FAILURE);
        }
    } else {
        // Porcesul parinte
        if (background) {
            if (background_count < MAX_BACKGROUND_PROCESSES) {
                background_processes[background_count].pid = pid;
                strncpy(background_processes[background_count].command, command, MAX_COMMAND_LENGTH);
                background_count++;
                printf("Process %d running in background: %s\n", pid, command);
            } else {
                printf("Maximum background process limit reached.\n");
            }
        } else {
            int status;
            current_pid = pid;
            waitpid(pid, &status, WUNTRACED);
            current_pid = -1;
            if (WIFEXITED(status)) { //verifica daca procesul copil s-a terminat normal
                return WEXITSTATUS(status); //returneaza codul de ieisre al procesului copil
            }
        }
    }
    return -1;
}


void print_jobs() {
    printf("Background processes:\n");
    for (int i = 0; i < background_count; i++) {
        printf("[%d] PID: %d Command: %s\n", i + 1, background_processes[i].pid, background_processes[i].command);
    }
}

void bring_to_foreground(int job_number) {
    if (job_number < 1 || job_number > background_count) {
        printf(ORANGE "Invalid job number.\n" RESET);
        return;
    }

    pid_t pid = background_processes[job_number - 1].pid;
    printf(BABY_BLUE "Bringing process %d to foreground: %s\n" RESET, pid, background_processes[job_number - 1].command);

    for (int i = job_number - 1; i < background_count - 1; i++) {
        background_processes[i] = background_processes[i + 1];
    }
    background_count--;

    
    kill(pid, SIGCONT); // trimite semnalul SIGCONT procesului pt a-l relua din starea de suspendare
    current_pid = pid; // acest proces este acum procesul in prim-plan
    // WUNTRACED = instruieste waitpid sa returneze si atunci cand procesul copil este suspendat
    waitpid(pid, NULL, WUNTRACED); // permite shell-ului sa afiseze un mesaj precum "Process 1234 suspended"

    if (WIFSTOPPED(pid)) { //verifica daca procesul a fost suspendat ( daca s-a apasat Ctrl+Z )
        printf(BABY_BLUE "\nProcess %d suspended again\n" RESET, pid);
        suspended_pid = pid; 
    }

    current_pid = -1; 
}


void kill_background_process(int job_number) {
    if (job_number < 1 || job_number > background_count) {
        printf(ORANGE "Invalid job number.\n" RESET);
        return;
    }

    // lista incepe de la 0 dar utilizatorul introduce nr de job pornind de la 1
    // de aceea job_number - 1
    pid_t pid = background_processes[job_number - 1].pid;

    // Trimite semnalul SIGKILL procesului
    // SIGKILL forteaza oprirea imediata a procesului, fara posibilitatea ca acesta sa gestioneze semnalul
    if (kill(pid, SIGKILL) == 0) {
        printf(BABY_BLUE "Process %d killed successfully.\n" RESET, pid);

        // Eliminam procesul din lista de background
        for (int i = job_number - 1; i < background_count - 1; i++) {
            background_processes[i] = background_processes[i + 1];
        }
        background_count--;
    } else {
        printf(ORANGE "Failed to kill process.\n" RESET);
    }
}







void execute_logical_commands(char *command) {
    char *and_position = strstr(command, "&&");
    char *or_position = strstr(command, "||");

    if (and_position != NULL) {
        *and_position = '\0';
        char *left_command = command;
        char *right_command = and_position + 2;


        if (execute_with_status(left_command) == 0) { //linia 292
            execute_logical_commands(right_command); //recursivitate
        }
        return;
    }

    if (or_position != NULL) {
        *or_position = '\0';
        char *left_command = command;
        char *right_command = or_position + 2;

        if (execute_with_status(left_command) != 0) {
            execute_logical_commands(right_command);
        }
        return;
    }

    execute_with_status(command);
}


void handle_fg_command() {
    if (suspended_pid == -1) {
        printf("No process to bring foreground\n");
        return;
    }

    kill(suspended_pid, SIGCONT); //il reia din starea de suspendare

    current_pid = suspended_pid;
    suspended_pid = -1;

    waitpid(current_pid, NULL, WUNTRACED); //se asteapta ca proceul reluat sa isi schimbe starea
    //( de ex sa se termine sau sa fie suspendat din nou)

    if (WIFSTOPPED(current_pid)) { // daca procesul a fost suspendat din nou
        printf("\nProcess %d suspended again\n", current_pid);
        suspended_pid = current_pid;
    }
    current_pid = -1;
}

int main() {

    welcome_message();
    
    char command[MAX_COMMAND_LENGTH];

    // SIGTSTP este un semnal trimis unui proces atunci cand se apasa Ctrl + Z
    //      spune sistemului sa suspende procesul curent, punandu-l in stare de asteptare, dar fara a-l termina
    // fct signal spune sistemului ca atunci cand procesul primeste semnalul SIGTSTP nu tb sa suspunde imediat procesul, 
    //      ci sa apeleze o fct def de utilizator
    signal(SIGTSTP, signal_handler); //linia 114 fct signal_handler

    while (1) {
        print_prompt(); //linia 43
        read_command(command); //linia 97

        // se adauga comanda curenta la history
        add_to_history(command); //linia 74

       if (strcmp(command, "exit") == 0) {
            break;
        }
        
        if (strcmp(command, "history") == 0) {
            print_history(); //linia 89
            continue;
        }
        
        if(strcmp(command, "help") == 0){
            display_help(); //linia 63
            continue;
        }
        
        if (strcmp(command, "jobs") == 0) {
            print_jobs(); //linia 365
            continue;
        }
        
        if (strncmp(command, "kill ", 5) == 0) {
            // comand + 5 = sare peste primele 5 caractere din kill+spatiu pt a putea retine numarul de dupa 
            int job_number = atoi(command + 5);
            kill_background_process(job_number); //linia 401
            continue;
        }

        if (strcmp(command, "fg") == 0) {
            // se aduce in prim-plan ultimul proces suspendat
            handle_fg_command(); //463
            continue;
        }

        if (strncmp(command, "fg ", 3) == 0) {
            int job_number = atoi(command + 3);
            bring_to_foreground(job_number); //linia 372
            continue;
        }

        // COMENZILE LOGICE
        execute_logical_commands(command); //linia 432

        
    }

    printf(BABY_PINK "Exiting our AAShell...\n" RESET);

    // eliberam memoria alocata pt history
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }

    return 0;
}
