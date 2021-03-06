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


struct _playerdDevice {
    int   refcnt;
    char* name;
    int   name_n;
    char* icon;
    int   icon_n;
    playerdDevice* next;
};


playerdDevice*
playerd_device_new ( ) {
    playerdDevice* self = (playerdDevice*) malloc(sizeof(playerdDevice));
    self->refcnt = 1;
    self->name_n = 0;
    self->icon_n = 0;
    self->next = NULL;
    return self;
};


void
playerd_device_ref (playerdDevice* self) {
    self->refcnt++;
};


void
playerd_device_unref (playerdDevice* self) {
    if (self->refcnt-- == 0) {
        if (self->name_n)
            free(self->name);
        if (self->icon_n)
            free(self->icon);
        if (self->next)
            playerd_device_unref(self->next);
        free(self);
    }
};

