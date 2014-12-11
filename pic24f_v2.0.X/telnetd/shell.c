 /*
 * Copyright (c) 2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: shell.c,v 1.1 2006/06/07 09:43:54 adam Exp $
 *
 */

#include "shell.h"

#include <string.h>
#include "../HardwareProfile.h"
#include "../uart2.h"
#include "../IR_control.h"

extern volatile unsigned long blinkLED_timer1 ;
extern volatile unsigned long ir_timer1;
extern volatile unsigned long ir_tx_times;
extern void (*ir_f)(void);

struct ptentry {
  char *commandstr;
  void (* pfunc)(char *str);
};

#define SHELL_PROMPT "uIP 1.0> "

/*---------------------------------------------------------------------------*/
static void
parse(register char *str, struct ptentry *t)
{
  struct ptentry *p;
  for(p = t; p->commandstr != NULL; ++p) {
    if(strncmp(p->commandstr, str, strlen(p->commandstr)) == 0) {
      break;
    }
  }

  p->pfunc(str);
}
/*---------------------------------------------------------------------------*/
static void
inttostr(register char *str, unsigned int i)
{
  str[0] = '0' + i / 100;
  if(str[0] == '0') {
    str[0] = ' ';
  }
  str[1] = '0' + (i / 10) % 10;
  if(str[0] == ' ' && str[1] == '0') {
    str[1] = ' ';
  }
  str[2] = '0' + i % 10;
  str[3] = ' ';
  str[4] = 0;
}
/*---------------------------------------------------------------------------*/
static void
help(char *str)
{
  shell_output("Available commands:", "");
  shell_output("stats   - show network statistics", "");
  shell_output("conn    - show TCP connections", "");
  shell_output("help, ? - show help", "");
  shell_output("exit    - exit shell", "");
}
/*---------------------------------------------------------------------------*/
static void
unknown(char *str)
{
  if(strlen(str) > 0) {
    shell_output("Unknown command: ", str);
  }
}
#define SkipWhiteSpace(x)    { while (*x && (*x == ' ')) x++; }
#define SkipCharacter(x)    { while (*x && (*x != ' ')) x++; }

void get_temp(char* str)
{
    int voltage = 0;
    int i;
    double temp;

    AD1CHS = 4;                    //ADC channel select 4
    AD1CON1bits.ADON =1;           // turn ADC on

    for(i=0; i<8; i++)
    {
        AD1CON1bits.DONE=0;         //resets DONE bit
        AD1CON1bits.SAMP=1;         //start sample
        while(AD1CON1bits.DONE==0); //wait for conversion to finish
        voltage += ADC1BUF0;         //get the voltage measurement
    }
    AD1CON1bits.ADON = 0;           // turn ADC off
    temp = (315.f*(voltage>>3))/1024.f-50.f;

    sprintf(Buffer, "Temp = %02f°C (%d)", temp, voltage);
    shell_output(Buffer, "");
}

void get_luminosity(char* str)
{
    int voltage = 0;
    int i;

    AD1CHS = 5;                    // ADC channel select 5
    AD1CON1bits.ADON =1;           // turn ADC on
    for(i=0; i<8; i++)
    {
        AD1CON1bits.DONE=0;         //resets DONE bit
        AD1CON1bits.SAMP=1;         //start sample
        while(AD1CON1bits.DONE==0); //wait for conversion to finish
        voltage += ADC1BUF0;         //get the voltage measurement
    }
    AD1CON1bits.ADON = 0;           // turn ADC off
    sprintf(Buffer, "Lum = %d", voltage>>3);
    shell_output(Buffer, "");
}

