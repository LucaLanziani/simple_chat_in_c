#include <stdio.h>
#include <stdlib.h>  
#include "listener.h"

void laucher(void *pport) {
    thread_arg_p arg;
    struct sockaddr_in listen_socket;
    int port = (int) *(int*)pport;

    if (inizialize_socket(&listen_socket, port, &listen_socket_id) <= 0) {
        printf(_("can't open socket"));
        pthread_exit(0);
    }

    //Intialize the list of connections
    connections_p connection_list = (connections_p) calloc(1, sizeof (connections_t));
    init_connections_list(connection_list);

    printf("Server up and running on port %d\n", port);
    while (1) {
        pthread_t thread;
        arg = (thread_arg_p) malloc(sizeof (thread_arg));
        arg->connection_list = connection_list;

        if ((arg->dialog_socket_id = accept(listen_socket_id, (struct sockaddr *) & arg->dialog_socket, (socklen_t *) & arg->dialog_socket_len)) < 0) {
            printf(_("Server %d: Error %s on accept\n"), getpid(), strerror(errno));
            exit(EXIT_FAILURE);
        } else {
            /* create a thread for the client*/
            if (0 != pthread_create(&thread, NULL, (void*) & start_routine, arg)) {
                printf(_("Error on pthread_create\n"));
            }

            if (pthread_detach(thread) != 0) {
                printf(_("Error on pthread_detach\n"));
            }
        }
    }
}

void test() {
    printf("ricevuto segnale\n");
}
