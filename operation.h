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

#ifndef OPERATION_H
#define OPERATION_H
#include "gestconnection.h"
#include "util.h"
#endif

user_info_p userinfo;


void start_routine(void *arg);
connection_p login(thread_arg_p arg);
void get_connection(thread_arg_p arg,connection_p connection);
void login_message(connections_p,connection_p connection);
void logout_message(connections_p,connection_p connection);
void sendmessage_all(connections_p,connection_p connection,char *message);
void sendmessage(int dialog_socket_id, char* message);