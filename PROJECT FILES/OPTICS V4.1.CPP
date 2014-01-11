#include<iostream.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<process.h>
#include<time.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include"cmouse.cpp"
#include"bitmaps.cpp"

char pic[] = "photon.bmp";

int gd = DETECT,gm,sel = 0;

struct point{int x,y;};

struct obj
{  
 char *name;
 point p;
 int type;
 float r,r1,r2,d,f;
};

obj lens[6]= {
	      {"Convex",{120,240,},1,0,60,60,1.5,0},
	      {"Concave",{220,240,},2,0,60,60,1.5,0},
	      {"Left Plano Convex",{320,240,},3,60,0,0,1.5,0},
	      {"Right Plano Convex",{420,240,},4,60,0,0,1.5,0},
	      {"Left Plano Concave",{510,240,},5,60,0,0,1.5,0},
	      {"Right Plano Concave",{600,240,},6,60,0,0,1.5,0},
	     };

obj lensf[6];

void lensfinit()
{
 for(int i=0;i<6;i++)lensf[i].type=0;
}

int mousechk(int x1,int y1,int x2,int y2)
{
 if((MM.x>x1+2&&MM.x<x2-2)&&(MM.y>y1+1&&MM.y<y2-1))return 1;
 else return 0;
}

void window(int l,int h,int x1,int y1,int col,char *string,int flag)
{       
 if((getpixel(x1-l/2+6,y1-h/2+5)!=col)&&flag == 0)
 {setcolor(7);rectangle(x1-l/2+3,y1-h/2+2,x1+l/2-3,y1+h/2-5);
 if(col==3||col==6)col =1;
 setfillstyle(SOLID_FILL,col);floodfill(x1,y1,7);setcolor(8);
 rectangle(x1-l/2+3,y1-h/2+2,x1+l/2-3,y1+h/2-5);
 line(x1-l/2+3,y1+h/2-5,x1-l/2,y1+h/2);
 line(x1+l/2-3,y1+h/2-5,x1+l/2,y1+h/2);
 line(x1-l/2,y1+h/2,x1-l/2,y1-h/2);
 line(x1+l/2,y1+h/2,x1+l/2,y1-h/2);
 line(x1-l/2,y1-h/2,x1+l/2,y1-h/2);
 setfillstyle(SOLID_FILL,8);floodfill(x1,y1-h/2+1,8);
 setcolor(7);line(x1-l/2,y1+h/2,x1+l/2,y1+h/2);
 rectangle(x1-l/2+3,y1-h/2+2,x1+l/2-3,y1+h/2-5);
 line(x1-l/2+3,y1+h/2-5,x1-l/2,y1+h/2);
 line(x1+l/2-3,y1+h/2-5,x1+l/2,y1+h/2);
 setfillstyle(SOLID_FILL,7);floodfill(x1,y1+h/2-1,7);
 }settextstyle(DEFAULT_FONT,HORIZ_DIR,2);setcolor(10);
 if((MM.x>x1-l/2+1&&MM.x<x1+l/2-1)&&(MM.y>y1-h/2+1&&MM.y<y1+h/2-1))setcolor(6);
 outtextxy(x1-(strlen(string)*7.5),y1-9,string);
}

void erase(int x1,int y1,int x2,int y2)
{
 setcolor(0);
 for(int i=x1;i<=x2;i++)line(i,y1,i,y2);
}

