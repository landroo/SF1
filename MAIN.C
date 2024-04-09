////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov cs Roland //
//			  //
// F”program.		  //
//                        //
////////////////////////////
// inkl£d f jlok
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

#include "sf1.h"
#include "szovegek.h"

#include "menu.c"
#include "graphics.c"
#include "sound.c"
#include "plasma.c"
#include "others.c"

// F”program
// argc parameterek
// argv parameterek mutatoja
void main(int argc,char *argv)
{
   unsigned char tm;
   if(argc||argv);	// Ide kell meg valamit irni
   if(init1())fomenu();
   if(Mentes.Music==2)stop_HSC();
   setvect(Bill,oldhandler1);
   setvect(Tmer,oldhandler2);
   end_vid_mod(Akt_Vid_Mod);
/*   if(Mentes.DPort){
      sb_voice(0);
      disable();
      setvect(0x08+Mentes.SBIRQ,OldIRQ);
      tm=inportb(0x21);
      outportb(0x21,tm|(1<<Mentes.SBIRQ));
      enable();
   }*/
   return;
}
// Program inicializ l s
char init1(void)
{
   int f;
   FILE *Fp;
   clrscr();
   randomize();
/*   if(reset_DSP(0x220))Mentes.DPort=0x220;
   else if(reset_DSP(0x240))Mentes.DPort=0x240;
   if(reset_MPU401(0x330))Mentes.MPort=0x330;
   else if(reset_MPU401(0x300))Mentes.MPort=0x300;
   if(Mentes.DPort){
      disable();
      OldIRQ=getvect(0x08+Mentes.SBIRQ);
      setvect(0x08+Mentes.SBIRQ,sbhandler);
      enable();
      sb_sample_rate(11000);
      sb_voice(1);
   }*/
   oldhandler1=getvect(Bill);
   setvect(Bill,billentyu);
   oldhandler2=getvect(Tmer);
   oldhandler3=getvect(Tmer);
   setvect(Tmer,idozites);
   if(!tolt())return(0);
   init2();
   if((AddTmb=(unsigned int *)malloc(480U))==NULL)return(0);
   for(f=0;f<480;f++)AddTmb[f]=f*80;
   Fp=fopen("sf1.cfg","rb");
   if(Fp==NULL){
      Mentes.DPort=0x220;
      Mentes.MPort=0x330;
      Mentes.SBIRQ=7;
      Mentes.SBDMA=1;
      Mentes.Music=1;
      Mentes.Sound=1;
      Mentes.Control=1;
      Mentes.Level=1;
      Mentes.Grap=1;
      for(f=0;f<14;f++)Mentes.Nev0[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev1[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev2[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev3[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev4[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev5[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev6[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev7[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev8[f]=Sor201[f];
      for(f=0;f<14;f++)Mentes.Nev9[f]=Sor201[f];
      Mentes.Pont0=1000;
      Mentes.Pont1=2000;
      Mentes.Pont2=3000;
      Mentes.Pont3=4000;
      Mentes.Pont4=5000;
      Mentes.Pont5=6000;
      Mentes.Pont6=7000;
      Mentes.Pont7=8000;
      Mentes.Pont8=9000;
      Mentes.Pont9=10000;
      Mentes.bal=0;
      Mentes.jobb=0;
      Mentes.tuz=0;
      Fp=fopen("sf1.cfg","wb");
      fwrite(&Mentes,sizeof(Mentes),1,Fp);
      fclose(Fp);
   }
   else{
      fread(&Mentes,sizeof(Mentes),1,Fp);
      fclose(Fp);
   }
   Akt_Vid_Mod=get_vid_mod();
   set_vid1_mod();
   for(f=0;f<256;f++)setcol(f,Pal1[f].Blue>>2,Pal1[f].Green>>2,Pal1[f].Red>>2);
   torol(0,0,320,480,0);
   if(Mentes.Music==2){
      init_HSC();
      play_HSC();
      oldhandler3=getvect(Tmer);
      setvect(Tmer,idozites);
   }
   return(1);
}
// P lya inicializ l s
void init2(void)
{
   int f;
   if(Bon&&(Mentes.Grap!=4))for(f=0;f<256;f++)
      setcol(f,(sin(f*M_PI*2/64)*31.0+31),(sin(f*M_PI*2/128)*31.0+31),(sin(f*M_PI*2/256)*31.0+31));
   else for(f=0;f<256;f++)setcol(f,Pal2[f].Blue>>2,Pal2[f].Green>>2,Pal2[f].Red>>2);
   for(f=0;f<46;f++){
      Ell[f].Ex=Kezdx[f];
      Ell[f].Ey=Kezdy[f];
      Ell[f].El=1;
      Ell[f].Tm=0;
      if(f<2){Ell[f].Kep=Elan[0][0];Ell[f].Pn=50;Ell[f].Tp=0;}
      if(f>1&&f<8){Ell[f].Kep=Elan[1][0];Ell[f].Pn=30;Ell[f].Tp=1;}
      if(f>7&&f<16){Ell[f].Kep=Elan[2][0];Ell[f].Pn=20;Ell[f].Tp=2;}
      if(f>15){Ell[f].Kep=Elan[3][0];Ell[f].Pn=10;Ell[f].Tp=3;}
   }
   for(f=0;f<9;f++)H_f[f].El=0;
   for(f=0;f<20;f++)E_f[f].El=0;
   Ell_Szam=46;
   Elix=1;
   return;
}
// Maga a j t‚k
void fo_prg(char Mode)
{
   char Ok=2,Kf=1;
   int f,n,r;
   int Rx=0,Ry=0,Rs=0;
   KeyHit=0;
   while(Ok){
      wait_sinc();
      control(Mode);
      if(Key[1])Ok=0;
      if(!Mode)if(KeyHit)Ok=0;
      if(Bal&&Hax>10&&!Rs){Hax-=Seb2;H1x+=Seb1;H2x+=Seb2;}
      if(Job&&Hax<294&&!Rs){Hax+=Seb2;H1x-=Seb1;H2x-=Seb2;}
      if(Akt_Page){
	 set_act_page(1);
	 if(Bon)plasma();
	 else show_hat(0);
	 if(!Rs)show_en(0);
	 show_ell(0);
	 show_tam(0);
	 show_lov1(0);
	 show_lov2(0);
	 szam_ir(64,228,Player[0].Pont,8,0);
	 szam_ir(224,228,Player[1].Pont,8,0);
	 show_rob(0);
	 if(Rs){show_big1(Rx,Ry,Rob2[6-Rs],48,48);Rs--;}
	 set_act_page(0);
	 }
      else{
	 set_act_page(0);
	 if(Bon)plasma();
	 else show_hat(240);
	 if(!Rs)show_en(240);
	 show_ell(240);
	 show_tam(240);
	 show_lov1(240);
	 show_lov2(240);
	 szam_ir(64,468,Player[0].Pont,8,0);
	 szam_ir(224,468,Player[1].Pont,8,0);
	 show_rob(240);
	 if(Rs)show_big1(Rx,Ry+240,Rob2[6-Rs],48,48);
	 set_act_page(1);
	 }
      if(Ok==2&&Mode){
	 if(!Akt_Page){
	    kiir(64,54,Sor001);
	    kiir(64,78,Sor002);
	    kiir(64,102,Sor003);
	    kiir(64,126,Sor004);
	    kiir(64,150,Sor005);
	    szam_ir(160,55,Pl+1,1,1);
	    szam_ir(160,79,Player[Pl].Szint,1,1);
	    szam_ir(160,103,Player[Pl].Kor,1,1);
	    szam_ir(160,127,Player[Pl].Elet,1,1);
	    szam_ir(146,151,Player[Pl].Pont,1,1);
	 }
	 else{
	    kiir(64,294,Sor001);
	    kiir(64,318,Sor002);
	    kiir(64,342,Sor003);
	    kiir(64,366,Sor004);
	    kiir(64,390,Sor005);
	    szam_ir(160,295,Pl+1,1,1);
	    szam_ir(160,319,Player[Pl].Szint,1,1);
	    szam_ir(160,343,Player[Pl].Kor,1,1);
	    szam_ir(160,367,Player[Pl].Elet,1,1);
	    szam_ir(146,391,Player[Pl].Pont,1,1);
	 }
	 var(200);
	 Ok=1;
      }
      if(Tuz&&!Rs&&Kf)
	for(f=0;f<9;f++)
	  if(!H_f[f].El){
	     H_f[f].El=1;
	     H_f[f].Hfx=Hax+7;
	     H_f[f].Hfy=197;
	     f=9;
//	     if(Mentes.DPort)sb_voice_DMA(Wav1,10754);
	     Tuz=0;
	  }
      if(Key[57])Kf=0;
      else Kf=1;
      for(f=0;f<46;f++)
	 for(n=0;n<9;n++)
	    if(H_f[n].El&&Ell[f].El==1&&
	       H_f[n].Hfx>Ell[f].Ex&&H_f[n].Hfx<Ell[f].Ex+12&&
	       H_f[n].Hfy>Ell[f].Ey&&H_f[n].Hfy<Ell[f].Ey+12){
	       Player[Pl].Pont+=Ell[f].Pn;
	       if(Ell[f].Tm)Player[Pl].Pont+=Ell[f].Pn;
	       Ell[f].El=10;
	       Ell[f].Tm=0;
	       H_f[n].El=0;
	    }
      for(f=0;f<20;f++)
	 if(E_f[f].El&&!Rs&&
	    E_f[f].Efx>Hax&&E_f[f].Efx<Hax+15&&
	    E_f[f].Efy>195&&E_f[f].Efy<224){
	    E_f[f].El=0;
	    if(Hax<48)Rx=0;
	    else if(Hax>272)Rx=272;
		 else Rx=Hax-16;
	    Ry=188;
	    Rs=6;
	    f=20;
	    Player[Pl].Elet--;
	    if(Mode)Ok=2;
	    else Ok=0;
	    if(!Player[Pl].Elet){
	       if(!Akt_Page)kiir(70,54,Sor006);
	       else kiir(70,294,Sor006);
	       var(200);
	       if((!Player[0].Elet)&&(!Player[1].Elet))Ok=0;
	    }
	    if(MaxPl){
	       if(Pl)Pl=0;
	       else Pl=1;
	       if(!Player[Pl].Elet)
		  if(Pl)Pl=0;
		  else Pl=1;
	    }
	 }
      r=random(Rand1);
      if(r==0&&random(2)){
	 tamad(0);
	 tamad(2);
	 tamad(3);
	 tamad(4);

      }else
	if(r==1&&random(2)){
	   tamad(1);
	   tamad(5);
	   tamad(6);
	   tamad(7);
	}else tamad(r);
      if(Ell_Szam<11)
	 for(f=0;f<46;f++)tamad(f);
      if(!Ell_Szam){
	 Bon=0;
	 if(Player[Pl].Kor==Player[Pl].Szint+1){
	    Bon=1;
	    slime_tolt("plasma.dat");
	    Player[Pl].Szint++;
	    Player[Pl].Kor=0;
	    if(!Akt_Page)kiir(50,54,Sor007);
	    else kiir(50,294,Sor007);
	    var(200);
	 }
	 else Player[Pl].Kor++;
	 if(Mode)Ok=2;
	 else Ok=0;
	 init2();
	 }
      if(Key[25]){
	 var(200);
	 Ok=1;
      }
      if(Bon){
	Teller++;
	if(Teller>30000)Teller=0;
      }
   }
   Bon=0;
   torol(0,0,320,480,0);
   slime_tolt("emb1.tga");
   for(f=0;f<256;f++)setcol(f,Pal1[f].Blue>>2,Pal1[f].Green>>2,Pal1[f].Red>>2);
   Bal=0;
   Job=0;
   Tuz=0;
   return;
}
// H tt‚r megjelen¡t‚se
// Hova - Melyik lapra tegye, 0 vagy 240
void show_hat(int Hova)
{
   switch(Mentes.Grap){
      case 1:{
	      show_big2(0,0+Hova,Hat1,464,100, 72-H1x,0, 72+H1x,0);
	      show_big2(0,60+Hova,Hat2,608,100,144-H2x,0,144+H2x,0);
	      break;
	     }
      case 2:{
	      show_big3(0,0+Hova,Hat1,464,100, 72-H1x,0, 72+H1x,0);
	      show_big3(0,60+Hova,Hat2,608,100,144-H2x,0,144+H2x,0);
	      break;
	     }
      case 3:{
	      show_big4(0,0+Hova,Hat1,464,100,72-H1x,0,72+H1x,0);
	      show_big4(0,60+Hova,Hat2,608,100,144-H2x,0,144+H2x,0);
	      break;
	     }
      case 4:{
	      torol(0,Hova,320,160+Hova,0);
	      break;
	     }
   }
   torol(0,160+Hova,320,228+Hova,0);
   show_big1(0,228+Hova,Hat3,64,12);
   show_big1(160,228+Hova,Hat4,64,12);
   return;
}
// Rak‚ta megjelen¡t‚se
// Hova - Melyik lapra tegye, 0 vagy 240
void show_en(int Hova)
{
   char f;
   if(LSzam<3)show_big1(Hax,200+Hova,Hajo[0],15,24);
   else show_big1(Hax,200+Hova,Hajo[1],15,24);
   for(f=0;f<9;f++)
      if(!H_f[f].El){show_litl(Hax+7,197+Hova,Lov1,1,3);f=9;}
   return;
}
// Ellenf‚l megjelen¡t‚se
// Hova - Melyik lapra tegye, 0 vagy 240
void show_ell(int Hova)
{
   char f;
   for(f=0;f<46;f++)
      if(Ell[f].El==1&&!Ell[f].Tm)
	 show_big1(Ell[f].Ex,Ell[f].Ey+Hova,Ell[f].Kep,12,12);
   if(Lep>2)Ir=-1;
   if(Lep<1)Ir=+1;
   if(!LSzam)Lep+=Ir;
   for(f=0;f<46;f++){
      if(!Ell[f].Tm)Ell[f].Kep=Elan[Ell[f].Tp][Lep];
      if(!LSzam)
	if(!Ell[f].Tm)Ell[f].Ex+=Elir;
	else Ell[f].Tx+=Elir;
   }
   if(Elix>76)Elir=-Seb1;
   if(Elix<-76)Elir=+Seb1;
   if(!LSzam)Elix+=Elir;
   LSzam++;
   if(LSzam>6)LSzam=0;
   return;
}
// T mad¢ ellenfelek
// Hova - Melyik lapra tegye, 0 vagy 240
void show_tam(int Hova)
{
   int f,n,r;
   for(f=0;f<46;f++){
      if(Ell[f].Tm==4){
	 Ell[f].Tm=0;
	 Ell[f].Ex=Ell[f].Tx;
	 Ell[f].Ey=Ell[f].Ty;
      }
      if(Ell[f].Tm==3){
	 if(Ell[f].Ex<300&&Ell[f].Ex>20)Ell[f].Ex+=Ell[f].Lx;
	 else Ell[f].Tm=4;
	 if(Ell[f].Ey<216)Ell[f].Ey+=Ell[f].Ly;
	 else Ell[f].Tm=4;
      }
      if(Ell[f].Tm==2){
	 Ell[f].Lx=(Hax+7-Ell[f].Ex)/40+Seb1-1;
	 Ell[f].Ly=(200-Ell[f].Ey)/50+Seb1-1;
	 if(Ell[f].Lx>0)Ell[f].Fs=8;
	 if(Ell[f].Lx>2)Ell[f].Fs=7;
	 if(Ell[f].Lx<0)Ell[f].Fs=11;
	 if(Ell[f].Lx<-2)Ell[f].Fs=10;
	 if(!Ell[f].Lx)Ell[f].Fs=9;
	 Ell[f].Kep=Elan[Ell[f].Tp][Ell[f].Fs];
	 Ell[f].Tm=3;
      }
      if(Ell[f].Tm==1)
	if(Ell[f].Fs<12){
	   Ell[f].Fs++;
	   Ell[f].Ex+=Tam_x[Ell[f].Fs];
	   Ell[f].Ey+=Tam_x[Ell[f].Fs];
	   Ell[f].Kep=Elan[Ell[f].Tp][Tam_f[Ell[f].Fs]];
	}
	else Ell[f].Tm=2;
      if(Ell[f].Tm)
	 show_big1(Ell[f].Ex,Ell[f].Ey+Hova,Ell[f].Kep,12,12);
      r=random(Rand2);
      if(r<10&&Ell[f].Tm)
	 for(n=0;n<20;n++)
	    if(!E_f[n].El){
	       E_f[n].El=1;
	       E_f[n].Efx=Ell[f].Ex+5;
	       E_f[n].Efy=Ell[f].Ey+6;
	       n=20;
//	       if(Mentes.DPort)sb_voice_DMA(Wav2,17362);
	    }
   }
   return;
}
// L”ved‚k megjelen¡t‚se
// Hova - Melyik lapra tegye, 0 vagy 240
void show_lov1(int Hova)
{
   int f;
   for(f=0;f<9;f++)
      if(H_f[f].El)
	 if(H_f[f].Hfy>Seb3){
	    H_f[f].Hfy-=Seb3;
	    show_litl(H_f[f].Hfx,H_f[f].Hfy+Hova,Lov1,1,3);
	    }
	 else H_f[f].El=0;
   return;
}
// Bomba megjelen¡t‚se
// Hova - Melyik lapra tegye, 0 vagy 240
void show_lov2(int Hova)
{
   int f;
   for(f=0;f<20;f++)
      if(E_f[f].El)
	 if(E_f[f].Efy<220){
	    E_f[f].Efy+=Seb3;
	    show_litl(E_f[f].Efx,E_f[f].Efy+Hova,Lov2,2,3);
	    }
	 else E_f[f].El=0;
   return;
}
// Ellens‚g robban s
// Hova - Melyik lapra tegye, 0 vagy 240
void show_rob(int Hova)
{
   int f,n;
   for(f=0;f<46;f++)
       if(Ell[f].El>9){
	  if(Ell[f].Ey)n=6;
	  else n=0;
	  show_big1(Ell[f].Ex-6,Ell[f].Ey-n+Hova,Rob1[Ell[f].El-10],24,24);
	  if(Akt_Page)Ell[f].El++;
	  if(Ell[f].El==13){Ell[f].El=0;Ell_Szam--;}
       }
   return;
}
// Tamado ellenfelek
// tam - a tamodo ellenfel szama
void tamad(int tam)
{
  if(tam<46)
    if(Ell[tam].El==1&&!Ell[tam].Tm){
       Ell[tam].Tm=1;
       Ell[tam].Fs=0;
       Ell[tam].Tx=Ell[tam].Ex;
       Ell[tam].Ty=Ell[tam].Ey;
    }
  return;
}

void control(char Mode)
{
   int r;
   if(Mode)
      switch(Mentes.Control){
	 case 1:{
		 if(Key[75])Bal=1;
		 else Bal=0;
		 if(Key[77])Job=1;
		 else Job=0;
		 if(Key[57])Tuz=1;
		 break;
		}
	 case 2:{
		 break;
		}
	 case 3:{
		 break;
		}
   }
   else{
      if(Bal)Bal--;
      else if(Job)Job--;
	   else{
	      r=random(50);
	      switch(r){
		 case 0:{
			 Bal=random(50);
			 break;
			}
		 case 1:{
			 Job=random(50);
			 break;
			}
	      }
      }
      r=random(10);
      if(r==5)Tuz=1;
   }
   return;
}