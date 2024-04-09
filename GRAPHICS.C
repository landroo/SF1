////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov†cs Roland //
//			  //
// Grafikus rutinok.	  //
//                        //
////////////////////////////
// Vide¢ m¢d lekÇrdezÇs
char get_vid_mod(void)
{
   char Mod;
   asm{
      push ax
      mov  ah,0x0f
      mov  al,0x00
      int  0x10
      mov  Mod,al
      pop  ax
      }
   return Mod;
}
// 320x240-es vide¢m¢d
void set_vid1_mod(void)
{
   asm{
      push dx
      push ax
      mov  ax,0x0013
      int  0x10
      mov  dx,0x03C2
      mov  al,0x0e3
      out  dx,al
      lea  si,Data_480
      mov  cx,18
      mov  dx,0x03d4
      mov  ax,0x0011
      out  dx,ax
      mov  al,6
      }
Set_Regs:
   asm{
      mov  ah,[si]
      out  dx,ax
      inc  al
      inc  si
      loop Set_Regs
      mov  dx,0x03C4
      mov  ax,0x0604
      out  dx,ax
      mov  dx,0x03d4
      mov  ax,0x0c009
      out  dx,ax
      pop  ax
      pop  dx
      }
   return;
}
// 360x480-as vide¢m¢d
void set_vid2_mod(void)
{
   asm{
      mov  ax,0x0013
      int  0x10
      mov  dx,0x03C2
      mov  al,0x0e3
      out  dx,al
      lea  si,Data_480
      mov  cx,18
      mov  dx,0x03d4
      mov  ax,0x0011
      out  dx,ax
      mov  al,0x6
      }
Set_Regs1:
   asm{
      mov  ah,[si]
      out  dx,ax
      inc  al
      inc  si
      loop Set_Regs1
      mov  dx,0x03C4
      mov  ax,0x0604
      out  dx,ax
      mov  dx,0x03c2
      mov  ax,0x00e7
      out  dx,ax
      mov  dx,0x03d4
      mov  ax,0x0e11
      out  dx,ax
      lea  si,Data_360
      mov  cx,6
      }
Set_Regs2:
   asm{
      mov  ax,[si]
      mov  bx,0xff
      mul  bx
      add  ax,cx
      out  dx,ax
      inc  al
      inc  si
      loop Set_Regs2
      mov  dx,0x03d4
      mov  ax,0x2d13
      out  dx,ax
      mov  dx,0x3d4
      mov  ax,0x8e11
      out  dx,ax
      }
   return;
}
// Vide¢ m¢d be†ll°t†s
// mod az eredeti vide¢ m¢d
void end_vid_mod(unsigned char mod)
{
   asm{
      push ax
      mov  ah,0x00
      mov  al,mod
      int  0x10
      pop  ax
   }
   return;
}
// LapbeÇll°t†s
// Page 0-†s vagy 1-es lap
// 1 ha y 0 l†tszik
// 0 ha y 240 l†tszik
void set_act_page(char Page)
{
   unsigned int Kimh,Kiml;
   asm{
      push ax
      push dx
      push cx
      mov  Kimh,0x4b0c
      mov  Kiml,0x000d
      mov  Akt_Page,0x01
      mov  al,Page
      mov  ah,0x01
      cmp  ah,al
      je   Set_Page
      mov  Kimh,0x000c
      mov  Kiml,0x000d
      mov  Akt_Page,0x00
      }
Set_Page:
   asm{
      mov  dx,0x03d4
      mov  ax,Kimh
      out  dx,ax
      mov  ax,Kiml
      out  dx,ax
      pop  cx
      pop  dx
      pop  ax
      }
   return;
}
// Paletta be†ll°t†s
// ColReg a sz°n sz†ma
// r,g,b Piros,Zîld, KÇk
void setcol(char ColReg, char Red, char Green, char Blue)
{
   asm{
      mov  dx,0x03c8
      mov  al,[ColReg]
      out  dx,al
      inc  dx
      mov  al,[Red]
      out  dx,al
      mov  al,[Green]
      out  dx,al
      mov  al,[Blue]
      out  dx,al
      }
  return;
}
// FÅggîlege visszafut†s v†r†s
void wait_sinc(void)
{
   asm{
      push dx
      push ax
      mov  dx,0x03da
      mov  ah,0x08
    }
Visz:
   asm{
      in   al,dx
      test al,ah
      jz   Visz
      pop  ax
      pop  dx
      }
   return;
}
// KÇprÇsz tîrlÇs
// Xk,Yk kezdî pont
// Xv,Yv vÇgpont
// Color a tîrîsz°n
void torol(int Xk,int Yk,int Xv,int Yv,char Color)
{
   asm{
      push es
      mov  ax,Video
      mov  es,ax
      mov  dx,0x03c4
      mov  al,0x02
      mov  ah,0x0f
      out  dx,ax
      mov  ax,Yk
      mov  dx,0x50
      mul  dx
      add  ax,Xk
      mov  di,ax
      mov  ax,Xv
      mov  bx,Xk
      sub  ax,bx
      mov  bx,0x08
      div  bx
      mov  dx,ax
      mov  ax,Yv
      mov  bx,Yk
      sub  ax,bx
      mov  cx,ax
      }
Cik1:
   asm{
      push cx
      mov  cx,dx
      push di
   }
Cik2:
   asm{
      mov  al,Color
      mov  ah,Color
      mov  es:[di],ax
      inc  di
      inc  di
      loop Cik2
      pop  di
      pop  cx
      add  di,0x50
      loop Cik1
      pop  es
   }
   return;
}
// Grafika megjelen°tÇse
// X,Y   hova
// Koc   mit
// Sx,Sy mÇret
void show_litl(int X,int Y,unsigned char *Koc,int Sx,int Sy)
{
   asm{
      push ds
      push es
      mov  ax,0xa000

      mov  es,ax
      mov  dx,0x03c4
      mov  ax,X
      mov  ah,al
      and  ah,0x03
      mov  al,0x01
      mov  cl,ah
      shl  al,cl
      push ax
      mov  ah,al
      mov  al,0x02
      out  dx,ax

      lds  bx,AddTmb
      mov  di,Y
      shl  di,1
      mov  di,[bx+di]

      mov  ax,X
      shr  ax,2
      add  di,ax
      lds  ax,Koc
      mov  si,ax
      mov  cx,Sx
      }
Cik_x:
   asm{
      push cx
      push di
      push si
      mov  cx,Sy
      }
Cik_y:
   asm{
      mov  al,[si]
      cmp  al,0xff
      je   Hat_Szin
      mov  es:[di],al
      }
Hat_Szin:
   asm{
      add  si,Sx
      add  di,0x50
      loop Cik_y
      pop  si
      pop  di
      pop  cx
      pop  ax
      shl  al,1
      mov  ah,0x10
      cmp  ah,al
      jne  Uj_Plane
      mov  al,0x01
      inc  di
      }
Uj_Plane:
   asm{
      push ax
      mov  dx,0x03c4
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      inc  si
      loop Cik_x
      pop  ax
      pop  es
      pop  ds
      }
   return;
}
// Grafika megjelen°tÇse
// X,Y   hova
// Koc   mit
// Sx,Sy mÇret
void show_big1(int X,int Y,unsigned char *Koc,int Sx,int Sy)
{
   asm{
      push ds
      push es
      mov  ax,0xa000
      mov  es,ax
      mov  dx,0x03c4
      mov  ax,X
      mov  ah,al
      and  ah,0x03
      mov  al,0x01
      mov  cl,ah
      shl  al,cl
      push ax
      mov  ah,al
      mov  al,0x02
      out  dx,ax

      lds  bx,AddTmb
      mov  di,Y
      shl  di,1
      mov  di,[bx+di]

      mov  ax,X
      shr  ax,2
      add  di,ax
      lds  ax,Koc

      mov  si,ax
      mov  ax,Sx
      mov  cx,0x04
      xor  dx,dx
      div  cx
      mov  bx,ax
      }
Cik_p:
   asm{
      push cx
      push di
      push si
      mov  cx,bx
      cmp  dx,0x00
      je   Cik_x
      dec  dx
      inc  cx
      }
Cik_x:
   asm{
      push cx
      push di
      push si
      mov  cx,Sy
      }
Cik_y:
   asm{
      mov  al,[si]
      cmp  al,0xff
      je   Hat_Szin
      mov  es:[di],al
      }
Hat_Szin:
   asm{
      add  si,Sx
      add  di,0x50
      loop Cik_y
      pop  si
      pop  di
      pop  cx
      add  si,0x04
      inc  di
      loop Cik_x
      pop  si
      pop  di
      pop  cx
      pop  ax
      shl  al,1
      mov  ah,0x10
      cmp  ah,al
      jne  Uj_Plane
      mov  al,0x01
      inc  di
      }
Uj_Plane:
   asm{
      push ax
      push dx
      mov  dx,0x03c4
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      pop  dx
      inc  si
      loop Cik_p
      pop  ax
      pop  es
      pop  ds
      }
   return;
}
// Grafika megjelen°tÇse
// X,Y   hova
// Koc   mit
// Sx,Sy eredeti mÇret
// Kx,Ky kezdet
// Vx,Vy vÇg
void show_big2(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy)
{
   unsigned int Kezd,Reszx,Reszy;
   asm{
      push ds
      push es
      mov  ax,Ky
      mov  cx,Sx
      mul  cx
      add  ax,Kx
      mov  Kezd,ax
      mov  ax,Sx
      mov  cx,Kx
      sub  ax,cx
      mov  cx,Vx
      sub  ax,cx
      mov  Reszx,ax
      mov  ax,Sy
      mov  cx,Ky
      sub  ax,cx
      mov  cx,Vy
      sub  ax,cx
      mov  Reszy,ax
      mov  ax,0xa000
      mov  es,ax
      mov  dx,0x03c4
      mov  ax,X
      mov  ah,al
      and  ah,0x03
      mov  al,0x01
      mov  cl,ah
      shl  al,cl
      push ax
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      lds  bx,AddTmb
      mov  di,Y
      shl  di,1
      mov  di,[bx+di]
      mov  ax,X
      shr  ax,2
      add  di,ax
      lds  ax,Koc
      add  ax,Kezd
      mov  si,ax
      mov  ax,Reszx
      mov  cx,0x04
      xor  dx,dx
      div  cx
      mov  bx,ax
      }
Cik_p:
   asm{
      push cx
      push di
      push si
      mov  cx,bx
      cmp  dx,0x00
      je   Cik_x
      dec  dx
      inc  cx
      }
Cik_x:
   asm{
      push cx
      push di
      push si
      mov  cx,Reszy
      }
Cik_y:
   asm{
      mov  al,[si]
      cmp  al,0xff
      je   Hat_Szin
      mov  es:[di],al
      }
Hat_Szin:
   asm{
      add  si,Sx
      add  di,0x50
      loop Cik_y
      pop  si
      pop  di
      pop  cx
      add  si,0x04
      inc  di
      loop Cik_x
      pop  si
      pop  di
      pop  cx
      pop  ax
      shl  al,1
      mov  ah,0x10
      cmp  ah,al
      jne  Uj_Plane
      mov  al,0x01
      inc  di
      }
Uj_Plane:
   asm{
      push ax
      push dx
      mov  dx,0x03c4
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      pop  dx
      inc  si
      loop Cik_p
      pop  ax
      pop  es
      pop  ds
      }
   return;
}
// Grafika megjelen°tÇse
// X,Y   hova
// Koc   mit
// Sx,Sy eredeti mÇret
// Kx,Ky kezdet
// Vx,Vy vÇg
void show_big3(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy)
{
   unsigned int Kezd,Reszx,Reszy;
   asm{
      push ds
      push es
      mov  ax,Ky
      mov  cx,Sx
      mul  cx
      add  ax,Kx
      mov  Kezd,ax
      mov  ax,Sx
      mov  cx,Kx
      sub  ax,cx
      mov  cx,Vx
      sub  ax,cx
      shr  ax,1
      mov  Reszx,ax
      mov  ax,Sy
      mov  cx,Ky
      sub  ax,cx
      mov  cx,Vy
      sub  ax,cx
      mov  Reszy,ax
      mov  ax,0xa000
      mov  es,ax
      mov  dx,0x03c4
      mov  ax,X
      shr  ax,1
      mov  ah,al
      and  ah,0x01
      mov  al,0x03
      mov  cl,ah
      shl  al,cl
      push ax
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      lds  bx,AddTmb
      mov  di,Y
      shl  di,1
      mov  di,[bx+di]
      mov  ax,X
      shr  ax,2
      add  di,ax
      lds  ax,Koc
      add  ax,Kezd
      mov  si,ax
      mov  ax,Reszx
      mov  cx,0x04
      xor  dx,dx
      div  cx
      mov  bx,ax
      }
Cik_p:
   asm{
      push cx
      push di
      push si
      mov  cx,bx
      cmp  dx,0x00
      je   Cik_x
      dec  dx
      inc  cx
      }
Cik_x:
   asm{
      push cx
      push di
      push si
      mov  cx,Reszy
      }
Cik_y:
   asm{
      mov  al,[si]
      cmp  al,0xff
      je   Hat_Szin
      mov  es:[di],al
      }
Hat_Szin:
   asm{
      add  si,Sx
      add  di,0x50
      loop Cik_y
      pop  si
      pop  di
      pop  cx
      add  si,0x08
      inc  di
      inc  di
      loop Cik_x
      pop  si
      pop  di
      pop  cx
      pop  ax
      shl  al,2
      mov  ah,48
      cmp  ah,al
      jne  Uj_Plane
      mov  al,3
      inc  di
      }
Uj_Plane:
   asm{
      push ax
      push dx
      mov  dx,0x03c4
      mov  ah,al
      mov  al,0x02
      out  dx,ax
      pop  dx
      inc  si
      inc  si
      loop Cik_p
      pop  ax
      pop  es
      pop  ds
      }
   return;
}
// Grafika megjelen°tÇse
// X,Y   hova
// Koc   mit
// Sx,Sy eredeti mÇret
// Kx,Ky kezdet
// Vx,Vy vÇg
void show_big4(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy)
{
   unsigned int Kezd,Reszx,Reszy;
   asm{
      push ds
      push es
      mov  ax,Ky
      mov  cx,Sx
      mul  cx
      add  ax,Kx
      mov  Kezd,ax
      mov  ax,Sx
      mov  cx,Kx
      sub  ax,cx
      mov  cx,Vx
      sub  ax,cx
      shr  ax,2
      mov  Reszx,ax
      mov  ax,Sy
      mov  cx,Ky
      sub  ax,cx
      mov  cx,Vy
      sub  ax,cx
      mov  Reszy,ax
      mov  ax,0xa000
      mov  es,ax
      mov  dx,0x03c4
      mov  ax,X
      shr  ax,2
      mov  al,2
      mov  ah,0x0f
      out  dx,ax
      lds  bx,AddTmb
      mov  di,Y
      shl  di,1
      mov  di,[bx+di]
      mov  ax,X
      shr  ax,2
      add  di,ax
      lds  ax,Koc
      add  ax,Kezd
      mov  si,ax
      mov  ax,0x50
      mov  cx,Reszx
      sub  ax,cx
      mov  dx,ax
      mov  bx,Reszy
      }
Cik_y:
   asm{
      mov  cx,Reszx
      }
Cik_x:
   asm{
      mov  al,[si]
      cmp  al,0xff
      je   Hat_Szin
      mov  [es:di],al
      }
Hat_Szin:
   asm{
      inc  di
      add  si,4
      dec  cx
      jnz  Cik_x
      add  di,dx
      mov  ax,Kx
      add  ax,Vx
      add  si,ax
      dec  bx
      jnz  Cik_y
      pop  es
      pop  ds
      }
   return;
}
