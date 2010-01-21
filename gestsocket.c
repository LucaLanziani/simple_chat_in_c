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

#include "gestsocket.h"

int inizialize_socket(struct sockaddr_in* listen_socket, int port, int* socket_descriptor) {


    int optval;
    /*parameter set*/
    listen_socket->sin_family = AF_INET;
    listen_socket->sin_port = htons(port);
    listen_socket->sin_addr.s_addr = INADDR_ANY;
    memset(listen_socket->sin_zero, '\0', 8);

    /* socket creation */
    if ((*socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf(_("Error on create socket"));
        return CREATE_SOCKET_ERROR;
    }

    if (setsockopt(*socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval)) < 0) {
        printf(_("Error on setsockopt"));
        return SETSOCKOPT_ERROR;
    }

    if (bind(*socket_descriptor, (struct sockaddr *) listen_socket, sizeof (*listen_socket)) < 0) {
        printf(_("Error on bind\n"));
        return BIND_ERROR;
    }

    if (listen(*socket_descriptor, MAXCONNECTION) < 0) {
        printf(_("Error on listen"));
        return LISTEN_ERROR;
    }

    return *socket_descriptor;
}
