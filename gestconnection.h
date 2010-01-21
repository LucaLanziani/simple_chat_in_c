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
#ifndef GESTCONNECTION_H
#define GESTCONNECTION_H
#include "util.h"
                                      
typedef struct thread_arg_s* thread_arg_p;
typedef struct thread_arg_s thread_arg;   

                                    
typedef struct sock_info_s* sock_info_p;
typedef struct sock_info_s sock_info_t;

struct sock_info_s
{ 
	int socket_id;
	thread_arg_p arg;
};

                                     
typedef struct user_info_s* user_info_p;
typedef struct user_info_s user_info_t;

struct user_info_s
{   char nick[USERNAMELEN];
    int state;
};



typedef struct connection_s* connection_p;
typedef struct connection_s  connection_t;

struct connection_s
{
   	user_info_p user;
	sock_info_p sock;
	pthread_mutex_t m_connection;
	connection_p next;
	connection_p prev;
};

                                         
typedef struct connections_s* connections_p;
typedef struct connections_s connections_t;

struct connections_s
{
	connection_p head;
	connection_p tail;
	pthread_mutex_t l_mutex;
};



struct thread_arg_s{
	struct sockaddr_in dialog_socket;
	int dialog_socket_id;
	socklen_t dialog_socket_len;
	pthread_t thread_id;
	connections_p connection_list;
};                        
#endif



void init_connections_list();
connection_p new_connection (user_info_p user, sock_info_p sock);
user_info_p new_user(char* nick, int state);
sock_info_p new_sock_info(int sd,thread_arg_p arg);
int add_connection(connections_p,connection_p connection1);
int close_connection(connections_p,connection_p connection);
void close_connections();