void drawlens(obj obj1,int t)
{
 float l1,l2,a1,a2;
 int col;
 if(obj1.d>=0.5&&obj1.d<1.0)col = 2;
 if(obj1.d>1.0&&obj1.d<1.25)col = 11;
 if(obj1.d>=1.25&&obj1.d<1.5)col = 9;
 if(obj1.d>=1.5&&obj1.d<1.75)col = 1;
 if(obj1.d>=1.75&&obj1.d<=2.0)col = 4;
 if(t==1) {obj1.p.x = 125;obj1.p.y=115;}
 switch(obj1.type)
 {
   case 1:
   setcolor(15);
   l1 = pow(obj1.r1*obj1.r1-2500.0,0.5);
   l2 = pow(obj1.r2*obj1.r2-2500.0,0.5);
   a1 = 180.0/3.14*atan(50.0/l1);
   a2 = 180.0/3.14*atan(50.0/l2);
   arc(obj1.p.x+l1,obj1.p.y,180.0-a1,180.0+a1,obj1.r1);
   arc(obj1.p.x-l2,obj1.p.y,0-a2,0+a2,obj1.r2);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x,obj1.p.y,15);
   break;

   case 2:
   setcolor(15);
   a1 = 180.0/3.14*asin(50.0/obj1.r1);
   a2 = 180.0/3.14*asin(50.0/obj1.r2);
   arc(obj1.p.x-obj1.r1-5.0,obj1.p.y,-a1,a1,obj1.r1);
   arc(obj1.p.x+obj1.r2+5.0,obj1.p.y,180.0-a2,180.0+a2,obj1.r2);
   l1 = 5.0+obj1.r1-obj1.r1*cos(3.14/180.0*a1);
   l2 = 5.0+obj1.r2-obj1.r2*cos(3.14/180.0*a2);
   line(obj1.p.x-l1,obj1.p.y-50+1,obj1.p.x+l2,obj1.p.y-50+1);
   line(obj1.p.x-l1,obj1.p.y+50-1,obj1.p.x+l2,obj1.p.y+50-1);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x,obj1.p.y,15);
   break;

   case 3:
   setcolor(15);
   l2 = pow(obj1.r*obj1.r-2500.0,0.5);
   a2 = 180.0/3.14*atan(50.0/l2);
   arc(obj1.p.x+l2,obj1.p.y,180.0-a2,180.0+a2,obj1.r);
   line(obj1.p.x-1,obj1.p.y-50,obj1.p.x-1,obj1.p.y+50);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x-3,obj1.p.y,15);
   break;

   case 4:
   setcolor(15);
   l1 = pow(obj1.r*obj1.r-2500.0,0.5);
   a1 = 180.0/3.14*atan(50.0/l1);
   arc(obj1.p.x-l1,obj1.p.y,-a1,a1,obj1.r);
   line(obj1.p.x+1,obj1.p.y-50,obj1.p.x+1,obj1.p.y+50);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x+3,obj1.p.y,15);
   break;

   case 5:
   setcolor(15);
   a1 = 180.0/3.14*asin(50.0/obj1.r)+0.5;
   arc(obj1.p.x-obj1.r-5.0,obj1.p.y,-a1,a1,obj1.r);
   l1 = 5.0+obj1.r-obj1.r*cos(3.14/180.0*a1);
   line(obj1.p.x-l1,obj1.p.y-50+1,obj1.p.x,obj1.p.y-50+1);
   line(obj1.p.x-l1,obj1.p.y+50-1,obj1.p.x,obj1.p.y+50-1);
   line(obj1.p.x,obj1.p.y-50,obj1.p.x,obj1.p.y+50);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x-2,obj1.p.y,15);
   break;

   case 6:
   setcolor(15);
   a2 = 180.0/3.14*asin(50.0/obj1.r)+0.5;
   arc(obj1.p.x+obj1.r+5.0,obj1.p.y,180.0-a2,180.0+a2,obj1.r);
   l2 = 5.0+obj1.r-obj1.r*cos(3.14/180.0*a2);
   line(obj1.p.x,obj1.p.y-50+1,obj1.p.x+l2,obj1.p.y-50+1);
   line(obj1.p.x,obj1.p.y+50-1,obj1.p.x+l2,obj1.p.y+50-1);
   line(obj1.p.x,obj1.p.y-50,obj1.p.x,obj1.p.y+50);
   setfillstyle(SOLID_FILL,col);
   floodfill(obj1.p.x+2,obj1.p.y,15);
   break;

   default: break;
 }
}


