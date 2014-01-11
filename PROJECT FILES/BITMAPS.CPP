#include<stdio.h>
#include<fstream.h>
#include<graphics.h>

int getcol(int);

int loadbitmap(char *filename)
{
 FILE *ptr=NULL;
 int width,height;
 unsigned long temp=0,i=0,j=0;
 unsigned long ww;
 ptr=fopen(filename,"rt");            //Open the bitmap file
 if(!ptr) return 0;                   //If its not there return
 width=0;height=0;                    //Initialise width and height to zero
 fseek(ptr,18,SEEK_SET);              //Go to offset 18 in file
 for(i=0x1;i<=0x10000;i*=0x100)       //Read the width
 {
  temp=fgetc(ptr);
  width+=(temp*i);
 }
 fseek(ptr,22,SEEK_SET);              //Go to offset 22 in file
 for(i=0x1;i<=0x10000;i*=0x100)       //Read the height
 {
  temp=fgetc(ptr);
  height+=(temp*i);
 }
 ww=width;                            //ww is the number of reads to make for each horizontal line
 if(ww%2)                             //ww has to be half of width - since each pixel is only 4 bits of information
 ww++;ww/=2;
 if(ww%4)                             //ww is stored as sets of 4 pixels each
 ww=(ww/4)*4+4;                       //If width is less than ww*2 we ignore what we read
 height=height+102;
 fseek(ptr,119,SEEK_SET);             //Offset 119 - Read the pixels (the bitmap is stored upside down)
 int ch,ch1,ch2;
 for(i=0;i<height-102;i++)
 {
  for(j=8;j<ww+8;j++)
  {
   ch=fgetc(ptr);                     //Each character read is 2 pixels - 4 bits per pixel (16 colors)
   ch1=ch;ch2=ch;                     //Find colors using bitwise ops
   ch1=ch1&(0xf0);ch1=ch1>>4;
   ch2=ch2&(0x0f);
   if(j*2<width)                      //Put first pixel read on screen
   putpixel(j*2,(height-1-i),getcol(ch1));
   if(j*2+1<width)                    //Put second pixel read on screen
   putpixel(j*2+1,(height-1-i),getcol(ch2));
  }
 }
 fclose(ptr);                         //Close the file
 return 1;
}

int getcol(int col)
{
 switch(col)
 {
  case 0: return 0;   //BLACK
  case 1: return 4;   //RED
  case 2: return 2;   //GREEN
  case 3: return 6;   //BROWN
  case 4: return 1;   //BLUE
  case 5: return 5;   //MAGENTA
  case 6: return 3;   //CYAN
  case 7: return 7;   //LIGHTGRAY
  case 8: return 8;   //DARKGRAY
  case 9: return 12;  //LIGHTRED
  case 10:return 10;  //LIGHTGREEN
  case 11:return 14;  //YELLOW
  case 12:return 9;   //LIGHTBLUE
  case 13:return 13;  //LIGHTMAGENTA
  case 14:return 11;  //LIGHTCYAN
  case 15:return 15;  //WHITE
 }
 return col;
}

