////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov†cs Roland //
//			  //
// EgyÇb rutinok.	  //
//                        //
////////////////////////////
// Grafik†k betîltÇse
char tolt(void)
{
   FILE *Fp;
   if((Slime=(char *)malloc(65535U))==NULL)return(0);
   slime_tolt("emb1.tga");
   Fp=fopen("emb2.tga","rb");
   if((Emb2=(unsigned char *)malloc(13440U))==NULL)return(0);
   fread(Emb2,1,18,Fp);fread(Emb2,1,768,Fp);fread(Emb2,1,13440U,Fp);
   fclose(Fp);
   Fp=fopen("emb3.tga","rb");
   if((Emb3=(unsigned char *)malloc(832U))==NULL)return(0);
   fread(Emb3,1,18,Fp);fread(Emb3,1,768,Fp);fread(Emb3,1,832U,Fp);
   fclose(Fp);
   Fp=fopen("hat11.tga","rb");
   if((Hat1=(unsigned char *)malloc(46400U))==NULL)return(0);
   fread(Hat1,1,18,Fp);fread(Pal2,1,768,Fp);fread(Hat1,1,46400U,Fp);
   fclose(Fp);
   Fp=fopen("hat12.tga","rb");
   if((Hat2=(unsigned char *)malloc(60800U))==NULL)return(0);
   fread(Hat2,1,18,Fp);fread(Hat2,1,768,Fp);fread(Hat2,1,60800U,Fp);
   fclose(Fp);
   Fp=fopen("hat13.tga","rb");
   if((Hat3=(unsigned char *)malloc(768U))==NULL)return(0);
   fread(Hat3,1,18,Fp);fread(Hat3,1,768,Fp);fread(Hat3,1,768U,Fp);
   fclose(Fp);
   Fp=fopen("hat14.tga","rb");
   if((Hat4=(unsigned char *)malloc(768U))==NULL)return(0);
   fread(Hat4,1,18,Fp);fread(Hat4,1,768,Fp);fread(Hat4,1,768U,Fp);
   fclose(Fp);
   Fp=fopen("hajo.tga","rb");
   fread(Hajo,1,18,Fp);fread(Elan[0],1,768,Fp);fread(Hajo[0],1,720,Fp);
   fclose(Fp);
   Fp=fopen("ellen1.tga","rb");
   fread(Elan[0],1,18,Fp);fread(Elan[0],1,768,Fp);fread(Elan[0],1,2160,Fp);
   fclose(Fp);
   Fp=fopen("ellen2.tga","rb");
   fread(Elan[1],1,18,Fp);fread(Elan[1],1,768,Fp);fread(Elan[1],1,2160,Fp);
   fclose(Fp);
   Fp=fopen("ellen3.tga","rb");
   fread(Elan[2],1,18,Fp);fread(Elan[2],1,768,Fp);fread(Elan[2],1,2160,Fp);
   fclose(Fp);
   Fp=fopen("ellen4.tga","rb");
   fread(Elan[3],1,18,Fp);fread(Elan[3],1,768,Fp);fread(Elan[3],1,2160,Fp);
   fclose(Fp);
   Fp=fopen("szamok.tga","rb");
   fread(Szam[0],1,18,Fp);fread(Szam[0],1,768,Fp);fread(Szam[0],1,1440,Fp);
   fclose(Fp);
   Fp=fopen("font.tga","rb");
   fread(Betuk[0],1,18,Fp);fread(Pal1,1,768,Fp);fread(Betuk[0],1,10584,Fp);
   fclose(Fp);
   Fp=fopen("robban1.tga","rb");
   fread(Rob1[0],1,18,Fp);fread(Rob1[0],1,768,Fp);fread(Rob1[0],1,2304,Fp);
   fclose(Fp);
   Fp=fopen("robban2.tga","rb");
   fread(Rob2[0],1,18,Fp);fread(Rob2[0],1,768,Fp);fread(Rob2[0],1,13824,Fp);
   fclose(Fp);
   Fp=fopen("fire.wav","rb");
   if((Wav1=(char *)malloc(10754U))==NULL);
   fread(Wav1,1,10754,Fp);
   fclose(Fp);
   Fp=fopen("bomb.wav","rb");
   if((Wav2=(char *)malloc(17362U))==NULL);
   fread(Wav2,1,17362,Fp);
   fclose(Fp);
   return(1);
}
// Timer megszak°t†s
void interrupt idozites(void)
{
   asm{
      mov  ax,Ido
      inc  ax
      cmp  ax,0xffff
      jne  Nulla
      xor  ax,ax
   }
Nulla:
   asm{
      mov  Ido,ax
     }
   oldhandler3();
   return;
}
// BillnetyÅ megszk°t†s
void interrupt billentyu(void)
{
   asm{
      push ds
      push ax
      push bx
      push cx
      mov  ax,SEG Key
      mov  ds,ax
      xor  bh,bh
      mov  cl,bh
      in   al,0x60
      mov  bl,al
      and  bl,0x7f
      shl  al,1
      cmc
      adc  cl,0x00
      mov  [bx+OFFSET Key],cl
      in   al,0x61
      mov  ah,al
      or   al,0x80
      out  0x61,al
      mov  al,ah
      out  0x61,al
      cli
      mov  al,0x20
      out  0x20,al
      sti
      pop  cx
      pop  bx
      pop  ax
      pop  ds
      }
   KeyHit=1;
   return;
}
// Sîveg ki°r†s
// Sz_x,Sz_y hova
// *Szoveg   mit
// High      milyen szinnel
void kiir(int Sz_x,int Sz_y,unsigned char *Szoveg)
{
   int f;
   for(f=1;f<Szoveg[0]+1;f++){
      if(Szoveg[f]<40)show_big1(Sz_x,Sz_y,Betuk[Szoveg[f]-1],14,18);
      Sz_x+=14;
   }
   return;
}
// Sz†mok megjelen°tÇse
// Sz_x,Sz_y hova
// Szm       mit
// Nul       Kell-e a nulla
// Szt	     Milyen karakter
void szam_ir(int Sz_x,int Sz_y,long Szm,char Nul,char Szt)
{
   char f,n;
   long tmp=100000000L;
   for(f=0;f<8;f++){
     tmp/=10;
     if(tmp<=Szm){
	n=Szm/tmp;
	Szm=Szm-n*tmp;
	if(Szt){
	   show_big1(Sz_x,Sz_y,Betuk[n+31],14,18);
	   Sz_x+=14;
	}
	else{
	   show_big1(Sz_x,Sz_y,Szam[n],12,12);
	   Sz_x+=12;
	}
     }
     else
	if((8-Nul)<=f){
	   if(Szt){
	      show_big1(Sz_x,Sz_y,Betuk[31],14,18);
	      Sz_x+=14;
	   }
	   else{
	      show_big1(Sz_x,Sz_y,Szam[0],12,12);
	      Sz_x+=12;
	   }
	}
   }
   return;
}
// mas lessz
char slime_tolt(char *nev)
{
   FILE *Fp;
   long f;
   Fp=fopen(nev,"rb");
//   if(Slime!=NULL)free(Slime);
//   if((Slime=(char *)malloc(65535U))==NULL)return(0);
   if(Bon)for(f=0;f<=65535U;f++)Slime[f]=fgetc(Fp);
   else fread(Slime,1,18,Fp);fread(Pal1,1,768,Fp);fread(Slime,1,16384U,Fp);
   fclose(Fp);
   return(1);
}
// V†r¢ rutin
// v v†r†s
void var(unsigned int v)
{
  unsigned int t;
  unsigned char s=1;
  while(s){
     s=0;
     for(t=0;t<128;t++)
	if(Key[t])s=1;
  }
  KeyHit=0;
  t=Ido;
  s=v;
  while(s){
     if(t!=Ido){
	s--;
	t=Ido;
     }
     if(KeyHit)s=0;
  }
  return;
}
