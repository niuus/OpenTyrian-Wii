#include "joystick.h"
#include "opentyr.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include <wiiuse/wpad.h>

//#include "backgrnd.h"
#include "config.h"
//#include "editship.h"
//#include "episodes.h"
#include "file.h"
//#include "fonthand.h"
//#include "helptext.h"
//#include "joystick.h"
#include "keyboard.h"
//#include "lds_play.h"
//#include "loudness.h"
//#include "menus.h"
//#include "mtrand.h"
//#include "network.h"
#include "nortsong.h"
//#include "nortvars.h"
//#include "palette.h"
//#include "params.h"
//#include "pcxmast.h"
//#include "picload.h"
#include "popkey.h"
//#include "setup.h"
//#include "shpmast.h"
//#include "sndmast.h"
//#include "sprite.h"
#include "varz.h"
//#include "vga256d.h"
#include "video.h"

#include <ctype.h>

SDL_Surface *cursorL, *cursorR, *alphaboardSurf, *numboardSurf;

SDL_Rect dispRect, boardRect, LcursorRect, RcursorRect;

SDL_Color cursorTrans;

int board = 0;

char keytemp[21];

int rowmax[4] =
{
	9,
	9,
	9,
	3
};

const struct _joystickButtons wiimoteButtons[] = {
		{"W-A"},
		{"W-B"},
		{"W-1"},
		{"W-2"},
		{"W-MINUS"},
		{"W-PLUS"},
		{"W-HOME"},
		{"N-Z"},
		{"N-C"},
		{"C-a"},
		{"C-b"},
		{"C-x"},
		{"C-y"},
		{"C-L"},
		{"C-R"},
		{"C-ZL"},
		{"C-ZR"},
		{"C-MINUS"},
		{"C-PLUS"},
		{"C-HOME"}
};

const struct _joystickButtons gcpadButtons[] = {
		{"A"},
		{"B"},
		{"X"},
		{"Y"},
		{"Z"},
		{"R"},
		{"L"},
		{"START"}
};

int keyTableSize = 57;

const struct keyTable keys[] =
{
		{ SDLK_SPACE, 		' ' },
		{ SDLK_MINUS, 		'-' },
		{ SDLK_PERIOD, 		'.' },
		{ SDLK_COMMA, 		',' },
		{ SDLK_COLON, 		':' },
		{ SDLK_EXCLAIM, 	'!' },
		{ SDLK_QUESTION, 	'?' },
		{ SDLK_HASH, 		'#' },
		{ SDLK_AT, 			'@' },
		{ SDLK_DOLLAR, 		'$' },
		{ SDLK_ASTERISK, 	'*' },
		{ SDLK_LEFTPAREN, 	'(' },
		{ SDLK_RIGHTPAREN, 	')' },
		{ SDLK_SLASH, 		'/' },
		{ SDLK_EQUALS, 		'=' },
		{ SDLK_PLUS, 		'+' },
		{ SDLK_LESS, 		'<' },
		{ SDLK_GREATER, 	'>' },
		{ SDLK_SEMICOLON, 	';' },
		{ SDLK_QUOTEDBL, 	'"' },
		{ SDLK_QUOTE, 		'\''},
		{ SDLK_WORLD_95,	'%'	},
		{ SDLK_a, 			'A' },
		{ SDLK_b, 			'B' },
		{ SDLK_c, 			'C' },
		{ SDLK_d, 			'D' },
		{ SDLK_e, 			'E' },
		{ SDLK_f, 			'F' },
		{ SDLK_g, 			'G' },
		{ SDLK_h, 			'H' },
		{ SDLK_i, 			'I' },
		{ SDLK_j, 			'J' },
		{ SDLK_k, 			'K' },
		{ SDLK_l, 			'L' },
		{ SDLK_m, 			'M' },
		{ SDLK_n, 			'N' },
		{ SDLK_o, 			'O' },
		{ SDLK_p, 			'P' },
		{ SDLK_q, 			'Q' },
		{ SDLK_r, 			'R' },
		{ SDLK_s, 			'S' },
		{ SDLK_t, 			'T' },
		{ SDLK_u, 			'U' },
		{ SDLK_v, 			'V' },
		{ SDLK_w, 			'W' },
		{ SDLK_x, 			'X' },
		{ SDLK_y, 			'Y' },
		{ SDLK_z, 			'Z' },
		{ SDLK_0, 			'0' },
		{ SDLK_1, 			'1' },
		{ SDLK_2, 			'2' },
		{ SDLK_3, 			'3' },
		{ SDLK_4, 			'4' },
		{ SDLK_5, 			'5' },
		{ SDLK_6, 			'6' },
		{ SDLK_7, 			'7' },
		{ SDLK_8, 			'8' },
		{ SDLK_9, 			'9' }
};

