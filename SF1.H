////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov†cs Roland //
//			  //
// Header f†jl.		  //
//                        //
////////////////////////////
// defin°ci¢k
#define Video 0xa000
#define	Fd2r  3.1415926535/512
#define Xsize 320/4
#define	Ysize 240
#define MaxEl 5
// konstansok
const Bill=0x09;
const Tmer=0x1c;
// stukt£r†k
typedef struct{
   unsigned char Red;
   unsigned char Green;
   unsigned char Blue;
}Rgbpal;
typedef struct{
   unsigned int Hfx;
   unsigned int Hfy;
   unsigned char El;
}En_fire;
typedef struct{
   int Lx;
   int Ly;
   int Ex;
   int Ey;
   int Tx;
   int Ty;
   unsigned char Tp;
   unsigned char El;
   unsigned char Tm;
   unsigned char Pn;
   unsigned char Fs;
   unsigned char far *Kep;
}Ellenseg;
typedef struct{
   unsigned int Efx;
   unsigned int Efy;
   unsigned char El;
}Ell_fire;
typedef struct{
   unsigned int DPort;
   unsigned int MPort;
   unsigned int SBIRQ;
   unsigned int SBDMA;
   unsigned char Music;
   unsigned char Sound;
   unsigned char Control;
   unsigned char Level;
   unsigned char Grap;
   unsigned char Nev0[14];
   unsigned char Nev1[14];
   unsigned char Nev2[14];
   unsigned char Nev3[14];
   unsigned char Nev4[14];
   unsigned char Nev5[14];
   unsigned char Nev6[14];
   unsigned char Nev7[14];
   unsigned char Nev8[14];
   unsigned char Nev9[14];
   long Pont0;
   long Pont1;
   long Pont2;
   long Pont3;
   long Pont4;
   long Pont5;
   long Pont6;
   long Pont7;
   long Pont8;
   long Pont9;
   char bal;
   char jobb;
   char tuz;
}Config;
typedef struct{
   char Szint;
   char Kor;
   char Elet;
   long Pont;
}Jatekos;
// tîmbîk
char huge *Slime;
unsigned char far Szam[10][144],
		  Elan[4][15][144],
		  Rob1[4][576],
		  Rob2[6][2304],
		  Hajo[2][360],
		  Betuk[42][252],
		  Key[128];
unsigned char far *Hat1,
		  *Hat2,
		  *Hat3,
		  *Hat4,
		  *Emb2,
		  *Emb3;
unsigned char far Lov1[3]={250,250,250};
unsigned char far Lov2[6]={246,246,246,246,246,246};
unsigned char     Data_480[18]={0x0b,0x3e,0x00,0x40,0x00,0x00,0x00,0x00,0x00,
				0x00,0xea,0x8c,0xdf,0x28,0x00,0xe7,0x04,0xe3};
unsigned char     Data_360[ 6]={0x6b,0x59,0x5a,0x8e,0x5e,0x8a};
unsigned int      Kezdx[46]={130,175,115,130,145,160,175,190,100,115,130,145,
			     160,175,190,205,85,100,115,130,145,160,175,190,
			     205,220,85,100,115,130,145,160,175,190,205,220,
			     85,100,115,130,145,160,175,190,205,220};
unsigned int      Kezdy[46]={0,0,15,15,15,15,15,15,30,30,30,30,30,30,30,30,
			     45,45,45,45,45,45,45,45,45,45,60,60,60,60,60,60,
			     60,60,60,60,75,75,75,75,75,75,75,75,75,75};
unsigned int *AddTmb;
char far *Wav1,*Wav2;
int  Tam_x[14]={1,1,1,1,0,0,0,0,0,0,1,1,1,1};
int  Tam_y[14]={1,1,1,1,0,0,0,0,0,0,1,1,1,1};
char Tam_f[14]={3,3,4,4,5,5,6,6,7,7,8,8,9,9};
// Strukt£r†k
Rgbpal   Pal1[256],Pal2[256];
En_fire	 H_f[9];
Ellenseg Ell[46];
Ell_fire E_f[20];
Jatekos  Player[2];
Config   Mentes;
// glob†lis v†ltoz¢k
unsigned char Akt_Vid_Mod=0,Akt_Page=0,DMA_Ok=0;
unsigned int Ido,Deg=0;
int Hax=152,
    H1x=0,
    H2x=0,
    Elix=1;
int Rand1=3000,
    Rand2=500;
char Lep=1,
     Ir=1,
     Elir=1,
     LSzam=0,
     Ell_Szam=46;
char KeyHit=0,
     Seb1=1,
     Seb2=2,
     Seb3=4,
     MaxPl=0,
     Pl=0,
     Bon=0,
     Bal=0,
     Job=0,
     Tuz=0;
long Xo=0,Yo=0,Dx=0,Dy=0;
float Teller=0;
// fÅggvÇnyek
// MAIN.C
void main(int argc,char *argv);
char init1(void);
void init2(void);
void fo_prg(char Mode);
void show_hat(int Hova);
void show_en(int Hova);
void show_ell(int Hova);
void show_tam(int Hova);
void show_lov1(int Hova);
void show_lov2(int Hova);
void show_rob(int Hova);
void tamad(int tam);
void control(char Mode);
// PLASMA.C
void texscreen(void);
void plasma(void);
// MENU.C
void fomenu(void);
void menu1(void);
void menu2(void);
void menu3(void);
// OTHERS.C
char tolt(void);
void interrupt idozites(void);
void interrupt billentyu(void);
void kiir(int Sz_x,int Sz_y,unsigned char *Szoveg);
void szam_ir(int Sz_x,int Sz_y,long Szm,char Nul,char Szt);
char slime_tolt(char *nev);
void var(unsigned int v);
// GRAPHICS.C
char get_vid_mod(void);
void set_vid1_mod(void);
void set_vid2_mod(void);
void end_vid_mod(unsigned char Mod);
void set_act_page(char page);
void setcol(char ColReg, char Red, char Green, char Blue);
void wait_sinc(void);
void torol(int Xk,int Yk,int Xv,int Yv,char Color);
void show_litl(int X,int Y,unsigned char *Koc,int Sx,int Sy);
void show_big1(int X,int Y,unsigned char *Koc,int Sx,int Sy);
void show_big2(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy);
void show_big3(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy);
void show_big4(int X,int Y,unsigned char *Koc,int Sx,int Sy,int Kx,int Ky,int Vx,int Vy);
//SOUND
char init_HSC(void);
void play_HSC(void);
void stop_HSC(void);
char reset_DSP(int Port);
char reset_MPU401(int Port);
char set_UART(int Port);
void send_MIDI(int Port,unsigned char Midi);
void interrupt sbhandler(void);
unsigned int sb_sample_rate(unsigned int Rate);
void sb_voice(int State);
void writedac(int Com);
//EgyÇb deklar†ci¢k
void far extern pascal INIT_PROC();
void interrupt (*oldhandler1)(void);
void interrupt (*oldhandler2)(void);
void interrupt (*oldhandler3)(void);
void interrupt (*OldIRQ)(void);
// Åzenetek
char detect1[21]={"ADlib card detected."};
char detect2[33]={"MPU-401 MIDI interface detected."};
char detect3[25]={"DSP processor detected."};


