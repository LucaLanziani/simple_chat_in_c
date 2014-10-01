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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "listener.h"
#include "chatinc.h"

int main(int argc, char *argv[]) {

    int options;
    pthread_t listener;
    int shared = 0;
    /* default listen port */
    int port = PORT;

    while ((options = getopt(argc, argv, "hp:")) != EOF) {
        switch (options) {
            case 'h':
                print_help(argv[0]);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            default:
                //help(argv[0]);
                break;
        }
    }

    if (0 != pthread_create(&listener, NULL, (void*) & laucher, NULL)) {
        printf(_("Error on pthread_create\n"));
    }
    
    pthread_join(listener, NULL);
    return EXIT_SUCCESS;
}

void print_help(char *name) {
    printf(_("Usage: %s [OPTION]\n"), name);
    printf(_("Create a server for a chat\n"));
    printf(_("Option List:\n"));
    printf(_("	p NUM: set a NUM port instead of default\n"));
    printf(_("without any port specified it will listen on port %d\n"), PORT);
    exit(EXIT_FAILURE);
}
