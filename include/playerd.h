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

#ifndef _PLAYERD_H
#define _PLAYERD_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _playerdDevice playerdDevice;

playerdDevice* playerd_device_new ( );
void playerd_device_ref (playerdDevice*);
void playerd_device_unref (playerdDevice*);


typedef struct _playerdPlayer playerdPlayer;

playerdPlayer* playerd_player_new ( );
void playerd_player_ref (playerdPlayer*);
void playerd_player_unref (playerdPlayer*);


#ifdef __cplusplus
}
#endif

#endif  /* _PLAYERD_H */