int boardStarts[4][2] =
{
		{ 126, 155 },
		{ 126, 187 },
		{ 126, 219 },
		{ 126, 251 }
};

const char *joystick_string_to_name( const char *value, int joystickIndex )
{
	char temp[8];
	static char name[30];
	int joystickType = joystickIndex < 4 ? JOYSTICK_WIIMOTE : JOYSTICK_GCPAD;

	if(strstr(value, "AX") != NULL)
	{
		if(strstr(value, "1+"))
		{
			sprintf(name, "JOY-R");
		}
		else if(strstr(value, "1-"))
		{
			sprintf(name, "JOY-L");
		}
		else if(strstr(value, "2+"))
		{
			sprintf(name, "JOY-D");
		}
		else if(strstr(value, "2-"))
		{
			sprintf(name, "JOY-U");
		}
		else if(strstr(value, "3+"))
		{
			sprintf(name, "RJOY-R");
		}
		else if(strstr(value, "3-"))
		{
			sprintf(name, "RJOY-L");
		}
		else if(strstr(value, "4+"))
		{
			sprintf(name, "RJOY-D");
		}
		else if(strstr(value, "4-"))
		{
			sprintf(name, "RJOY-U");
		}
	}
	else if(strstr(value, "BTN") != NULL)
	{
		if (joystickType == JOYSTICK_WIIMOTE)
		{
			for (int i = 20; i >= 1; i--)
			{
				sprintf(temp, "%d", i);
				if(strstr(value, temp) != NULL)
				{
					sprintf(name, "%s", wiimoteButtons[i-1].name);
					i = 0;
				}
			}
		}
		else if(joystickType == JOYSTICK_GCPAD)
		{
			for (int i = 8; i >= 1; i--)
			{
				sprintf(temp, "%d", i);
				if(strstr(value, temp) != NULL)
				{
					sprintf(name, "%s", gcpadButtons[i-1].name);
					i = 0;
				}
			}
		}
	}
	else if(strstr(value, "H") != NULL)
	{
		WPADData *joystick = WPAD_Data(joystickIndex);
		if(joystickType == JOYSTICK_WIIMOTE && (joystick->exp.type == WPAD_EXP_NONE))
		{
			if(strstr(value, "Y-"))
			{
				sprintf(name, "DPAD-R");
			}
			else if(strstr(value, "Y+"))
			{
				sprintf(name, "DPAD-L");
			}
			else if(strstr(value, "X-"))
			{
				sprintf(name, "DPAD-D");
			}
			else if(strstr(value, "X+"))
			{
				sprintf(name, "DPAD-U");
			}
		}
		else
		{
			if(strstr(value, "Y-"))
			{
				sprintf(name, "DPAD-U");
			}
			else if(strstr(value, "Y+"))
			{
				sprintf(name, "DPAD-D");
			}
			else if(strstr(value, "X-"))
			{
				sprintf(name, "DPAD-L");
			}
			else if(strstr(value, "X+"))
			{
				sprintf(name, "DPAD-R");
			}
		}
	}
	return name;
}

int row = 3, key = 2, lastrow = 0;

