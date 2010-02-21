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
#include "config.h"
#include "episodes.h"
#include "fonthand.h"
#include "helptext.h"
#include "keyboard.h"
#include "menus.h"
#include "nortsong.h"
#include "opentyr.h"
#include "palette.h"
#include "picload.h"
#include "setup.h"
#include "sprite.h"
#include "video.h"

char episode_name[6][31], difficulty_name[7][21], gameplay_name[5][26];

bool select_gameplay( void )
{
	JE_loadPic(2, false);
	JE_dString(JE_fontCenter(gameplay_name[0], FONT_SHAPES), 20, gameplay_name[0], FONT_SHAPES);
	
	int gameplay = 1,
	    gameplay_max = 4;
	
	bool fade_in = true;
	for (; ; )
	{
		for (int i = 1; i <= gameplay_max; i++)
		{
			JE_outTextAdjust(JE_fontCenter(gameplay_name[i], SMALL_FONT_SHAPES), i * 24 + 30, gameplay_name[i], 15, - 4 + (i == gameplay ? 2 : 0) - (i == 4 ? 4 : 0), SMALL_FONT_SHAPES, true);
		}
		JE_showVGA();
		
		if (fade_in)
		{
			fade_palette(colors, 10, 0, 255);
			fade_in = false;
		}
		
		JE_word temp = 0;
		JE_textMenuWait(&temp, false);
		
		if (newkey)
		{
			switch (lastkey_sym)
			{
			case SDLK_UP:
				gameplay--;
				if (gameplay < 1)
				{
					gameplay = gameplay_max;
				}
				JE_playSampleNum(S_CURSOR);
				break;
			case SDLK_DOWN:
				gameplay++;
				if (gameplay > gameplay_max)
				{
					gameplay = 1;
				}
				JE_playSampleNum(S_CURSOR);
				break;
				
			case SDLK_RETURN:
				if (gameplay == 4)
				{
					JE_playSampleNum(S_SPRING);
					/* TODO: NETWORK */
					fprintf(stderr, "error: networking via menu not implemented\n");
					break;
				}
				JE_playSampleNum(S_SELECT);
				fade_black(10);
				
				onePlayerAction = (gameplay == 2);
				twoPlayerMode = (gameplay == 3);
				return true;
				
			case SDLK_ESCAPE:
				JE_playSampleNum(S_SPRING);
				/* fading handled elsewhere
				fade_black(10); */
				
				return false;
				
			default:
				break;
			}
		}
	}
	
	return false;
}

bool select_episode( void )
{
	JE_loadPic(2, false);
	JE_dString(JE_fontCenter(episode_name[0], FONT_SHAPES), 20, episode_name[0], FONT_SHAPES);
	
	int episode = 1,
	    episode_max = EPISODE_MAX - 1;
	
	bool fade_in = true;
	for (; ; )
	{
		for (int i = 1; i <= episode_max; i++)
		{
			JE_outTextAdjust(20, i * 30 + 20, episode_name[i], 15, -4 + (i == episode ? 2 : 0) - (!episodeAvail[i - 1] ? 4 : 0), SMALL_FONT_SHAPES, true);
		}
		JE_showVGA();
		
		if (fade_in)
		{
			fade_palette(colors, 10, 0, 255);
			fade_in = false;
		}
		
		JE_word temp = 0;
		JE_textMenuWait(&temp, false);
		
		if (newkey)
		{
			switch (lastkey_sym)
			{
			case SDLK_UP:
				episode--;
				if (episode < 1)
				{
					episode = episode_max;
				}
				JE_playSampleNum(S_CURSOR);
				break;
			case SDLK_DOWN:
				episode++;
				if (episode > episode_max)
				{
					episode = 1;
				}
				JE_playSampleNum(S_CURSOR);
				break;
				
			case SDLK_RETURN:
				if (!episodeAvail[episode - 1])
				{
					JE_playSampleNum(S_SPRING);
					break;
				}
				JE_playSampleNum(S_SELECT);
				fade_black(10);
				
				JE_initEpisode(episode);
				initial_episode_num = episodeNum;
				return true;
				
			case SDLK_ESCAPE:
				JE_playSampleNum(S_SPRING);
				/* fading handled elsewhere
				fade_black(10); */
				
				return false;
				
			default:
				break;
			}
		}
	}
	
	return false;
}