int mainmenu()
{
 int flag = 0,con = 0,i;
 cleardevice();
 setcolor(10);
 rectangle(10,10,getmaxx()-10,getmaxy()-10);
 rectangle(15,15,getmaxx()-15,getmaxy()-15);
 setfillstyle(SOLID_FILL,12);floodfill(12,12,10);
 window(310,30,320,120,1,"DESIGNER",flag);
 window(310,30,320,200,1,"CREDITS ",flag);
 window(310,30,320,280,1,"QUIT",flag);
 MM.reset();directvideo = 0;MM.show();
 while(con == 0)
 {
  window(310,30,320,120,1,"DESIGNER",flag);
  window(310,30,320,200,1,"CREDITS ",flag);
  window(310,30,320,280,1,"QUIT",flag);
  MM.autoEventUpdate();
  if(MM.leftPressed)
  {
   if((MM.x>320-310/2+1&&MM.x<320+310/2-1)&&(MM.y>120-30/2+1&&MM.y<120+30/2-1))
   {
    con = 1;
    for(i = 0;i<5;i++)
    {
     window(310,30,320,120,4,"DESIGNER",0);delay(50);
     window(310,30,320,120,14,"DESIGNER",0);delay(50);
    }
    delay(1000);
   }
   if((MM.x>320-310/2+1&&MM.x<320+310/2-1)&&(MM.y>200-30/2+1&&MM.y<200+30/2-1))
   {
    con = 5;
    for(i = 0;i<5;i++)
    {
     window(310,30,320,200,4,"CREDITS",0);delay(50);
     window(310,30,320,200,14,"CREDITS",0);delay(50);
    }
    delay(1000);
   }
   if((MM.x>320-310/2+1&&MM.x<320+310/2-1)&&(MM.y>280-30/2+1&&MM.y<280+30/2-1))
   {
    con = 6;
    for(i = 0;i<5;i++)
    {
     window(310,30,320,280,4,"QUIT",0);delay(50);
     window(310,30,320,280,14,"QUIT",0);delay(50);
    }
    delay(1000);
   }
  }
 }
 cleardevice();return con;
}

void selectlens(obj obj1,int &flag,char help[][50],int &sel,int mod)
{
 static int xval=50;
 int i;
 float a,d;
 if(mod==0)
 {
  switch(obj1.type)
  {
   case 1: case 2:
   gotoxy(36,3);puts(help[3]);
   gotoxy(37,4);cin>>a;
   while(a>600)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[4]);
    gotoxy(37,4);cin>>a;
   }
   while(xval>a)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);cout<<help[18]<<xval;
    gotoxy(37,4);cin>>a;
   }
   obj1.p.x=a;xval=a;

   gotoxy(36,3);puts(help[5]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r1=a;

   gotoxy(36,3);puts(help[6]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r2=a;

   gotoxy(36,3);puts(help[9]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>d;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[10]);
   } while((!(d>=0.5&&d<=2.0))||(d==1.0));
   gotoxy(36,3);puts(help[0]);
   gotoxy(36,3);puts(help[11]);
   gotoxy(37,4);puts(help[0]);
   obj1.d=d;

   lensf[sel] = obj1;
   break;

   case 3: case 4: case 5: case 6:
   gotoxy(36,3);puts(help[3]);
   gotoxy(37,4);cin>>a;
   while(a>600)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[4]);
    gotoxy(37,4);cin>>a;
   }
   while(xval>a)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);cout<<help[18]<<xval;
    gotoxy(37,4);cin>>a;
   }
   obj1.p.x=a; xval=a;

   gotoxy(36,3);puts(help[8]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r=a;

   gotoxy(36,3);puts(help[9]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>d;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[10]);
   } while((!(d>=0.5&&d<=2.0))||(d==1.0));
   gotoxy(36,3);puts(help[0]);
   gotoxy(36,3);puts(help[11]);
   gotoxy(37,4);puts(help[0]);
   obj1.d=d;

   lensf[sel] = obj1;
   break;
  }
  sel++;
 }
 flag = 1;
 if(mod!=0)
 {
  switch(obj1.type)
  {
   case 1: case 2:
   gotoxy(36,3);puts(help[3]);
   gotoxy(37,4);cin>>a;
   while(a>600)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[4]);
    gotoxy(37,4);cin>>a;
   }
   while(xval>a)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);cout<<help[18]<<xval;
    gotoxy(37,4);cin>>a;
   }
   obj1.p.x=a;xval=a;

   gotoxy(36,3);puts(help[5]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r1=a;

   gotoxy(36,3);puts(help[6]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r2=a;

   gotoxy(36,3);puts(help[9]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>d;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[10]);
   } while((!(d>=0.5&&d<=2.0))||(d==1.0));
   gotoxy(36,3);puts(help[0]);
   gotoxy(36,3);puts(help[17]);
   gotoxy(37,4);puts(help[0]);
   obj1.d=d;

   lensf[mod-1] = obj1;
   break;

   case 3: case 4: case 5: case 6:
   gotoxy(36,3);puts(help[3]);
   gotoxy(37,4);cin>>a;
   while(a>600)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[4]);
    gotoxy(37,4);cin>>a;
   }
   while(xval>a)
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);cout<<help[18]<<xval;
    gotoxy(37,4);cin>>a;
   }
   obj1.p.x=a; xval=a;

   gotoxy(36,3);puts(help[8]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>a;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[7]);
   } while(!(a>=60&&a<=200));
   gotoxy(36,3);puts(help[0]);
   obj1.r=a;

   gotoxy(36,3);puts(help[9]);
   do
   {
    gotoxy(37,4);puts(help[0]);
    gotoxy(37,4);cin>>d;
    gotoxy(36,3);puts(help[0]);
    gotoxy(36,3);puts(help[10]);
   } while((!(d>=0.5&&d<=2.0))||(d==1.0));
   gotoxy(36,3);puts(help[0]);
   gotoxy(36,3);puts(help[17]);
   gotoxy(37,4);puts(help[0]);
   obj1.d=d;

   lensf[mod-1] = obj1;
   break;
  }
 }
 for(i = 0;i<sel;i++)
 {
  gotoxy(5,i+15);
  cout<<i+1<<" ";
  puts(lensf[i].name);
 }
}

