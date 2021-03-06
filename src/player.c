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
#include "playerd.h"


struct _playerdPlayer {
    int   refcnt;
    char* name;
    int   name_n;
    playerdDevice* device;
};


playerdPlayer*
playerd_player_new ( ) {
    playerdPlayer* self = (playerdPlayer*) malloc(sizeof(playerdPlayer));
    self->refcnt = 1;
    self->name_n = 0;
    self->device = NULL;
    return self;
};


void
playerd_player_ref (playerdPlayer* self) {
    self->refcnt++;
};


void
playerd_player_unref (playerdPlayer* self) {
    if (self->refcnt-- == 0) {
        if (self->name_n)
            free(self->name);
        if (self->device)
            playerd_device_unref(self->device);
        free(self);
    }
};

