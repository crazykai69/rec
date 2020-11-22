/*
Copyright (c) 2012, Broadcom Europe Ltd
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr

#include "bcm_host.h"
#include "vgfont.h"
#include <time.h>

int getTemp(char *tempVal, char *freezerStat, uint32_t *freezerStatColor, char *co2Val);

void render_subtitle(GRAPHICS_RESOURCE_HANDLE img, const char *text, const uint32_t textColor, const uint32_t rightJustify, const uint32_t text_size, const uint32_t y_offset)
{
  uint32_t text_length = strlen(text);
  uint32_t width = 0, height = 0;
  uint32_t left = 0;
  uint32_t img_w, img_h;

  uint cr = 0xff;
  uint cg = 0xff;
  uint cb = 0xff;

  if (textColor == 1)
  {
    cr = 0;
    cb = 0;
  }
  if (textColor == 2)
  {
    cg = 0;
    cb = 0;
  }

  graphics_get_resource_size(img, &img_w, &img_h);

  graphics_resource_text_dimensions_ext(img, text, text_length, &width, &height, text_size);

  if (rightJustify)
    left = img_w - width - 40;
  else
    left = 40;

  if (width)
  {
    graphics_resource_render_text_ext(img, left, y_offset,
                                      GRAPHICS_RESOURCE_WIDTH,
                                      GRAPHICS_RESOURCE_HEIGHT,
                                      GRAPHICS_RGBA32(cb, cg, cr, 0xff), /* fg BGRA */
                                      GRAPHICS_RGBA32(0, 0, 0, 127),     /* bg */
                                      text, text_length, text_size);
  }
}

int main(void)
{
  GRAPHICS_RESOURCE_HANDLE img;
  uint32_t width, height;
  int LAYER = 1;

  bcm_host_init();

  int s;

  s = gx_graphics_init(".");
  assert(s == 0);

  s = graphics_get_display_size(0, &width, &height);
  assert(s == 0);

  s = gx_create_window(0, width, height, GRAPHICS_RESOURCE_RGBA32, &img);
  assert(s == 0);

  // transparent before display to avoid screen flash
  graphics_resource_fill(img, 0, 0, width, height, GRAPHICS_RGBA32(0, 0, 0, 0x00));

  graphics_display_resource(img, 0, LAYER, 0, 0, GRAPHICS_RESOURCE_WIDTH, GRAPHICS_RESOURCE_HEIGHT, VC_DISPMAN_ROT0, 1);

  uint32_t text_size = 70;
  char timeOld[25];

  char tempVal[25];
  memset(tempVal, 0, 25);

  char freezerStat[25];
  memset(freezerStat, 0, 25);

  //  char co2Val[25];
  //  memset(co2Val,0,25);

  //  uint32_t freezerStatCol = 0;

  //  getTemp(tempVal,freezerStat,&freezerStatCol,co2Val);

  while (1)
  {

    time_t timer;
    char text[25];
    char date[25];
    memset(text, 0, 25);
    memset(date, 0, 25);
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(text, 10, "%H:%M", tm_info);
    strftime(date, 25, "%d.%m.%Y", tm_info);

    if (strcmp(timeOld, text) != 0)
    {
      graphics_resource_fill(img, 0, 0, width, height, GRAPHICS_RGBA32(0, 0, 0, 0x00));

      //      render_subtitle(img, *text, textColor, rightJustify, text_size, y_offset)

      render_subtitle(img, text, 0, 1, 70, height - 70 - 20);    // Time
      render_subtitle(img, tempVal, 0, 0, 70, height - 70 - 20); // Temperature
      // render_subtitle(img, date,        0, 0, text_size, height-text_size-20);                  // Date
      // render_subtitle(img, co2Val,      0, 0, text_size, height-(text_size*2)-40);              // CO2
      // render_subtitle(img, freezerStat, freezerStatCol, 0, text_size, height-(text_size*3)-60); // Freezer

      graphics_update_displayed_resource(img, 0, 0, 0, 0);

      getTemp(tempVal, freezerStat, &freezerStatCol, co2Val);

      strcpy(timeOld, text);
    }
  }

  graphics_display_resource(img, 0, LAYER, 0, 0, GRAPHICS_RESOURCE_WIDTH, GRAPHICS_RESOURCE_HEIGHT, VC_DISPMAN_ROT0, 0);
  graphics_delete_resource(img);

  return 0;
}

int getTemp(char *tempVal, char *freezerStat, uint32_t *freezerStatColor, char *co2Val)
{
  int socket_desc;
  struct sockaddr_in server;
  char *message, server_reply[4000];
  char *realContent;
  char *tok;

  memset(server_reply, 0, 4000);

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }

  server.sin_addr.s_addr = inet_addr("217.160.231.123");
  server.sin_family = AF_INET;
  server.sin_port = htons(80);

  //Connect to remote server
  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    puts("connect error");
    return 1;
  }

  //Send HTTP message
  message = "GET /dashboard/ajaxdata.php?format=2&tempOnly=1 HTTP/1.1\r\nHost: www.kai-merklein.de\r\n\r\n";

  if (send(socket_desc, message, strlen(message), 0) < 0)
  {
    puts("Send failed");
    return 1;
  }

  //Receive a reply from the server
  if (recv(socket_desc, server_reply, 4000, 0) < 0)
  {
    puts("recv failed");
  }

  realContent = strstr(server_reply, "\r\n\r\n") + 4;

  // Innen, aussen, Pool
  tok = strtok(realContent, "#");
  if (tok)
  {
    strcpy(tempVal, tok);

    // Freezer
    tok = strtok(NULL, "#"); // Temp + Stat
    if (tok)
      strcpy(freezerStat, tok);
    tok = strtok(NULL, "#"); // Color: 0: normal, 1: gruen, 2: rot
    if (tok)
      *freezerStatColor = atoi(tok);

    // CO2
    tok = strtok(NULL, "#"); // Temp + Stat
    if (tok)
      strcpy(co2Val, tok);
  }

  close(socket_desc);

  return 0;
}
