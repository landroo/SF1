////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov cs Roland //
//			  //
// Plasma h tt‚r rutinok. //
//                        //
////////////////////////////

#include "dxtmb.h"
#include "dytmb.h"
#include "slimes.h"
#include "slimec.h"

// Menu h tt‚r
void texscreen(void)
{
  unsigned int  X,Y;
  unsigned char Color;
  long Ystart,Xstart,Xpos,Ypos;
  int  Adx2,Xp2,Yp2;
  Dx=DxTmb[Deg];
  Dy=DyTmb[Deg];
  Adx2=-Dy>>8;
  if(Akt_Page)_SI=0;
  else _SI=19200;
  Xstart=Xo;
  Ystart=Yo;
  asm{
     mov  dx,0x03c4
     mov  al,0x02
     mov  ah,0x0f
     out  dx,ax
  }
  for(X=0;X<Xsize;X++){
     Xpos=Xstart;
     Ypos=Ystart;
     Xp2=Xpos>>8;
     Yp2=Ypos>>8;
     for(Y=0;Y<Ysize;Y++){
	asm{
	   mov  bx,[Yp2]
	   mov  bl,byte ptr [Xp2]+1
	   shr  bx,1
	   shr  bx,1
	}
	Color=Slime[_BX];
	asm{
	   mov	ax,0xa000
	   mov	es,ax
	   mov  al,[Color]
	   mov  es:[si],al
	}
	Xp2+=Adx2;
	_SI+=80;
     }
     Xstart+=Dx+SlimeC[X&127];
     Ystart+=Dy;
     _SI-=(80*(Ysize))-1;
   }
   return;
}
// Bonus level h tt‚r
void plasma(void)
{
   int pl1,pl2;
   unsigned Kezd;
   if(Akt_Page)Kezd=0;
   else Kezd=19200;
   pl1=48+cos(Teller/37)*47.0+256*(int)(48+47*(sin(Teller/31)));
   pl2=48+sin(Teller/24)*47.0+256*(int)(48+47*(cos(Teller/19)))-pl1;
   switch(Mentes.Grap){
      case 1:{
	      _ES=0xa000;
	      asm{
		  pusha
		  mov  di,Kezd
		  push ds
		  lds  si,Slime
		  add  si,pl1
		  mov  bx,pl2
		  mov  cl,114
	      }
l11:
	      asm{
		  mov  dx,80
	      }
l12:
	      asm{
		  push dx
		  mov  dx,0x03c4
		  mov  al,2
		  mov  ah,0x03
		  out  dx,ax
		  pop  dx
		  lodsb
		  add  al,[si+bx]
		  mov  es:[di+80],al
		  stosb
		  dec  di
		  push dx
		  mov  dx,0x03c4
		  mov  al,2
		  mov  ah,0x0c
		  out  dx,ax
		  pop  dx
		  lodsb
		  add  al,[si+bx]
		  mov  es:[di+80],al
		  stosb
		  dec  dx
		  jnz  l12
		  add  si,256-160
		  add  di,80
		  dec  cl
		  jnz  l11
		  pop  ds
		  popa
	      }
	      break;
	     }
      case 2:{
	      _ES=0xa000;
	      asm{
		  pusha
		  mov  di,Kezd
		  push ds
		  lds  si,Slime
		  add  si,pl1
		  mov  bx,pl2
		  mov  cl,114
	      }
l21:
	      asm{
		  mov  dx,80
	      }
l22:
	      asm{
		  push dx
		  mov  dx,0x03c4
		  mov  al,2
		  mov  ah,0x0f
		  out  dx,ax
		  pop  dx
		  lodsb
		  add  al,[si+bx]
		  mov  es:[di+80],al
		  stosb
		  dec  dx
		  jnz  l22
		  add  si,256-80
		  add  di,80
		  dec  cl
		  jnz  l21
		  pop  ds
		  popa
	      }
	      break;
	     }
      case 3:{
	      _ES=0xa000;
	      asm{
		  pusha
		  mov  di,Kezd
		  push ds
		  lds  si,Slime
		  add  si,pl1
		  mov  bx,pl2
		  mov  cl,114
	      }
l31:
	      asm{
		  mov  dx,40
	      }
l32:
	      asm{
		  push dx
		  mov  dx,0x03c4
		  mov  al,2
		  mov  ah,0x0f
		  out  dx,ax
		  pop  dx
		  lodsb
		  add  al,[si+bx]
		  mov  ah,al
		  mov  es:[di+80],ax
		  stosw
		  dec  dx
		  jnz  l32
		  add  si,256-40
		  add  di,80
		  dec  cl
		  jnz  l31
		  pop  ds
		  popa
	      }
	      break;
	     }
      case 4:{
	      torol(0,(!Akt_Page)*240,320,228+(!Akt_Page)*240,0);
	      break;
	     }
   }
   show_big1(0,228+Akt_Page*240,Hat3,64,12);
   show_big1(160,228+Akt_Page*240,Hat4,64,12);
   return;
}

