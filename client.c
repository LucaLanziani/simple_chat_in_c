#include <stdio.h>         /* per usare input-output */
#include <sys/socket.h>    /* per usare socket */
#include <stdlib.h>        /* per usare exit() */
#include <errno.h>         /* gestione degli errori */
#include <netdb.h>         /* per gethostbyname() */
#include <string.h>        /* per memcpy() */
#include <signal.h>	       /* per usare signal */
#include <unistd.h>

#include "util.h"

void close_connection(int sock) {
    close(sock);
    exit(0);
}

void sock_send(int sock, char msg[USERNAMELEN]) {
    if (send(sock, msg, strlen(msg), 0) <=0) {
        fprintf(stderr, "Error %s during write\n", strerror(errno));
        close(sock);
        exit(0);
    }
}

void sock_recv(sock,)

int setusername(int sock, char *nick) {
    char username[USERNAMELEN];
    char result[256];
    memset(username, '\0', USERNAMELEN);
    memset(result, '\0', 256);    
    
    
    strncpy(username, nick, USERNAMELEN-1);
    sock_send(sock, username)

    if (recv(sock, result, 255, 0) <= 0) {
        fprintf(stderr, "Error %s during read\n", strerror(errno));
        fflush(stdout);
        close(sock);
        exit(0);
    }
    while (atoi(result) != 100) {
        printf("%s\n",result);
        printf("Please insert the nickname: ");
        fflush(stdout);
        memset(username, '\0', USERNAMELEN);
        fgets(username, USERNAMELEN-1, stdin);
        fflush(stdin);
        if (send(sock, username, strlen(username), 0) <= 0) {
            fprintf(stderr, "Error %s during write\n", strerror(errno));
            close(sock);
            exit(0);
        }
        memset(result, '\0', MESSAGELEN);
        if (recv(sock, result, sizeof(result)-1, 0) <= 0) {
            fprintf(stderr, "Error %s during read\n", strerror(errno));
            fflush(stdout);
            close(sock);
            exit(0);
        }
    }
    memset(result, '\0', 256);
    if (recv(sock, result, 255, 0) <= 0) {
        fprintf(stderr, "Error %s during read\n", strerror(errno));
        fflush(stdout);
        close(sock);
        exit(0);
    }
    printf("%s\n", result);
    return 0;

}

void ReadingProcess(int sock, int pid) {
    message_t result;
    while (1) {
        memset(result, '\0', MESSAGELEN);
        if (recv(sock, result, sizeof (result) - 1, 0) <= 0) {
            fprintf(stderr, "Error %s during read\n", strerror(errno));
            fflush(stdout);
            if (pid != 0) {
                //printf("Sto per uccidere il processo con PID %d\n",pid);
                fflush(stdout);
                return;
            }
        }
        printf("%s", result);
        fflush(stdout);
    }
}

void WritingProcess(int sock) {
    int i;
    int h;
    message_t input;
    i = 0;
    while (1) {

        memset(input, '\0', MESSAGELEN);
        while (fgets(input, MESSAGELEN - 1, stdin) == NULL) {
            memset(input, '\0', MESSAGELEN);
        }

        if (strncmp(EXITSTRING, input, strlen(EXITSTRING)) == 0) {
            printf("Terminating");
            return;
        }

        if (send(sock, input, strlen(input), 0) <= 0) {
            fprintf(stderr, "Error %s during write\n", strerror(errno));
        }
        i++;
    }
}

main(int argc, char** argv) {
    char nick[USERNAMELEN];

    //signal(SIGPIPE,SIG_IGN);
    signal(SIGINT, close_connection);
    signal(SIGQUIT, close_connection);
    signal(SIGTSTP, close_connection);

    /* descrittore della socket */
    struct sockaddr_in server;
    struct hostent *hp;
    char input[256];
    int pid, sock;


    if (argc != 4) {
        printf("use: %s <Nick> <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hp = gethostbyname(argv[2]);
    if (hp == NULL) {
        fprintf(stderr, "%s: %s unknown host.\n", argv[0], argv[2]);
        exit(EXIT_FAILURE);
    }


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "error  %s on socket creation\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* inizializzazione della struttura server */
    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[3]));

    /* connessione */
    if (connect(sock, (struct sockaddr *) & server, sizeof (server)) < 0) {
        fprintf(stderr, "error %s on connection\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    strncpy(nick, argv[1], USERNAMELEN-1);
    if (setusername(sock, nick) == 0) {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        if (pid == 0) {
            pid = getppid();
            WritingProcess(sock);
            kill(pid, SIGINT);
            exit(0);
        } else {
            ReadingProcess(sock, pid);
            kill(pid, SIGINT);

            exit(0);
        }
    }

    close(sock);
    printf("socket closed\n");
    exit(0);
}