int designerinit()
{
 int i,flag = 1,quit = 0,ctr=0;
 sel = 0;
 char ch;
 char help[][50] = {"                                       \0",//0
		    "Click On Buttons to activate event     \0",//1
		    "Select a desired lens                  \0",//2
		    "Enter X coord of optical centre of lens\0",//3
		    "Sorry!!!Enter X coord below 600        \0",//4
		    "Enter Radius of First Interface        \0",//5
		    "Enter Radius of Second Interface       \0",//6
		    "Sorry!!!Enter radius between 60 and 200\0",//7
		    "Enter Radius of curved Interface       \0",//8
		    "Enter Refractive Index of Lens         \0",//9
		    "Sorry!!!Enter RI from 0.5 to 2.0(exc.1)\0",//10
		    "New Lens Inserted                      \0",//11
		    "Sorry!!!Overflow of Lenses(Max : 6)    \0",//12
		    "Enter Lens Number to be Modified       \0",//13
		    "Enter Value(1 to \0"                      ,//14
		    "Sorry!!!Underflow of Lenses            \0",//15
		    "Lens Removed                           \0",//16
		    "Lens Modified                          \0",//17
		    "Sorry!!!Enter X coord above \0"           ,//18
		    "\0",//19
		    "\0",//20
		    "\0",//21
		   };
 loadbitmap(pic);
 lensfinit();
 setcolor(12);
 rectangle(10,10,getmaxx()-10,getmaxy()-10);
 rectangle(15,15,getmaxx()-15,getmaxy()-15);
 setfillstyle(SOLID_FILL,15);floodfill(12,12,12);
 setcolor(10);rectangle(50,55,200,175);rectangle(45,50,205,180);
 setfillstyle(SOLID_FILL,12);floodfill(47,57,10);
 setcolor(12);rectangle(30,220,230,320);
 setcolor(10);line(80,184,80,210);line(60,197,80,184);line(80,210,60,197);
 setfillstyle(SOLID_FILL,1);floodfill(75,197,10);
 line(170,184,170,210);line(190,197,170,184);line(170,210,190,197);floodfill(175,197,10);
 rectangle(95,186,155,212);floodfill(97,188,10);outtextxy(102,195,"SELECT");
 setcolor(11);outtextxy(235,37,"HELP:");
 gotoxy(36,3);cout<<"Click on buttons to activate event";
 outtextxy(233,52,"INPUT:");
 setcolor(12);line(287,65,600,65);
 settextstyle(DEFAULT_FONT,HORIZ_DIR,1);setcolor(11);
 outtextxy(30,37,"LENS:");
 window(80,26,425,440,1,"DONE",0);
 window(80,26,535,440,1,"QUIT",0);
 window(80,26,315,440,1,"BACK",0);
 setfillstyle(SOLID_FILL,1);settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
 rectangle(255,276,575,294);floodfill(257,278,10);outtextxy(375,282,"REMOVE LENS");
 rectangle(255,298,575,316);floodfill(257,300,10);outtextxy(375,303,"MODIFY LENS");
 erase(51,56,199,174);
 gotoxy(10,3);cout<<"                    ";
 gotoxy(10,3);puts(lens[ctr].name);
 drawlens(lens[ctr],1);
 MM.reset();directvideo = 0;MM.show();
 while(quit==0)
 {
  if(MM.leftPressed)
  {
   if((mousechk(60,184,80,210)==1)&&flag==1)
   {
    if(ctr==0)ctr = 5; else --ctr;
    erase(51,56,199,174);
    gotoxy(10,3);cout<<"                    ";
    gotoxy(10,3);puts(lens[ctr].name);
    gotoxy(36,3);puts(help[2]);
    drawlens(lens[ctr],1);
    delay(600);
   }
   if((mousechk(170,184,190,210)==1)&&flag==1)
   {
    if(ctr==5)ctr = 0; else ++ctr;
    erase(51,56,199,174);
    gotoxy(10,3);cout<<"                    ";
    gotoxy(10,3);puts(lens[ctr].name);
    gotoxy(36,3);puts(help[2]);
    drawlens(lens[ctr],1);
    delay(600);
   }
   if((mousechk(95,186,155,212)==1)&&flag==1)
   {
    gotoxy(10,3);cout<<"                    ";
    gotoxy(10,3);puts(lens[ctr].name);
    gotoxy(36,3);puts(help[2]);
    if(sel<6) {flag = 0;selectlens(lens[ctr],flag,help,sel,0);}
    else {gotoxy(36,3);puts(help[12]);}
    delay(600);
   }
   if((mousechk(255,298,575,316)==1)&&flag==1)
   {
    if(sel!=0)
    {
     gotoxy(10,3);cout<<"                    ";
     gotoxy(10,3);puts(lens[ctr].name);
     gotoxy(36,3);puts(help[13]);
     flag = 0;
     do
     {
      gotoxy(37,4);puts(help[0]);
      gotoxy(37,4);cin>>i;
      gotoxy(36,3);puts(help[0]);
      gotoxy(36,3);puts(help[14]);
      gotoxy(54,3);cout<<sel<<")";
     } while(!(i>=1&&i<=sel));
     gotoxy(36,3);puts(help[0]);
     selectlens(lensf[i-1],flag,help,sel,i);
    }
    else
    {
     gotoxy(36,3);puts(help[0]);
     gotoxy(36,3);puts(help[15]);
    }
     delay(600);
   }
   if((mousechk(255,270,575,288)==1)&&flag==1)
   {
    if(sel!=0)
    {
     gotoxy(36,3);puts(help[0]);
     gotoxy(36,3);puts(help[16]);
     for(i=0;i<sel;i++)
     {
      gotoxy(5,i+15);
      cout<<"                       ";
     }
     --sel;
     for(i = 0;i<sel;i++)
     {
      gotoxy(5,i+15);
      cout<<i+1<<" ";
      puts(lensf[i].name);
     }
    }
    else
    {
     gotoxy(36,3);puts(help[0]);
     gotoxy(36,3);puts(help[15]);
    }
    delay(600);
   }
   if((mousechk(495,427,575,453)==1)&&flag==1)
   {
    for(i=0;i<5;i++)
    {
     window(80,26,535,440,4,"QUIT",0);delay(50);
     window(80,26,535,440,14,"QUIT",0);delay(50);
    }
    delay(1000);
    quit = 1; return 6;
   }
   if((mousechk(275,427,355,453)==1)&&flag==1)
   {
    for(i = 0;i<5;i++)
    {
     window(80,26,315,440,4,"BACK",0);delay(50);
     window(80,26,315,440,14,"BACK",0);delay(50);
    }
    delay(1000);
    quit = 1; return 0;
   }
   if((mousechk(385,427,465,453)==1)&&flag==1)
   {
    for(i = 0;i<5;i++)
    {
     window(80,26,425,440,4,"DONE",0);delay(50);
     window(80,26,425,440,14,"DONE",0);delay(50);
    }
    delay(1000);
    quit = 1;return 7;
   }
  }
  MM.autoEventUpdate();
  if(kbhit())ch = getch();
  if(ch=='0')quit = 1;
 }
}

