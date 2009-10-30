/* 
 * OpenTyrian Classic: A modern cross-platform port of Tyrian
 * Copyright (C) 2007-2009  The OpenTyrian Development Team
 * hq2x, hq3x, hq4x Copyright (C) 2003 MaxSt ( maxst@hiend3d.com )
 * Scale2x, Scale3x Copyright (C) 2001, 2002, 2003, 2004 Andrea Mazzoleni
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

#include "video_scale.h"

#include "palette.h"
#include "video.h"

#include <assert.h>

void nn_32( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale );
void nn_16( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale );

void scale2x_32( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale );
void scale2x_16( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale );

int scale, scaler = 1;

const struct scaler_struct scalers[] =
{
	{ 1, nn_16,      nn_32,      "None" },
	{ 2, nn_16,      nn_32,      "2x" },
	{ 2, scale2x_16, scale2x_32, "Scale2x" }
};

void nn_32( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale )
{
	Uint8 *src = src_surface->pixels, *src_temp,
	      *dst = dst_surface->pixels, *dst_temp;
	int src_pitch = src_surface->pitch,
	    dst_pitch = dst_surface->pitch;
	const int dst_Bpp = 4;         // dst_surface->format->BytesPerPixel
	
	const int height = vga_height, // src_surface->h
	          width = vga_width;   // src_surface->w
	
	for (int y = height; y > 0; y--)
	{
		src_temp = src;
		dst_temp = dst;
		
		for (int x = width; x > 0; x--)
		{
			for (int z = scale; z > 0; z--)
			{
				*(Uint32 *)dst = rgb_palette[*src];
				dst += dst_Bpp;
			}
			src++;
		}
		
		src = src_temp + src_pitch;
		dst = dst_temp + dst_pitch;
		
		for (int z = scale; z > 1; z--)
		{
			memcpy(dst, dst_temp, dst_pitch);
			dst += dst_pitch;
		}
	}
}

void nn_16( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale )
{
	Uint8 *src = src_surface->pixels, *src_temp,
	      *dst = dst_surface->pixels + (dst_surface->h - vga_height*scale)/2*dst_surface->pitch, *dst_temp;
	int src_pitch = src_surface->pitch,
	    dst_pitch = dst_surface->pitch;
	const int dst_Bpp = 2;         // dst_surface->format->BytesPerPixel
	
	const int height = vga_height, // src_surface->h
	          width = vga_width;   // src_surface->w
	
	for (int y = height; y > 0; y--)
	{
		src_temp = src;
		dst_temp = dst;
		
		for (int x = width; x > 0; x--)
		{
			for (int z = scale; z > 0; z--)
			{
				*(Uint16 *)dst = rgb_palette[*src];
				dst += dst_Bpp;
			}
			src++;
		}
		
		src = src_temp + src_pitch;
		dst = dst_temp + dst_pitch;
		
		for (int z = scale; z > 1; z--)
		{
			memcpy(dst, dst_temp, dst_pitch);
			dst += dst_pitch;
		}
	}
}

void scale2x_32( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale )
{
	(void)scale;
	
	Uint8 *src = src_surface->pixels, *src_temp,
	      *dst = dst_surface->pixels, *dst_temp;
	int src_pitch = src_surface->pitch,
	    dst_pitch = dst_surface->pitch;
	const int dst_Bpp = 4;         // dst_surface->format->BytesPerPixel
	
	const int height = vga_height, // src_surface->h
	          width = vga_width;   // src_surface->w
	
	int prevline, nextline;
	
	Uint32 E0, E1, E2, E3, B, D, E, F, H;
	for (int y = 0; y < height; y++)
	{
		src_temp = src;
		dst_temp = dst;
		
		prevline = (y > 0) ? -src_pitch : 0;
		nextline = (y < height - 1) ? src_pitch : 0;
		
		for (int x = 0; x < width; x++)
		{
			B = rgb_palette[*(src + prevline)];
			D = rgb_palette[*(x > 0 ? src - 1 : src)];
			E = rgb_palette[*src];
			F = rgb_palette[*(x < width - 1 ? src + 1 : src)];
			H = rgb_palette[*(src + nextline)];
			
			if (B != H && D != F) {
				E0 = D == B ? D : E;
				E1 = B == F ? F : E;
				E2 = D == H ? D : E;
				E3 = H == F ? F : E;
			} else {
				E0 = E1 = E2 = E3 = E;
			}
			
			*(Uint32 *)dst = E0;
			*(Uint32 *)(dst + dst_Bpp) = E1;
			*(Uint32 *)(dst + dst_pitch) = E2;
			*(Uint32 *)(dst + dst_pitch + dst_Bpp) = E3;
			
			src++;
			dst += 2 * dst_Bpp;
		}
		
		src = src_temp + src_pitch;
		dst = dst_temp + 2 * dst_pitch;
	}
}

void scale2x_16( SDL_Surface *src_surface, SDL_Surface *dst_surface, int scale )
{
	(void)scale;
	
	Uint8 *src = src_surface->pixels, *src_temp,
	      *dst = dst_surface->pixels + (dst_surface->h - vga_height*scale)/2*dst_surface->pitch, *dst_temp;
	int src_pitch = src_surface->pitch,
	    dst_pitch = dst_surface->pitch;
	const int dst_Bpp = 2;         // dst_surface->format->BytesPerPixel
	
	const int height = vga_height, // src_surface->h
	          width = vga_width;   // src_surface->w
	
	int prevline, nextline;
	
	Uint16 E0, E1, E2, E3, B, D, E, F, H;
	for (int y = 0; y < height; y++)
	{
		src_temp = src;
		dst_temp = dst;
		
		prevline = (y > 0) ? -src_pitch : 0;
		nextline = (y < height - 1) ? src_pitch : 0;
		
		for (int x = 0; x < width; x++)
		{
			B = rgb_palette[*(src + prevline)];
			D = rgb_palette[*(x > 0 ? src - 1 : src)];
			E = rgb_palette[*src];
			F = rgb_palette[*(x < width - 1 ? src + 1 : src)];
			H = rgb_palette[*(src + nextline)];
			
			if (B != H && D != F) {
				E0 = D == B ? D : E;
				E1 = B == F ? F : E;
				E2 = D == H ? D : E;
				E3 = H == F ? F : E;
			} else {
				E0 = E1 = E2 = E3 = E;
			}
			
			*(Uint16 *)dst = E0;
			*(Uint16 *)(dst + dst_Bpp) = E1;
			*(Uint16 *)(dst + dst_pitch) = E2;
			*(Uint16 *)(dst + dst_pitch + dst_Bpp) = E3;
			
			src++;
			dst += 2 * dst_Bpp;
		}
		
		src = src_temp + src_pitch;
		dst = dst_temp + 2 * dst_pitch;
	}
}

// kate: tab-width 4; vim: set noet:
