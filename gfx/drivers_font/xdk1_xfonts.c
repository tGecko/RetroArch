/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2015 - Daniel De Matteis
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xtl.h>
#include "../font_d3d_driver.h"
#include "../../general.h"

typedef struct
{
   d3d_video_t *d3d;
   XFONT *debug_font;
   D3DSurface *pFrontBuffer;
} xfonts_t;

static void *xfonts_init_font(void *video_data,
      const char *font_path, unsigned font_size)
{
   xfonts_t *xfonts = (xfonts_t*)calloc(1, sizeof(*xfont));

   if (!xfonts)
      return NULL;

   (void)font_path;
   (void)font_size;

   xfonts->d3d = video_data;

   XFONT_OpenDefaultFont(&xfonts->debug_font);
   xfonts->debug_font->SetBkMode(XFONT_TRANSPARENT);
   xfonts->debug_font->SetBkColor(D3DCOLOR_ARGB(100,0,0,0));
   xfonts->debug_font->SetTextHeight(14);
   xfonts->debug_font->SetTextAntialiasLevel(xfonts->debug_font->GetTextAntialiasLevel());

   return xfonts;
}

static void xfonts_free_font(void *data)
{
   xfonts_t *xfonts = (xfonts_t*)data;

   if (xfont)
      free(xfont);
   xfont = NULL;
}

static void xfonts_render_msg(void *data, const char *msg,
      const void *userdata)
{
   wchar_t str[PATH_MAX_LENGTH];
   float x, y;
   settings_t *settings = config_get_ptr();
   const struct font_params *params = (const struct font_params*)userdata;
   xfonts_t *xfonts = (xfonts_t*)data;

   if (params)
   {
      x = params->x;
      y = params->y;
   }
   else
   {
      x = settings->video.msg_pos_x;
      y = settings->video.msg_pos_y;
   }

   xfonts->d3d->dev->GetBackBuffer(-1, D3DBACKBUFFER_TYPE_MONO, &xfonts->pFrontBuffer);

   mbstowcs(str, msg, sizeof(str) / sizeof(wchar_t));
   xfonts->debug_font->TextOut(xfonts->pFrontBuffer, str, (unsigned)-1, x, y);
   xfonts->pFrontBuffer->Release();
}

d3d_font_renderer_t d3d_xdk1_font = {
   xfonts_init_font,
   xfonts_free_font,
   xfonts_render_msg,
   "XDK1 Xfonts",
   NULL,
   NULL,
   NULL,
};