void init_popkey()
{
	char dir[40];
	sprintf(dir, "%s/popkey/alphaboard.png", data_dir());
	alphaboardSurf = IMG_Load(dir);
	sprintf(dir, "%s/popkey/numboard.png", data_dir());
	numboardSurf = IMG_Load(dir);
	sprintf(dir, "%s/popkey/leftcursor.png", data_dir());
	cursorL = IMG_Load(dir);
	sprintf(dir, "%s/popkey/rightcursor.png", data_dir());
	cursorR = IMG_Load(dir);

	dispRect.w = DISPLAY_WIDTH;
	dispRect.h = DISPLAY_HEIGHT;
}

int row3x[5] =
{
		0,
		64,
		200,
		240,
		320
};

void popkey(int dx, int dy)
{
	SDL_Surface *boardSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, BOARD_WIDTH, BOARD_HEIGHT, 16, 0, 0, 0, 0);

	dispRect.x = dx;
	dispRect.y = dy;

	if (row < 3)
	{
		LcursorRect.x = boardStarts[row][0] + (key * 32);
		RcursorRect.x = boardStarts[row][0] + (key * 32) + 24;
	}
	else if (row == 3)
	{
		LcursorRect.x = boardStarts[row][0] + row3x[key];
		RcursorRect.x = boardStarts[row][0] + row3x[key] + (row3x[key+1] - row3x[key] - 10);
	}
	if (board == 1 && row == 1 &&(key == 8 || key == 9))
	{
		LcursorRect.x = boardStarts[row][0] + (8 * 32);
		RcursorRect.x = boardStarts[row][0] + (9 * 32) + 24;
	}

	LcursorRect.y = boardStarts[row][1];
	RcursorRect.y = boardStarts[row][1];

	if (board == 0) // Alpha keyboard
	{
		SDL_BlitSurface(alphaboardSurf, NULL, boardSurf, NULL);
	}
	else if (board == 1) // Numeric/symbolic keyboard
	{
		SDL_BlitSurface(numboardSurf, NULL, boardSurf, NULL);
	}

	SDL_BlitSurface(boardSurf, NULL, display_surface, &dispRect);
	SDL_BlitSurface(cursorL, NULL, display_surface, &LcursorRect);
	SDL_BlitSurface(cursorR, NULL, display_surface, &RcursorRect);

	SDL_Flip(display_surface);

	SDL_FreeSurface(boardSurf);
}