void
command_tv_td(char *str)
{
    char* cmd = str;
    // Remove first command
    // SkipCharacter(cmd);
    // SkipWhiteSpace(cmd);

    // if((!strncmp(cmd, "start", 4)) || (!strncmp(cmd, "stop", 4))) {
    //     ir_sendcmd_rc6(0x0C, 1);
    // }
    // else if(!strncmp(cmd, "vp", 2)) {
    //     ir_sendcmd_rc6(0x10, 1);
    // }
    // else if(!strncmp(cmd, "vm", 2)) {
    //     ir_sendcmd_rc6(0x11, 1);
    // }
    // else if(!strncmp(cmd, "pp", 2)) {
    //     ir_sendcmd_canal(0x14, 1);
    // }
    // else if(!strncmp(cmd, "pm", 2)) {
    //     ir_sendcmd_canal(0x13, 1);
    // }
    // else if(!strncmp(cmd, "zoom", 4)) {
    //     ir_sendcmd_canal(0x16, 1);
    // }
    // else if(!strncmp(cmd, "menu", 4)) {
    //     ir_sendcmd_canal(0x19, 1);
    // }
    // else if(!strncmp(cmd, "guide", 5)) {
    //     ir_sendcmd_canal(0x1A, 1);
    // }
    // else if(!strncmp(cmd, "sortie", 6)) {
    //     ir_sendcmd_canal(0x1B, 1);
    // }
    // else if(!strncmp(cmd, "return", 7)) {
    //     ir_sendcmd_canal(0x1C, 1);
    // }
    // else if(!strncmp(cmd, "menub", 5)) {
    //     ir_sendcmd_canal(0x1F, 1);
    // }


    // else if(!strncmp(cmd, "play", 4)) {
    //     ir_sendcmd_canal(0x36, 1);
    // }
    // else if(!strncmp(cmd, "cstop", 5)) {
    //     ir_sendcmd_canal(0x38, 1);
    // }
    // else if(!strncmp(cmd, "pause", 5)) {
    //     ir_sendcmd_canal(0x3A, 1);
    // }

    // else if(!strncmp(cmd, "cr", 2)) {
    //     ir_sendcmd_canal(0x20, 1);
    // }
    // else if(!strncmp(cmd, "cu", 2)) {
    //     ir_sendcmd_canal(0x22, 1);
    // }
    // else if(!strncmp(cmd, "cl", 2)) {
    //     ir_sendcmd_canal(0x24, 1);
    // }
    // else if(!strncmp(cmd, "cd", 2)) {
    //     ir_sendcmd_canal(0x26, 1);
    // }
    // else if(!strncmp(cmd, "ce", 2)) {
    //     ir_sendcmd_canal(0x2a, 1);
    // }

    // else if(!strncmp(cmd, "mute", 4)) {
    //     ir_sendcmd_rc6(0x0D, 1);
    // }
    // else if(!strncmp(cmd, "src", 3)) {
    //     ir_sendcmd_rc6(0x38, 1);
    // }
    // else if(!strncmp(cmd, "info", 4)) {
    //     ir_sendcmd_rc6(0x0F, 1);
    // }
    // else if(!strncmp(cmd, "up", 2)) {
    //     ir_sendcmd_rc6(0x58, 1);
    // }
    // else if(!strncmp(cmd, "down", 4)) {
    //     ir_sendcmd_rc6(0x59, 1);
    // }
    // else if(!strncmp(cmd, "left", 4)) {
    //     ir_sendcmd_rc6(0x5A, 1);
    // }
    // else if(!strncmp(cmd, "right", 5)) {
    //     ir_sendcmd_rc6(0x5B, 1);
    // }
    // else if(!strncmp(cmd, "enter", 5)) {
    //     ir_sendcmd_rc6(0x5C, 1);
    // }
    // else {
    // #define IsNum(c)            (('0' <= c) && (c <= '9'))
    //     if(IsNum(cmd[0])) {
    //         ir_sendcmd_canal(((cmd[0]-'0')&0x0f), 1);
    //         shell_output("channel: ", cmd);
    //     }
    //     else {
    //         shell_output("TV: ", cmd);
    //     }
    // }
    shell_output("TV: ", cmd);
}
/*---------------------------------------------------------------------------*/
static struct ptentry parsetab[] =
  {{"stats", help},
   {"conn", help},
   {"help", help},
   {"exit", shell_quit},
   {"?", help},
   {"tv", command_tv_td},
   {"temp", get_temp},
   {"lum", get_luminosity},

   /* Default action */
   {NULL, unknown}};
/*---------------------------------------------------------------------------*/
void
shell_init(void)
{
}
/*---------------------------------------------------------------------------*/
void
shell_start(void)
{
  shell_output("uIP command shell", "");
  shell_output("Type '?' and return for help", "");
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
void
shell_input(char *cmd)
{
  parse(cmd, parsetab);
  shell_prompt(SHELL_PROMPT);
}
/*---------------------------------------------------------------------------*/
