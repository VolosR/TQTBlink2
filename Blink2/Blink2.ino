#include <TFT_eSPI.h>
#include "NotoSansBold15.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define small NotoSansBold15
unsigned short colors[5]={0xC6FB,0x83E5,0x8987,0x1126, 0x1A49};

int x[3]={76,76,4};
int y[3]={0,54,22};
int w[3]={50,50,50};
int h[3]={50,50,40};
int c[2]={0x1988,colors[2]};
String stateS[2]={"ON","OFF"};
String labels[4]={"x10", "x100","x10K","x100K"}; 

bool selected=0;
int multiPlier[2]={0,0};
double mValues[4]={10,100,10000,100000};
int values[2]={50,50};
double times[3]={0};
String Stimes[3];
bool turnOn=false;

bool deb,deb2=0;
#define LED 34

double currentTime=0;
double period=0;

void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(47,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,turnOn);
  tft.init();
  tft.setRotation(3);
  sprite.createSprite(128,128);
  sprite.setTextColor(TFT_WHITE,TFT_BLACK);
}


void draw()
  {
    sprite.fillSprite(TFT_BLACK);
    sprite.setTextDatum(4);

    for(int i=0;i<2;i++){
    sprite.loadFont(small);
    sprite.fillRect(x[i],y[i],w[i],h[i],colors[4]);
    sprite.setTextColor(colors[0],colors[4]);
    sprite.drawString(String(values[i]),x[i]+25,y[i]+25); 
    sprite.drawSmoothArc(x[i]+25, y[i]+25, 22, 18, 40, 320, 0x3B6D, colors[4]);
    if(values[i]>0)
    sprite.drawSmoothArc(x[i]+25, y[i]+25, 21, 19, 40, map(values[i],0,100,40,320), colors[0], 0x3B6D);
    sprite.unloadFont();
    sprite.drawString(stateS[i],x[i]+25,y[i]+42);
    sprite.fillSmoothCircle(x[i]+44, y[i]+6, 2, TFT_SILVER,colors[4]);
    }
    
    sprite.drawRect(x[selected],y[selected],w[selected],h[selected],colors[0]);
    
    sprite.setTextDatum(0);
    sprite.loadFont(small);
    sprite.setTextColor(colors[0],TFT_BLACK);
    sprite.drawString("BLINK",0,0);
    
    sprite.unloadFont();
    sprite.fillSmoothRoundRect(49, 0, 24, 13, 10, c[turnOn],TFT_BLACK);
    sprite.fillSmoothCircle(56+(turnOn*12), 6, 3, colors[0], c[turnOn]);

    sprite.setTextDatum(4);
    sprite.setTextColor(TFT_BLACK,colors[1]);
    
    for(int i=0;i<4;i++)
    {
      sprite.fillRect(i*32,112,28,10,colors[1]);
      sprite.drawString(labels[i],i*32+14,117);
    }

    sprite.fillRect(multiPlier[0]*32,107,28,3,colors[2]);
    sprite.fillRect(multiPlier[1]*32,125,28,3,colors[2]);

    sprite.setTextColor(0x3D14,TFT_BLACK);
    sprite.setTextDatum(0);
    sprite.drawString("TIME ON",0,18);
    sprite.drawString("TIME OFF",0,48);
    sprite.drawString("FREQENCY",0,78);

    sprite.setTextColor(colors[0],TFT_BLACK);
    sprite.setTextDatum(0);
    
    for(int i=0;i<3;i++){
    sprite.drawString(Stimes[i],0,30+(i*30),2);
    sprite.drawLine(0,28+(i*30),70,28+(i*30),0x4228);}
    
    
    sprite.setTextDatum(4);
    sprite.setTextColor(0x4A49,TFT_BLACK);
    sprite.drawString(stateS[turnOn],62,20);


    sprite.pushSprite(0,0);
  }



void loop() {

  if(turnOn==0)
  if(millis()>currentTime+times[1])
  {turnOn=1;
  digitalWrite(LED,turnOn);
  currentTime=millis();} 

    if(turnOn==1)
  if(millis()>currentTime+times[0])
  {turnOn=0;
  digitalWrite(LED,turnOn);
  currentTime=millis();} 

  if(digitalRead(0)==0)
  {if(deb==0) {deb=1; selected=!selected;}
  }else deb=0;

  if(digitalRead(47)==0)
  {if(deb2==0) {deb2=1; multiPlier[selected]++;
  if(multiPlier[selected]==4) multiPlier[selected]=0;}
  }else deb2=0;
  
  values[selected]=map(analogRead(16),0,4096,1,101);

  times[0]=values[0]*mValues[multiPlier[0]];
  times[1]=values[1]*mValues[multiPlier[1]];
  times[2]=1/((times[0]/1000)+(times[1]/1000));

  if(times[0]>2000 && times[0]<=60000 ) Stimes[0]=String(times[0]/1000.00)+" S"; 
  else if (times[0]>60000) Stimes[0]=String(times[0]/60000.00)+" Min";
  else Stimes[0]=String((int)times[0])+" mS"; 

  if(times[1]>2000 && times[1]<=60000 ) Stimes[1]=String(times[1]/1000.00)+" S"; 
  else if (times[1]>60000) Stimes[1]=String(times[1]/60000.00)+" Min";
  else Stimes[1]=String((int)times[1])+" mS"; 

  

  Stimes[2]=String(times[2])+" Hz";
  
  draw();
  
}