bool select_difficulty( void )
{
	JE_loadPic(2, false);
	JE_dString(JE_fontCenter(difficulty_name[0], FONT_SHAPES), 20, difficulty_name[0], FONT_SHAPES);
	
	difficultyLevel = 2;
	int difficulty_max = 6;
	
	bool fade_in = true;
	for (; ; )
	{
		for (int i = 1; i <= difficulty_max; i++)
		{
			JE_outTextAdjust(JE_fontCenter(difficulty_name[i], SMALL_FONT_SHAPES), i * 24 + 30, difficulty_name[i], 15, -4 + (i == difficultyLevel ? 2 : 0), SMALL_FONT_SHAPES, true);
		}
		JE_showVGA();
		
		if (fade_in)
		{
			fade_palette(colors, 10, 0, 255);
			fade_in = false;
		}
		
		JE_word temp = 0;
		JE_textMenuWait(&temp, false);
		
		/*if (SDL_GetModState() & KMOD_SHIFT)
		{
			if ((difficulty_max < 4 && keysactive[SDLK_g]) ||
			    (difficulty_max == 4 && keysactive[SDLK_RIGHTBRACKET]))
			{
				difficulty_max++;
			}
		} else if (difficulty_max == 5 && keysactive[SDLK_l] && keysactive[SDLK_o] && keysactive[SDLK_r] && keysactive[SDLK_d]) {
			difficulty_max++;
		}*/
		
		if (newkey)
		{
			switch (lastkey_sym)
			{
			case SDLK_UP:
				difficultyLevel--;
				if (difficultyLevel < 1)
				{
					difficultyLevel = difficulty_max;
				}
				JE_playSampleNum(S_CURSOR);
				break;
			case SDLK_DOWN:
				difficultyLevel++;
				if (difficultyLevel > difficulty_max)
				{
					difficultyLevel = 1;
				}
				JE_playSampleNum(S_CURSOR);
				break;
				
			case SDLK_RETURN:
				JE_playSampleNum(S_SELECT);
				/* fading handled elsewhere
				fade_black(10); */
				
				if (difficultyLevel == 6)
				{
					difficultyLevel = 8;
				} else if (difficultyLevel == 5) {
					difficultyLevel = 6;
				}
				return true;
				
			case SDLK_ESCAPE:
				JE_playSampleNum(S_SPRING);
				/* fading handled elsewhere
				fade_black(10); */
				
				return false;
				
			default:
				break;
			}
		}
	}
	
	return false;
}

bool game_modes(char **code, int *engageDiff)
{
	char game_mode[20] = "Special Games/Modes";

	char *menuText[] =
	{
			"Play Destruct",
			"Arcade: STORMWIND",
			"Engage: Lord of Game"
	};

	int shipsMax = 6, maxSel = 2, shipSel = 1, sel = 0;

	bool fade_in = true, arcadeChanged = false, engageChanged = false, action = false, result = false;
	for(; ; )
	{
		//blit_sprite(VGAScreenSeg, 50, 50, OPTION_SHAPES, 35);  // message box

		JE_loadPic(2, false);
		JE_dString(JE_fontCenter(game_mode, FONT_SHAPES), 20, game_mode, FONT_SHAPES);

		if(arcadeChanged)
		{
			sprintf(menuText[1], "Arcade: %s", specialName[shipSel]);
			arcadeChanged = false;
		}
		if(engageChanged)
		{
			sprintf(menuText[2], "Engage: %s", *engageDiff == 6 ? "Suicide" : "Lord of Game");
			engageChanged = false;
		}
		for (int i = 0; i <= 2; i++)
		{
			JE_outTextAdjust(JE_fontCenter(menuText[i], SMALL_FONT_SHAPES), i * 24 + 60, menuText[i], 15, -4 + (i == sel ? 2 : 0), SMALL_FONT_SHAPES, true);
		}
		JE_showVGA();

		if (fade_in)
		{
			fade_palette(colors, 10, 0, 255);
			fade_in = false;
		}

		JE_word temp = 0;
		JE_textMenuWait(&temp, false);

		if (newkey)
		{
			switch (lastkey_sym)
			{
			case SDLK_UP:
				sel--;
				if (sel < 0)
				{
					sel = maxSel;
				}
				JE_playSampleNum(S_CURSOR);
				break;
			case SDLK_DOWN:
				sel++;
				if (sel > maxSel)
				{
					sel = 0;
				}
				JE_playSampleNum(S_CURSOR);
				break;
			case SDLK_LEFT:
				switch(sel)
				{
				case 0:
					break;
				case 1:
					shipSel--;
					if(shipSel < 0)
						shipSel = shipsMax;
					JE_playSampleNum(S_CURSOR);
					arcadeChanged = true;
					break;
				case 2:
					if(*engageDiff == 8)
						*engageDiff = 6;
					else if (*engageDiff == 6)
						*engageDiff = 8;
					engageChanged = true;
					JE_playSampleNum(S_CURSOR);
					break;
				default:
					break;
				}
				break;
			case SDLK_RIGHT:
				switch(sel)
				{
				case 0:
					break;
				case 1:
					shipSel++;
					if(shipSel > shipsMax)
						shipSel = 0;
					JE_playSampleNum(S_CURSOR);
					arcadeChanged = true;
					break;
				case 2:
					if(*engageDiff == 8)
						*engageDiff = 6;
					else if (*engageDiff == 6)
						*engageDiff = 8;
					JE_playSampleNum(S_CURSOR);
					engageChanged = true;
					break;
				default:
					break;
				}
				break;
			case SDLK_RETURN:
				JE_playSampleNum(S_SELECT);
				switch(sel)
				{
				case 0:
					sprintf(*code, "%s", specialName[7]);
					result = true;
					action = true;
					break;
				case 1:
					sprintf(*code, "%s", specialName[shipSel]);
					result = true;
					action = true;
					break;
				case 2:
					sprintf(*code, "%s", specialName[8]);
					result = true;
					action = true;
					break;
				default:
					break;
				}
				break;
			case SDLK_ESCAPE:
				JE_playSampleNum(S_SPRING);
				result = false;
				action = true;
				break;
			default:
				break;
			}
		}
		if (action == true)
			return result;
	}
}

// kate: tab-width 4; vim: set noet:
