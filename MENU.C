////////////////////////////
//			  //
// Space Fight I.         //
// (C) 1995 Kov cs Roland //
//			  //
// Menk.		  //
//                        //
////////////////////////////
// F”menu
void fomenu(void)
{
  int f;
  unsigned int i;
  char Ok=1,Men=0,Ts=0,s=0,m=5,Demo=200;
  i=Ido;
  while(Ok){
     if(Key[1]&&!m)Ok=0;
     if(Key[72]&&!m){
	if(!Men)Men=4;
	else Men--;
	m=5;
     }
     if(Key[80]&&!m){
	if(Men==4)Men=0;
	else Men++;
	m=5;
     }
     if(Key[28]||Key[57]&&!m){
       switch(Men){
	  case 0:{
		  Bon=0;
		  Player[0].Elet=MaxEl;
		  Player[0].Pont=0;
		  Player[0].Szint=1;
		  Player[0].Kor=1;
		  if(MaxPl)Player[1].Elet=MaxEl;
		  else Player[1].Elet=0;
		  Player[1].Pont=0;
		  Player[1].Szint=1;
		  Player[1].Kor=1;
		  torol(0,0,320,480,0);
		  init2();
		  fo_prg(1);
		  break;
		 }
	  case 1:{
		  menu1();
		  break;
		 }
	  case 2:{
		  menu2();
		  break;
		 }
	  case 3:{
		  menu3();
		  break;
		 }
	  case 4:Ok=0;
       }
       m=5;
       Demo=200;
     }
     if(Akt_Page){
	set_act_page(1);
	texscreen();
	show_big1(80,10,Emb2,160,84);
	show_big1(270,190,Emb3,26,32);
	if(!(Men==0&&s))kiir(60,104,Sor101);
	if(!(Men==1&&s))kiir(60,128,Sor102);
	if(!(Men==2&&s))kiir(60,152,Sor103);
	if(!(Men==3&&s))kiir(60,176,Sor104);
	if(!(Men==4&&s))kiir(60,200,Sor105);
	set_act_page(0);
     }
     else{
	set_act_page(0);
	texscreen();
	show_big1(80,250,Emb2,160,84);
	show_big1(270,430,Emb3,26,32);
	if(!(Men==0&&s))kiir(60,344,Sor101);
	if(!(Men==1&&s))kiir(60,368,Sor102);
	if(!(Men==2&&s))kiir(60,392,Sor103);
	if(!(Men==3&&s))kiir(60,416,Sor104);
	if(!(Men==4&&s))kiir(60,440,Sor105);
	set_act_page(1);
     }
     Deg++;
     if(Deg>1023)Deg=0;
     Xo+=(1253L<<5);
     Yo+=(1534L<<5);
     wait_sinc();
     if(i!=Ido){i=Ido;Ts++;if(m)m--;Demo--;}
     if(Ts>5){
       Ts=0;
       if(s)s=0;
       else s=1;
     }
     if(!Demo){
	Bon=random(2);
	if(Bon)slime_tolt("plasma.dat");
	Player[0].Elet=MaxEl;
	Player[0].Pont=0;
	Player[0].Szint=1;
	Player[0].Kor=1;
	if(MaxPl)Player[1].Elet=MaxEl;
	else Player[1].Elet=0;
	Player[1].Pont=0;
	Player[1].Szint=1;
	Player[1].Kor=1;
	torol(0,0,320,480,0);
	init2();
	fo_prg(0);
	Demo=200;
     }
  }
  return;
}
// Els” Menu
void menu1(void)
{
  unsigned int i;
  char Ok=1,Ts=0,s=0,m=5;
  i=Ido;
  while(Ok){
     if((Key[1]||Key[28]||Key[57])&&!m)Ok=0;
     if(Akt_Page){
	set_act_page(1);
	texscreen();
	kiir(80,10,Sor102);
	kiir(6,30,Mentes.Nev9);
	szam_ir(200,31,Mentes.Pont9,8,1);
	kiir(6,50,Mentes.Nev8);
	szam_ir(200,51,Mentes.Pont8,8,1);
	kiir(6,70,Mentes.Nev7);
	szam_ir(200,71,Mentes.Pont7,8,1);
	kiir(6,90,Mentes.Nev6);
	szam_ir(200,91,Mentes.Pont6,8,1);
	kiir(6,110,Mentes.Nev5);
	szam_ir(200,111,Mentes.Pont5,8,1);
	kiir(6,130,Mentes.Nev5);
	szam_ir(200,131,Mentes.Pont4,8,1);
	kiir(6,150,Mentes.Nev4);
	szam_ir(200,151,Mentes.Pont3,8,1);
	kiir(6,170,Mentes.Nev3);
	szam_ir(200,171,Mentes.Pont2,8,1);
	kiir(6,190,Mentes.Nev2);
	szam_ir(200,191,Mentes.Pont1,8,1);
	kiir(6,210,Mentes.Nev1);
	szam_ir(200,211,Mentes.Pont0,8,1);
	set_act_page(0);
     }
     else{
	set_act_page(0);
	texscreen();
	kiir(80,250,Sor102);
	kiir(6,270,Mentes.Nev9);
	szam_ir(200,271,Mentes.Pont9,8,1);
	kiir(6,290,Mentes.Nev8);
	szam_ir(200,291,Mentes.Pont8,8,1);
	kiir(6,310,Mentes.Nev7);
	szam_ir(200,311,Mentes.Pont7,8,1);
	kiir(6,330,Mentes.Nev6);
	szam_ir(200,331,Mentes.Pont6,8,1);
	kiir(6,350,Mentes.Nev5);
	szam_ir(200,351,Mentes.Pont5,8,1);
	kiir(6,370,Mentes.Nev4);
	szam_ir(200,371,Mentes.Pont4,8,1);
	kiir(6,390,Mentes.Nev3);
	szam_ir(200,391,Mentes.Pont3,8,1);
	kiir(6,410,Mentes.Nev2);
	szam_ir(200,411,Mentes.Pont2,8,1);
	kiir(6,430,Mentes.Nev1);
	szam_ir(200,431,Mentes.Pont1,8,1);
	kiir(6,450,Mentes.Nev0);
	szam_ir(200,451,Mentes.Pont0,8,1);
	set_act_page(1);
     }
     Deg++;
     if(Deg>1023)Deg=0;
     Xo+=(1253L<<5);
     Yo+=(1534L<<5);
     if(i!=Ido){i=Ido;Ts++;if(m)m--;}
     if(Ts>5){
       Ts=0;
       if(s)s=0;
       else s=1;
     }
     wait_sinc();
  }
  return;
}
// M sodik Menu
void menu2(void)
{
  unsigned int i;
  char Ok=1,Men=0,Ts=0,s=0,m=5;
  FILE *Fp;
  i=Ido;
  while(Ok){
     if(Key[1]&&!m)Ok=0;
     if(Key[72]&&!m){
	if(!Men)Men=6;
	else Men--;
	m=5;
     }
     if(Key[80]&&!m){
	if(Men==6)Men=0;
	else Men++;
	m=5;
     }
     if((Key[28]||Key[57])&&!m){
       switch(Men){
	  case 0:{
		  if(MaxPl)MaxPl=0;
		  else MaxPl=1;
		  break;
		 }
	  case 1:{
		  if(Mentes.Level<3)Mentes.Level++;
		  else Mentes.Level=1;
		  break;
		 }
	  case 2:{
		  if(Mentes.Control<3)Mentes.Control++;
		  else Mentes.Control=1;
		  break;
		 }
	  case 3:{
		  if(Mentes.Sound<3)Mentes.Sound++;
		  else Mentes.Sound=1;
		  break;
		 }
	  case 4:{
		  if(Mentes.Music==2){
		     oldhandler3=oldhandler2;
		     stop_HSC();
		     setvect(Tmer,idozites);
		  }
		  if(Mentes.Music<3)Mentes.Music++;
		  else Mentes.Music=1;
		  if(Mentes.Music==2){
		     init_HSC();
		     play_HSC();
		     oldhandler3=getvect(Tmer);
		     setvect(Tmer,idozites);
		  }
		  break;
		 }
	  case 5:{
		  if(Mentes.Grap<4)Mentes.Grap++;
		  else Mentes.Grap=1;
		  break;
		 }
	  case 6:{
		  Ok=0;
		  break;
		 }
       }
       m=10;
     }
     wait_sinc();
     if(Akt_Page){
	set_act_page(1);
	texscreen();
	kiir(110,5,Sor103);
	if(!(Men==0&&s)){
	   kiir(10,35,Sor001);
	   if(!MaxPl)kiir(136,35,Sor301);
	   else kiir(136,35,Sor302);
	   }
	if(!(Men==1&&s)){
	   kiir(10,65,Sor002);
	   if(Mentes.Level==1)kiir(136,65,Sor303);
	   if(Mentes.Level==2)kiir(136,65,Sor304);
	   if(Mentes.Level==3)kiir(136,65,Sor305);
	}
	if(!(Men==2&&s)){
	   kiir(10,95,Sor306);
	   if(Mentes.Control==1)kiir(136,95,Sor307);
	   if(Mentes.Control==2)kiir(136,95,Sor308);
	   if(Mentes.Control==3)kiir(136,95,Sor309);
	}
	if(!(Men==3&&s)){
	   kiir(10,125,Sor310);
	   if(Mentes.Sound==1)kiir(136,125,Sor311);
	   if(Mentes.Sound==2)kiir(136,125,Sor312);
	   if(Mentes.Sound==3)kiir(136,125,Sor313);
	}
	if(!(Men==4&&s)){
	   kiir(10,155,Sor314);
	   if(Mentes.Music==1)kiir(136,155,Sor311);
	   if(Mentes.Music==2)kiir(136,155,Sor315);
	   if(Mentes.Music==3)kiir(136,155,Sor316);
	}
	if(!(Men==5&&s)){
	   kiir(10,185,Sor317);
	   if(Mentes.Grap==1)kiir(136,185,Sor318);
	   if(Mentes.Grap==2)kiir(136,185,Sor319);
	   if(Mentes.Grap==3)kiir(136,185,Sor320);
	   if(Mentes.Grap==4)kiir(136,185,Sor321);
	}
	if(!(Men==6&&s))kiir(10,215,Sor105);
	set_act_page(0);
     }
     else{
	set_act_page(0);
	texscreen();
	kiir(110,245,Sor103);
	if(!(Men==0&&s)){
	   kiir(10,275,Sor001);
	   if(!MaxPl)kiir(136,275,Sor301);
	   else kiir(136,275,Sor302);
	   }
	if(!(Men==1&&s)){
	   kiir(10,305,Sor002);
	   if(Mentes.Level==1)kiir(136,305,Sor303);
	   if(Mentes.Level==2)kiir(136,305,Sor304);
	   if(Mentes.Level==3)kiir(136,305,Sor305);
	}
	if(!(Men==2&&s)){
	   kiir(10,335,Sor306);
	   if(Mentes.Control==1)kiir(136,335,Sor307);
	   if(Mentes.Control==2)kiir(136,335,Sor308);
	   if(Mentes.Control==3)kiir(136,335,Sor309);
	}
	if(!(Men==3&&s)){
	   kiir(10,365,Sor310);
	   if(Mentes.Sound==1)kiir(136,365,Sor311);
	   if(Mentes.Sound==2)kiir(136,365,Sor312);
	   if(Mentes.Sound==3)kiir(136,365,Sor313);
	}
	if(!(Men==4&&s)){
	   kiir(10,395,Sor314);
	   if(Mentes.Music==1)kiir(136,395,Sor311);
	   if(Mentes.Music==2)kiir(136,395,Sor315);
	   if(Mentes.Music==3)kiir(136,395,Sor316);
	}
	if(!(Men==5&&s)){
	   kiir(10,425,Sor317);
	   if(Mentes.Grap==1)kiir(136,425,Sor318);
	   if(Mentes.Grap==2)kiir(136,425,Sor319);
	   if(Mentes.Grap==3)kiir(136,425,Sor320);
	   if(Mentes.Grap==4)kiir(136,425,Sor321);
	}
	if(!(Men==6&&s))kiir(10,455,Sor105);
	set_act_page(1);
     }
     Deg++;
     if(Deg>1023)Deg=0;
     Xo+=(1253L<<5);
     Yo+=(1534L<<5);
     if(i!=Ido){i=Ido;Ts++;if(m)m--;}
     if(Ts>5){
       Ts=0;
       if(s)s=0;
       else s=1;
     }
  }
  Fp=fopen("sf1.cfg","wb");
  fwrite(&Mentes,sizeof(Mentes),1,Fp);
  fclose(Fp);
  return;
}
// Harmadik Menu
void menu3(void)
{
  char Ok=1;
  while(Ok){
     if(Key[1])Ok=0;
     if(Key[28]||Key[57])Ok=0;
     if(Akt_Page){
	set_act_page(1);
	texscreen();
	show_big1(80,10,Emb2,160,84);
	show_big1(270,190,Emb3,26,32);
	set_act_page(0);
     }
     else{
	set_act_page(0);
	texscreen();
	show_big1(80,250,Emb2,160,84);
	show_big1(270,430,Emb3,26,32);
	set_act_page(1);
     }
     Deg++;
     if(Deg>1023)Deg=0;
     Xo+=(1253L<<5);
     Yo+=(1534L<<5);
     wait_sinc();
  }
  return;
}
