////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov†cs Roland //
//			  //
// Hang Çs zene rutinok.  //
//                        //
////////////////////////////
#include "music.h"
// Ad Lib inicializ†l¢
char init_HSC(void)
{
   asm mov ax,0
   INIT_PROC();
   return;
}
// Ad Lib lej†tsz¢
// zene a zene sz†ma
void play_HSC(void)
{
   unsigned int s,o;
   s=FP_SEG(music);
   o=FP_OFF(music);
   asm{
      push ds
      mov  ax,1
      mov  ds,s
      mov  dx,o
   }
   INIT_PROC();
   asm pop ds
   return;
}
// Ad Lib meg†ll°t¢
void stop_HSC(void)
{
   asm mov ax,3
   INIT_PROC();
   return;
}
// Detekt†lja a digit†lis hangcsippet
// Port ahol keresni kell
char reset_DSP(int Port)
{
   asm{
       push ax
       push dx
       push cx
       mov  dx,Port
       add  dl,6
       mov  al,1
       out  dx,al
       sub  al,al
       }
Delay:
   asm{
       dec  al
       jnz  Delay
       out  dx,al
       sub  cx,cx
       }
Empty:
   asm{
       mov  dx,Port
       add  dl,0x0e
       in   al,dx
       or   al,al
       jns  NextLoop
       sub  dl,4
       in   al,dx
       cmp  al,0xaa
       je   ResetOK
       inc  dx
       }
NextLoop:
   asm{
       loop Empty
       pop  cx
       pop  dx
       pop  ax
      }
   return(0);
ResetOK:
   asm{
       pop  cx
       pop  dx
       pop  ax
   }
   puts(detect3);
   return(1);
}
//Detekt†lja az MPU-401-es NIDI portot
//Port az interfÇsz c°me
char reset_MPU401(int Port)
{
   asm{
       push ax
       push dx
       push cx
       mov  dx,Port
       inc  dx
       }
Busy:
   asm{
       in   al,dx
       test al,0x40
       jnz  Busy
       mov  al,0xff
       out  dx,al
       sub  cx,cx
       }
Empty:
   asm{
       in   al,dx
       test al,0x80
       jnz  NextLoop
       dec  dx
       in   al,dx
       cmp  al,0xfe
       je   ResetOK
       inc  dx
       }
NextLoop:
   asm{
       loop Empty
       pop  cx
       pop  dx
       pop  ax
      }
   return(0);
ResetOK:
   asm{
       pop  cx
       pop  dx
       pop  ax
   }
   puts(detect2);
   return(1);
}
// UART m¢dba kapcsol
// Prt a prtc°m
char set_UART(int Port)
{
   asm{
       push ax
       push dx
       push cx
       mov  dx,Port
       inc  dx
       }
Busy:
   asm{
       in   al,dx
       test al,0x40
       jnz  Busy
       mov  al,0x3f
       out  dx,al
       sub  cx,cx
       }
Empty:
   asm{
       in   al,dx
       test al,0x80
       jnz  NextLoop
       dec  dx
       in   al,dx
       cmp  al,0xfe
       je   UARTMode
       inc  dx
       }
NextLoop:
   asm{
       loop Empty
       pop  cx
       pop  dx
       pop  ax
   }
   return(0);
UARTMode:
   asm{
       pop  cx
       pop  dx
       pop  ax
   }
   return(1);
}
//KikÅld egy MIDI utas°t†st
//Port a prtc°m
//midi egy midid utas°t†s
void send_MIDI(int Port,unsigned char Midi)
{
   asm{
       push ax
       push dx
       push cx
       mov  dx,Port
       inc  dx
       }
Busy:
   asm{
       in   al,dx
       test al,0x40
       jnz  Busy
       mov  al,Midi
       dec  dx
       out  dx,al
       pop  cx
       pop  dx
       pop  ax
   }
   return;
}
//DSP interrupt kezelî
void far interrupt sbhandler()
{
   enable();
   DMA_Ok=1;
   inportb(0x0e);
   outportb(0x20,0x20);
   return;
}
//Be†ll°tja a lej†tsz†si sebessÇget
//Rate   lej†tsz†si sebessÇk KHz-ben
unsigned int sb_sample_rate(unsigned int Rate)
{
   unsigned char tc;
   tc=(unsigned char)(256-((1000000L+Rate/2)/Rate));
   Rate=(unsigned int)(1000000L/(256-tc));
   writedac(0x40);
   writedac(tc);
   return Rate;
}
//A Speakert kapcsolja ki,be
//State 0 ki, 1 be
void sb_voice(int State)
{
   writedac((State)?0xd1:0xd3);
}
//A DSP-t kezeli
//Com a kikÅldendî utas°t†s
void writedac(int Com)
{
   while(inportb(Mentes.DPort+0x0c)&0x80);
   outportb(Mentes.DPort+0x0c,Com);
   return;
}