int credits()
{
 int i,flag = 1,quit = 0;
 setcolor(12);
 settextstyle(10,HORIZ_DIR,5);outtextxy(178,40,"CREDITS ");
 settextstyle(7,HORIZ_DIR,1);setcolor(14);
 outtextxy(30,150,"At the outset, we would like to thank our computer teachers,");
 outtextxy(30,170,"Ms. Radhika and Ms. Priya for ensuring that our project");
 outtextxy(30,190,"is of a high standard. Their guidance was invaluable to us.");
 outtextxy(30,220,"We would also like to place on record our sincerest thanks to");
 outtextxy(30,240,"our senior, Niranjan for introducing us to graphics.");
 outtextxy(30,270,"We also thank our Principal, Mr. Nathan and our Vice");
 outtextxy(30,290,"Principal, Ms. Sasikala for providing us with an excellent");
 outtextxy(30,310,"programming environment.");
 outtextxy(30,340,"Our thanks also go out to our physics teacher, Ms. Sundari");
 outtextxy(30,360,"for giving us an interesting insight into ray optics.");
 outtextxy(30,380,"We also thank her for her useful feedback on our project.");
 window(80,26,245,440,1,"QUIT",0);
 window(80,26,365,440,1,"BACK",0);
 setcolor(12);
 rectangle(10,10,getmaxx()-10,getmaxy()-10);
 rectangle(15,15,getmaxx()-15,getmaxy()-15);
 setfillstyle(SOLID_FILL,15);floodfill(12,12,12);
 MM.reset();directvideo = 0;MM.show();
 while(quit==0)
 {
  if(MM.leftPressed)
  {
   if((mousechk(205,427,285,453)==1)&&flag==1)
   {
    for(i = 0;i<5;i++)
    {
     window(80,26,245,440,4,"QUIT",0);delay(50);
     window(80,26,245,440,14,"QUIT",0);delay(50);
    }
    delay(1000);
    quit = 1;return 6;
   }
   if((mousechk(325,427,405,453)==1)&&flag==1)
   {
    for(i = 0;i<5;i++)
    {
     window(80,26,365,440,4,"BACK",0);delay(50);
     window(80,26,365,440,14,"BACK",0);delay(50);
    }
    delay(1000);
    quit = 1;return 0;
   }
  }
  MM.autoEventUpdate();
 }
}

