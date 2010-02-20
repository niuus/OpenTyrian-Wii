/*
 * OpenTyrian Classic: A modern cross-platform port of Tyrian
 * Copyright (C) 2007-2009  The OpenTyrian Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef POPKEY_H
#define POPKEY_H

#include "opentyr.h"

#include "SDL.h"

#define SWITCH_WIDTH 64
#define SPACE_WIDTH 136
#define OK_WIDTH 40
#define CANCEL_WIDTH 80
#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define CURSORHW 32
#define BOARD_WIDTH 320
#define BOARD_HEIGHT 128
#define BOARD_SCALE 1
#define OSK_SAVEMENU 0
#define OSK_HIGHSCORE 1

extern int row, rowmax[4], key, keyTableSize, lastrow, board;

const struct keyTable {
        SDLKey sym;
        char name;
} keys[58];

const struct _joystickButtons {
	char name[9];
} wiimoteButtons[20], gcpadButtons[8];

const char *joystick_string_to_name( const char *value, int joystickIndex );

void init_popkey (); // Initialize popup keyboard system
void runPopkey( SDLKey lastkey, int method, char *stemp, bool *quit, int slot, bool *cancel );
void popkey (int dx, int dy); // Display and run keyboard
void getkey();
void popshadow ( SDL_Surface *screen );
void close_popkey (); // Wrap up keyboard systems, free surfaces, etc.

#endif /* POPKEY_H */
