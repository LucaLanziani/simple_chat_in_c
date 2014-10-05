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

#include "operation.h"

void start_routine(void* paramiter) {
    thread_arg_p arg = (thread_arg_p) paramiter;
    get_connection(arg, login(arg));
}

connection_p login(thread_arg_p arg) {
    char *UserName;
    int i;
    UserName = (char *) malloc(USERNAMELEN * sizeof (char));

    memset(UserName, '\0', USERNAMELEN);
    do {
        i = recv(arg->dialog_socket_id, UserName, USERNAMELEN, 0);
        if (i == 0) {
            pthread_exit(0);
        }
        if (i < 3) {
            sendmessage(arg->dialog_socket_id, "Nick name lenght should be at least 3");
        }
    } while (i < 3);
    sendmessage(arg->dialog_socket_id, "100");

    arg->thread_id = pthread_self();
    sock_info_p sockinfo = new_sock_info(arg->dialog_socket_id, arg);
    userinfo = new_user(UserName, 0);

    connection_p connection_info = new_connection(userinfo, sockinfo);

    add_connection(arg->connection_list, connection_info);
    login_message(arg->connection_list, connection_info);

    return connection_info;
}

void get_connection(thread_arg_p arg, connection_p connection) {
    message_t message_tmp, message;
    while (1) {
        memset(message, '\0', MESSAGELEN);
        memset(message_tmp, '\0', MESSAGELEN);
        if (recv(arg->dialog_socket_id, message_tmp, MESSAGELEN - strlen(connection->user->nick), 0) <= 0) {
            logout_message(arg->connection_list, connection);
            close_connection(arg->connection_list, connection);
            pthread_exit(0);
        } else {
            snprintf(message, MESSAGELEN, "%s> %s", connection->user->nick, message_tmp);
            sendmessage_all(arg->connection_list, connection, message);
        }
    }
}

void login_message(connections_p connection_list, connection_p connection) {
    message_t loginmess, message;
    memset(loginmess, '\0', sizeof (loginmess));
    snprintf(message, MESSAGELEN, "%s> %s", connection->user->nick, " logged in\n");
    snprintf(loginmess, MESSAGELEN, "%s> %s", connection->user->nick, " logged in");
    printf(_("%s\n"), loginmess);
    sendmessage_all(connection_list, connection, message);
}

void logout_message(connections_p connection_list, connection_p connection) {
    message_t logoutmess, message;
    memset(logoutmess, '\0', sizeof (logoutmess));
    snprintf(logoutmess, sizeof (logoutmess), "%s", connection->user->nick);

    snprintf(message, MESSAGELEN, "%s> %s", connection->user->nick, " logged out\n");
    snprintf(logoutmess, MESSAGELEN, "%s> %s", connection->user->nick, " logged out");
    printf(_("%s\n"), logoutmess);
    sendmessage_all(connection_list, connection, message);
}

void sendmessage(int dialog_socket_id, char* message) {
    message_t mymsg;
    send(dialog_socket_id, message, strlen(message), 0);
}

void sendmessage_all(connections_p connection_list, connection_p connection, char* message) {
    message_t mymsg;
    connection_p connection_acc;
    connection_p connection_prec;
    connection_acc = connection_list->head;
    connection_prec = NULL;
    /* Iterate on connections */
    while (connection_acc != NULL) {
        printf("Sending message to %s\n", connection_acc->user->nick);
        /* locking the mutex of the current connection */
        pthread_mutex_lock(&connection_acc->m_connection);
        /* unlocking the mutex of previous connection */
        if (connection_prec != NULL) {
            pthread_mutex_unlock(&connection_prec->m_connection);
        }
        /* send message on current connection */
        send(connection_acc->sock->socket_id, message, strlen(message), 0);
        connection_prec = connection_acc;
        /* next connection */
        connection_acc = connection_acc->next;
    }
    if (connection_prec != NULL) {
        pthread_mutex_unlock(&connection_prec->m_connection);
    }
}