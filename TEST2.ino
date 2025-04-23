#include <LiquidCrystal.h>
LiquidCrystal lcd(0,1,2,3,4,5,6);
double a=0.0;
unsigned char i;
signed char c=1;
signed char b=1;
unsigned char speed=80;
unsigned char avrage=100;
unsigned char max_avrage=100;
unsigned char min_avrage=100;
//*************
//double filter=0.1;
//*************
double avrage_filter=0.00;
//*************
double CHANGES = 0.1;           //Q      
double NOIS = 5.0;              //R
double UNCERTAINTY = 1.0;       //P
double K_COEFFICIENT = 0.0;     //K
//*************
void setup()
{
 pinMode(7,OUTPUT);
 pinMode(8,INPUT);
 digitalWrite(7, LOW);
 lcd.begin(16,2);
 lcd.clear();
 pinMode (11,INPUT_PULLUP);
 pinMode (12,INPUT_PULLUP);
 pinMode (9,OUTPUT);
 pinMode (10,OUTPUT);
}
void loop() 
{
 if(digitalRead(12)==LOW)
 {
  if(c==1)
  {
    lcd.setCursor(11,1);
    lcd.print("V:on ");
    digitalWrite(10,HIGH);
    while(digitalRead(12)==LOW){};
    delay(500);
  }
  if(c==-1)
  {
    lcd.setCursor(11,1);
    lcd.print("V:off");
    digitalWrite(10,LOW);
    while(digitalRead(12)==LOW){};
    delay(500);
  }
  c=c*-1;
 }
 if(digitalRead(11)==LOW)
 {
  if(b==1)
  {
    while(digitalRead(11)==LOW){};
    delay(500);
  }
  if(b==-1)
  {
    digitalWrite(9,LOW);//analogWrite(9,0);
    while(digitalRead(11)==LOW){};
    delay(500);
  }
  b=b*-1;
 }
 if(c==1)
  {
   lcd.setCursor(11,1);
   lcd.print("V:off");
  }
  else
  {
   lcd.setCursor(11,1);
   lcd.print("V:on ");
  }
  a=0.00;
   digitalWrite(7,HIGH);
   delayMicroseconds(10);
   digitalWrite(7,LOW);
   a=pulseIn(8,HIGH);
  a=(a*0.034)/2;
  //a=a-23.2;
 //*************
 //avrage_filter = (filter* a) + ((1 - filter) * avrage_filter);
 //*************
  UNCERTAINTY = UNCERTAINTY + CHANGES;
  K_COEFFICIENT = UNCERTAINTY / (UNCERTAINTY + NOIS);
  avrage_filter = avrage_filter + K_COEFFICIENT * (a - avrage_filter);
  UNCERTAINTY = (1 - K_COEFFICIENT) * UNCERTAINTY;
  //*************
  lcd.setCursor(0,0);
  lcd.print("Height:");
  lcd.print(avrage_filter,1);
  lcd.print("CM   ");
  delay(50);
 if(b==-1 && a<=avrage)
 {
  digitalWrite(9,HIGH);//analogWrite(9,speed);
  lcd.setCursor(0,1);
  lcd.print("PWM:");
  lcd.print(speed);
  lcd.print("   ");
  if(avrage==min_avrage){avrage=max_avrage;}
 }
 else
 { 
  digitalWrite(9,LOW);//analogWrite(9,0);
  lcd.setCursor(0,1);
  lcd.print("PWM:0  ");
  if(avrage==max_avrage && b==-1){avrage=min_avrage;}
 }
}