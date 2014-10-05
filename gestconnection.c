/***************************************************************************
 *   Copyright (C) 2008 by Luca   *
 *   mosca@PC3000   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gestconnection.h"

/* inizializzo la lista delle connessioni e
   il mutex associata ad essa 
 */
void init_connections_list(connections_p connection_list) {
    connection_list->head = NULL;
    connection_list->tail = NULL;
    pthread_mutex_init(&(connection_list->l_mutex), NULL);
}

/* crea un nuovo item connection contenente
   le informazioni sul nuovo utente e sulla socket associata.
   inizializzo anche il mutex relativo alla connessione
 */
connection_p new_connection(user_info_p userinfo, sock_info_p sockinfo) {
    connection_p connection;
    connection = (connection_p) malloc(sizeof (connection_t));
    connection->user = userinfo;
    connection->sock = sockinfo;
    pthread_mutex_init(&connection->m_connection, NULL);
    connection->prev = NULL;
    connection->next = NULL;
    return connection;

}

/* creo un nuovo item user con le informazioni sull'utente collegatosi */
user_info_p new_user(char *nick, int state) {
    user_info_p user;
    user = (user_info_p) malloc(sizeof (user_info_t));
    user->state = state;
    memset(user->nick, '\0', USERNAMELEN);
    strncpy(user->nick, nick, strlen(nick));
    return user;
}

sock_info_p new_sock_info(int sd, thread_arg_p arg) {
    sock_info_p sock;
    sock = (sock_info_p) malloc(sizeof (sock_info_t));
    sock->socket_id = sd;
    sock->arg = arg;
    return sock;
}

int add_connection(connections_p connection_list, connection_p connection1) {
    connection_p connection2;
    /*add on the structure*/
    //controllo che le connessioni non siano in chiusura

    if (connection_list->head == NULL) { /* is empty */

        connection1->next = NULL;
        connection1->prev = NULL;
        connection_list->head = connection1;
        connection_list->tail = connection1;
    } else {
        connection2 = connection_list->tail;
        pthread_mutex_lock(&connection2->m_connection); //lock last element
        connection2->next = connection1;
        connection1->prev = connection2;
        connection1->next = NULL;
        connection_list->tail = connection1;
        pthread_mutex_unlock(&connection2->m_connection); //Unlock
    }
    return EXIT_SUCCESS;
}

int close_connection(connections_p connection_list, connection_p connection) {
    if (connection->prev == NULL) {
        //The first element
        if (connection->next == NULL) {
            //the only elemetn
            pthread_mutex_lock(&connection->m_connection);
            connection_list->head = NULL;
            connection_list->tail = NULL;
            pthread_mutex_unlock(&connection->m_connection);

        } else {
            pthread_mutex_lock(&connection->m_connection);
            pthread_mutex_lock(&connection->next->m_connection);
            connection_list->head = connection->next;
            connection->next->prev = NULL;
            pthread_mutex_unlock(&connection->next->m_connection);
            pthread_mutex_unlock(&connection->m_connection);

        }
    } else {
        while (pthread_mutex_trylock(&connection->prev->m_connection) > 0)

            pthread_mutex_lock(&connection->m_connection);

        if (connection->next == NULL) {
            //last element
            connection->prev->next = connection->next;
            connection_list->tail = connection->prev;
        } else {
            pthread_mutex_lock(&connection->next->m_connection);
            connection->prev->next = connection->next;
            connection->next->prev = connection->prev;
            pthread_mutex_unlock(&connection->next->m_connection);
        }
        pthread_mutex_unlock(&connection->m_connection);
        pthread_mutex_unlock(&connection->prev->m_connection);
    }
    pthread_mutex_destroy(&connection->m_connection);
    close(connection->sock->socket_id);
    free(connection->user);
    free(connection->sock);
    free(connection);
}

void close_connections(connections_p connection_list) {
    connection_p connection;
    while (pthread_mutex_trylock(&connection_list->l_mutex) > 0) {
        fflush(stdout);
    }

    connection_p tmp;
    tmp = connection_list->head;

    while (tmp != NULL) {

        pthread_mutex_lock(&tmp->m_connection);

        close(tmp->sock->socket_id);
        pthread_mutex_unlock(&tmp->m_connection);
        pthread_mutex_destroy(&tmp->m_connection);

        tmp = tmp->next;
    }

    pthread_mutex_unlock(&connection_list->l_mutex);

    pthread_mutex_destroy(&connection_list->l_mutex);
    close(listen_socket_id);
    pthread_exit(0);
    exit(0);
}