int drawline(int x1,int &x2,int &f,int m,int col)
{
 float u = x2-x1,y,i;
 int flag=0;
 setcolor(col);
 if(x2>x1)
 {
  for(i=u;i>0;i--)
  {
   line(x1,f,x1+u-i,f+(u-i)*(240.0-f)/u);
   line(x1,480.0-f,x1+u-i,480.0-f-(u-i)*(240.0-f)/u);
   if(lensf[m+1].p.x == (x1+u-i))
   {
    f=f+(u-i)*(240.0-f)/u;
    flag = 1;
    break;
   }
   delay(10);
  }
  if(flag==0)
  {
   for(i = 0;i<5;i++){setcolor(4); circle(x1+u,240,i);}
   if((m+1)!=sel)
   {
    u=lensf[m+1].p.x-x2;
    for(i=u;i>0;i--)
    {
     setcolor(14);
     line(x2,240,x2+u-i,240-(u-i)*(30.0)/u);
     line(x2,240,x2+u-i,240+(u-i)*(30.0)/u);
     if(lensf[m+1].p.x == (x2+u-i))
      {f=f+i*((240.0-f)/u);break;}
     delay(10);
    }
   }
  }
 }
 else
 {
  for(i=0;i>u;i--)
  {
   line(x1,f,x1+i,f+i*((240.0-f)/u));
   line(x1,480.0-f,x1+i,480.0-f-i*((240.0-f)/u));
   if(lensf[m+1].p.x == (x1+i)){f=f+i*((240.0-f)/u);break;}
   delay(10);
  }
  for(i = 0;i<5;i++){setcolor(8);circle(x1+u,240,i);}
 }
 return f;
}