void runPopkey( SDLKey lastkey, int method, char *stemp, bool *quit, int slot, bool *cancel )
{
	int maxChar;
	char tempChar;
	if (method == 0)
	{
		maxChar = 13;
	}
	else
	{
		maxChar = 15;
	}
	switch(lastkey_sym)
	{
	case SDLK_RIGHT:
		lastrow = row;
		if (board == 1 && row == 1 && (key == 8))
		{
			break;
		}
		else if (row == 2 && key == 8)
		{
			key = 1;
		}
		else if (row == 3 && key == 3)
		{
			key = 0;
		}
		else if (key < rowmax[row])
			key++;
		else
			key = 0;
		break;
	case SDLK_UP:
		lastrow = row;
		if (row == 3 && (key == 0 || key == 9))
		{
			row = 1;
			lastrow = 2;
		}
		else if (row > 0)
		{
			row--;
		}
		else
		{
			row = 3;
		}
		if (row == 2 && lastrow == 3)
		{
			switch (key)
			{
			case 0:
				break;
			case 1:
				key = 2;
				break;
			case 2:
				key = 6;
				break;
			case 3:
				key = 7;
				break;
			}
		}
		else if (row == 3 && lastrow == 0)
		{
			switch (key)
			{
			case 0:
			case 1:
				key = 0;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				key = 1;
				break;
			case 6:
				key = 2;
				break;
			case 7:
			case 8:
			case 9:
				key = 3;
				break;
			}
		}
		break;
	case SDLK_DOWN:
		lastrow = row;
		if (row == 1 && (key == 0 || key == 9))
		{
			row = 3;
			lastrow = 2;
		}
		else if (row < 3)
		{
			row++;
		}
		else
		{
			row = 0;
		}
		if (row == 3 && lastrow == 2)
		{
			switch (key)
			{
			case 0:
			case 1:
				key = 0;
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				key = 1;
				break;
			case 6:
				key = 2;
				break;
			case 7:
			case 8:
			case 9:
				key = 3;
				break;
			}
		}
		else if (row == 0 && lastrow == 3)
		{
			switch (key)
			{
			case 0:
				break;
			case 1:
				key = 2;
				break;
			case 2:
				key = 6;
				break;
			case 3:
				key = 7;
				break;
			}
		}
		break;
	case SDLK_LEFT:
		lastrow = row;
		if (board == 1 && row == 1 && (key == 8 || key == 9))
		{
			key = 7;
		}
		else if (row == 2 && key == 1)
		{
			key = 8;
		}
		else if (row == 3 && key == 0)
		{
			key = 3;
		}
		else if (key > 0)
			key--;
		else key = 9;
		break;
	case SDLK_BACKSPACE:
	case SDLK_DELETE:
		if (temp > 0)
		{
			temp--;
			tempChar = ' ';
			JE_playSampleNum(S_CURSOR);
		}
		break;
	case SDLK_ESCAPE:
		*quit = true;
		if(method == OSK_HIGHSCORE)
		{
			*cancel = true;
		}
		JE_playSampleNum(S_SPRING);
		break;
	case SDLK_RETURN:if(row == 3)
		{
			if (key == 0)
			{
				board ^= 1;
				break;
			}
			else if (key == 1)
			{
				stemp[temp] = ' ';
				temp++;
				break;
			}
			else if (key == 2)
			{
				*quit = true;
				if (method == OSK_SAVEMENU)
				{
					JE_saveGame(slot, stemp);
				}
				JE_playSampleNum(S_SELECT);
				board = 0;
				key = 2;
				break;
			}
			else if (key == 3)
			{
				*quit = true;
				if (method == OSK_HIGHSCORE)
				{
					*cancel = true;
				}
				JE_playSampleNum(S_SPRING);
				board = 0;
				key = 2;
				break;
			}
			break;
		}
		else if (row == 1)
		{
			if (board == 0 && key == 9)
			{
				if (temp > 0)
				{
					temp--;
					stemp[temp] = ' ';
					JE_playSampleNum(S_CURSOR);
				}
				break;
			}
			else if (board == 1 && (key == 8 || key == 9))
			{
				if (temp > 0)
				{
					temp--;
					stemp[temp] = ' ';
					JE_playSampleNum(S_CURSOR);
				}
				break;
			}
		}
		getkey();
		JE_playSampleNum(S_CURSOR);
	default:
		for (int i = 0; i <= keyTableSize; i++)
		{
			if (keys[i].sym == lastkey_sym)
			{
				if (temp >= maxChar)
				{
					temp = maxChar;
					JE_playSampleNum(S_SPRING);
					break;
				}
				JE_playSampleNum(S_CURSOR);
				stemp[temp] = keys[i].name;
				temp++;
			}
		}
		break;
	}
}

