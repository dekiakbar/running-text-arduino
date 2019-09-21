#include <SPI.h>
#include <TimerOne.h>
#include <Wire.h>
#include <DMD.h>         
#include "SystemFont5x7.h" 
#include "Arial_black_16.h"
#include <RTClib.h>
// Mendifinisakan Fungsi
#define max_char 100
#define bOK A0          
#define bUP A1
#define bDOWN A2
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);                  // Panjang x Lebar
RTC_DS1307 rtc;                               // Chip RTC yang digunakan
// Deklarasi Variable
char message[max_char];
char r_char;
byte index = 0;
int i;
int waktu;
int Tahun ;
int Bulan ;
int Tanggal ;
int Jam ;
int Menit ;
int Detik ;
char dmdBuff[10];


void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

void setup() {
  Timer1.initialize( 5000 );           
  Timer1.attachInterrupt( ScanDMD );
  dmd.selectFont(System5x7);
  dmd.clearScreen(true);
  rtc.begin();
  Serial.begin(9600);
  // Cara Setting Jam gunakan 2 fungsi dibawah, secara manual atau otomatis singcron PC, kemudian upload program
  // Setelah terupload, cek jam sudah sesuai, hapus code setting jam dibawah, dan upload ulang.
  
  // Set RTC sesuai dengan Laptop / PC
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // set rtc custom
   //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
  // pin Mode yang dugunakan pada Inputan
  pinMode(bOK,INPUT_PULLUP);
  pinMode(bUP,INPUT_PULLUP);
  pinMode(bDOWN,INPUT_PULLUP);
}

void showTime() {
  // Program menampilkan Jam
  DateTime now = rtc.now();
  Tahun = now.year();
  Bulan = now.month();
  Tanggal = now.day();
  Jam = now.hour();
  Menit = now.minute();
  Detik = now.second();
  
  if (Jam < 10){
    if (Menit < 10){
      if (Detik < 10)
        {sprintf(dmdBuff,"0%1d:0%1d:0%1d",Jam,Menit,Detik);}
      else
        {sprintf(dmdBuff,"0%1d:0%1d:%2d",Jam,Menit,Detik);}
    }else{
    if (Detik < 10)
        {sprintf(dmdBuff,"0%1d:%2d:0%1d",Jam,Menit,Detik);}
        else
        {sprintf(dmdBuff,"0%1d:%2d:%2d",Jam,Menit,Detik);}
    }
  }else{
  if (Menit < 10){
      if (Detik < 10)
        {sprintf(dmdBuff,"%2d:0%1d:0%1d",Jam,Menit,Detik);}
      else
        {sprintf(dmdBuff,"%2d:0%1d:%2d",Jam,Menit,Detik);}
    }else{
    if (Detik < 10){
        sprintf(dmdBuff,"%2d:%2d:0%1d",Jam,Menit,Detik);
      }else{
        sprintf(dmdBuff,"%2d:%2d:%2d",Jam,Menit,Detik);
      }
    }
  }
  dmd.drawString( 7, 0, dmdBuff,8,GRAPHICS_NORMAL );
  
  if (Bulan < 10){
    if (Tanggal < 10)
      {sprintf(dmdBuff,"0%1d/0%1d/%4d",Tanggal,Bulan,Tahun);}
      else
      {sprintf(dmdBuff,"%2d/0%1d/%4d",Tanggal,Bulan,Tahun);}
  }else{
    if (Tanggal < 10)
      {sprintf(dmdBuff,"0%1d/%2d/%4d",Tanggal,Bulan,Tahun);}
      else
      {sprintf(dmdBuff,"%2d/%2d/%4d",Tanggal,Bulan,Tahun);}
  }
  
  dmd.drawString( 1, 9, dmdBuff,10,GRAPHICS_NORMAL );
}

void loop() {
  
  if(Serial.available()){
        for(i=0; i<100; i++){
        message[i] = '\0';
    }
     index=0;
  }
  
  while(Serial.available() > 0){
    if(index < (max_char-1)) {
        r_char = Serial.read();
        message[index] = r_char;
        index++;
        message[index] = '\0';
      }
  }
  
  dmd.clearScreen( true );
  dmd.selectFont(Arial_Black_16);
  dmd.drawMarquee(message, max_char,(32*DISPLAYS_ACROSS)-1 ,0);
  long start=millis();
  long timer=start;
  boolean ret=false;

  while(!ret){
     if ((timer+90) < millis()) {
         ret=dmd.stepMarquee(-1,0);
         timer=millis();
       }
  }
  
  for (waktu=0;waktu < 10;waktu++){
  dmd.clearScreen( true );
  dmd.selectFont(System5x7);
  showTime();
  delay(1000);
  }
  
}