int propscreen()
{
 int i,x1,x2;
 float u,v,f=210;
 setcolor(10);
 for(i = 0;i<sel;i++){drawlens(lensf[i],3);}
 setcolor(10);
 for(i = 0;i<sel;i++){line(lensf[i].p.x,190,lensf[i].p.x,289);}
 line(0,240,640,240);
 u = lensf[0].p.x;
 for(i=u;i>0;i--)
 {
  setcolor(14);
  line(0,240,u-i,240-(u-i)*(30.0)/u);
  line(0,240,u-i,240+(u-i)*(30.0)/u);
  delay(10);
  for(int j = 0;j<5;j++){setcolor(4);circle(0,240,j);}
 }
 do
 {
  switch(lensf[i].type)
  {
   case 1: lensf[i].f=(lensf[i].r1*lensf[i].r2)/((lensf[i].d-1.0)*(lensf[i].r1+lensf[i].r2));
	   break;
   case 2: lensf[i].f=-(lensf[i].r1*lensf[i].r2)/((lensf[i].d-1.0)*(lensf[i].r1+lensf[i].r2));
	   break;
   case 3:
   case 4: lensf[i].f = lensf[i].r/(lensf[i].d - 1.0);
	   break;
   case 5:
   case 6: lensf[i].f = -lensf[i].r/(lensf[i].d - 1.0);
	   break;
  }
  v = u*lensf[i].f/(u-lensf[i].f);
  x1 = lensf[i].p.x + v;
  if(v<0){drawline(lensf[i].p.x,x1,f,i,7);break;}
  else f=drawline(lensf[i].p.x,x1,f,i,14);
  i++;
  u = lensf[i].p.x - x1;
 } while(i<sel);
 getch(); return 6;
}

struct linvideo{ int x1,x2,sign,con,y1,y2,xt; };

void introvideo()
{
 int i,exit = 1;
 linvideo line1[100],line2[100];
 for(i = 0;i<100;i++)
 {
  line1[i].con = 1;
  line1[i].x1 = 10;
  line1[i].y1 = 40 + i*4;
  line2[i].con = 2;
  line2[i].x1 = 580;
  line2[i].y1 = 42 + i*4;
 }
 setcolor(11);
 arc(0,240,-45,45,630);
 arc(0,240,-45,45,631);
 line(10,20,10,460);
 line(9,20,9,460);
 setcolor(10);
 while(exit!=0)
 {
  for(i = 0;i<100;i++)
  {
   if(line1[i].con == 1)
   {
    line1[i].x1++;
    if(getpixel(line1[i].x1,line1[i].y1) == 11)
    {
     line1[i].con = 3;
     line1[i].x2 = line1[i].x1;
     line1[i].y2 = line1[i].y1;
    }
    else{putpixel(line1[i].x1,line1[i].y1,12);}
   }
   if(line2[i].con == 2)
   {
    line2[i].x1--;
    if(getpixel(line2[i].x1,line2[i].y1) == 11)
    {
     line2[i].con = 4;
     line2[i].x2 = line2[i].x1;
     line2[i].y2 = line2[i].y1;
    }
    else{putpixel(line2[i].x1,line2[i].y1,10);}
   }
   if(line1[i].con == 3)
   {
    setcolor(0);
    line1[i].x1--;
    line(line1[i].x2,line1[i].y2,line1[i].x1,line1[i].y1-((line1[i].y1-240.0)/(line1[i].x1-10.0)*(line1[i].x2-line1[i].x1)));
    if(line1[i].x1 == 15)exit = 0;
   }
  }
  delay(10);
 }
 setcolor(9);
 settextstyle(7,HORIZ_DIR,7);
 outtextxy(180,70,"PHOTONICS");
 setcolor(6);
 settextstyle(7,HORIZ_DIR,3);
 outtextxy(330,200,"VERSION <4.1>");
 setcolor(15);
 settextstyle(7,HORIZ_DIR,3);
 outtextxy(300,280,"BY");
 outtextxy(230,320,"FARID JALAL");
 outtextxy(200,345,"SUBHODEEP MOITRA");
 getch();
}

void administrator()
{
 int flag = 0,quit = 0;
 while(quit == 0)
 {
  switch(flag)
  {
   case 0: closegraph();initgraph(&gd,&gm,"");
	   flag = mainmenu();break;
   case 1: closegraph();initgraph(&gd,&gm,"");
	   flag = designerinit();break;
   case 5: closegraph();initgraph(&gd,&gm,"");
	   flag = credits();break;
   case 6: quit = 1;break;
   case 7: closegraph();initgraph(&gd,&gm,"");
	   flag = propscreen();break;
  }
 }
}

void main()
{
 initgraph(&gd,&gm,"");
 introvideo();
 closegraph();
 initgraph(&gd,&gm,"");
 administrator();
 closegraph();
}