void getkey()
{
	switch (board)
	{
	case 0:
		switch (row)
		{
		case 0:
			switch (key)
			{
			case 0:
				lastkey_sym = SDLK_q;
				break;
			case 1:
				lastkey_sym = SDLK_w;
				break;
			case 2:
				lastkey_sym = SDLK_e;
				break;
			case 3:
				lastkey_sym = SDLK_r;
				break;
			case 4:
				lastkey_sym = SDLK_t;
				break;
			case 5:
				lastkey_sym = SDLK_y;
				break;
			case 6:
				lastkey_sym = SDLK_u;
				break;
			case 7:
				lastkey_sym = SDLK_i;
				break;
			case 8:
				lastkey_sym = SDLK_o;
				break;
			case 9:
				lastkey_sym = SDLK_p;
				break;
			}
			break;
		case 1:
			switch (key)
			{
			case 0:
				lastkey_sym = SDLK_a;
				break;
			case 1:
				lastkey_sym = SDLK_s;
				break;
			case 2:
				lastkey_sym = SDLK_d;
				break;
			case 3:
				lastkey_sym = SDLK_f;
				break;
			case 4:
				lastkey_sym = SDLK_g;
				break;
			case 5:
				lastkey_sym = SDLK_h;
				break;
			case 6:
				lastkey_sym = SDLK_j;
				break;
			case 7:
				lastkey_sym = SDLK_k;
				break;
			case 8:
				lastkey_sym = SDLK_l;
				break;
			}
			break;
		case 2:
			switch (key)
			{
			case 0:
				break;
			case 1:
				lastkey_sym = SDLK_z;
				break;
			case 2:
				lastkey_sym = SDLK_x;
				break;
			case 3:
				lastkey_sym = SDLK_c;
				break;
			case 4:
				lastkey_sym = SDLK_v;
				break;
			case 5:
				lastkey_sym = SDLK_b;
				break;
			case 6:
				lastkey_sym = SDLK_n;
				break;
			case 7:
				lastkey_sym = SDLK_m;
				break;
			case 8:
				lastkey_sym = SDLK_PERIOD;
				break;
			case 9:
				break;
			}
			break;
		case 3:
			break;
		}
		break;
	case 1:
		switch (row)
		{
		case 0:
			switch (key)
			{
			case 0:
				lastkey_sym = SDLK_1;
				break;
			case 1:
				lastkey_sym = SDLK_2;
				break;
			case 2:
				lastkey_sym = SDLK_3;
				break;
			case 3:
				lastkey_sym = SDLK_4;
				break;
			case 4:
				lastkey_sym = SDLK_5;
				break;
			case 5:
				lastkey_sym = SDLK_6;
				break;
			case 6:
				lastkey_sym = SDLK_7;
				break;
			case 7:
				lastkey_sym = SDLK_8;
				break;
			case 8:
				lastkey_sym = SDLK_9;
				break;
			case 9:
				lastkey_sym = SDLK_0;
				break;
			}
			break;
		case 1:
			switch (key)
			{
			case 0:
				lastkey_sym = SDLK_EXCLAIM;
				break;
			case 1:
				lastkey_sym = SDLK_QUESTION;
				break;
			case 2:
				lastkey_sym = SDLK_HASH;
				break;
			case 3:
				lastkey_sym = SDLK_DOLLAR;
				break;
			case 4:
				lastkey_sym = SDLK_WORLD_95;
				break;
			case 5:
				lastkey_sym = SDLK_ASTERISK;
				break;
			case 6:
				lastkey_sym = SDLK_LEFTPAREN;
				break;
			case 7:
				lastkey_sym = SDLK_RIGHTPAREN;
				break;
			case 8:
			case 9:
				break;
			}
			break;
		case 2:
			switch (key)
			{
			case 0:
				break;
			case 1:
				lastkey_sym = SDLK_COMMA;
				break;
			case 2:
				lastkey_sym = SDLK_COLON;
				break;
			case 3:
				lastkey_sym = SDLK_SEMICOLON;
				break;
			case 4:
				lastkey_sym = SDLK_SLASH;
				break;
			case 5:
				lastkey_sym = SDLK_QUOTE;
				break;
			case 6:
				lastkey_sym = SDLK_QUOTEDBL;
				break;
			case 7:
				lastkey_sym = SDLK_PLUS;
				break;
			case 8:
				lastkey_sym = SDLK_MINUS;
				break;
			case 9:
				break;
			}
			break;
		case 3:
			break;
		}
	}
}

void popshadow( SDL_Surface *screen)
{
	SDL_BlitSurface(alphaboardSurf, NULL, screen, &dispRect);
}

void close_popkey()
{
	SDL_FreeSurface(alphaboardSurf);
	SDL_FreeSurface(numboardSurf);
	SDL_FreeSurface(cursorL);
	SDL_FreeSurface(cursorR);
}
