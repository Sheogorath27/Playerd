/*
 *  playerd
 *  Copyright (C) 2014,2015 Copyleft Games Group
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program; if not, see http://www.gnu.org/licenses
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "playerd.h"

void start(){
    printf("Started\n");
    //do something here
}

void stop() {
    printf("Stopped\n");
    //do something here
}

int main(int argc, char** argv) {
    if (argc >= 2) {
        printf("Running playerd with:%s\n", argv[1]);

        if (strcmp(argv[1], "--start") == 0) {
            printf("Starting the service...\n");
            start();
        }

        if (strcmp(argv[1], "--stop") == 0) {
            printf("Stopping the service...\n");
            stop();
        }
    }
    return 0;
}
