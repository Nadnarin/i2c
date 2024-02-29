#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
int I_analogPin = A1;
int V_analogPin = A2;
int Voffset_analogPin = A3;
int current=0,voltage=0,ZeroV=0;
float sum_current,sum_voltage=0, sum_zero=0,sum_power=0,sum_kWh=0;
float Irms=0,Vrms=0,zero=500,P=0;
int k=0;

void setup() {
lcd.begin();
lcd.backlight();

TCCR1A = 0;
TCCR1B = 0;
TCCR1B |= (1 << WGM12);
TCCR1B |= (1 << CS10);
OCR1A = 3199;
TIMSK1 |= (1 << OCIE1A);
TCNT1 = 0;
}
void loop() {
lcd.setCursor(4, 0);
lcd.print("GROUP 9");
delay(4000);


while(1) {
TCNT1 = 0;
sum_zero=0;
  sum_current=0;
  sum_voltage=0;
  sum_power=0;
TIMSK1 |=(1 << OCIE1A); 
delay(2000); 
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("V =");
lcd.setCursor(4, 0);
lcd.print(Vrms);
lcd.setCursor(15, 0);
lcd.print("V");
lcd.setCursor(0, 1);
lcd.print("I =");
lcd.setCursor(4, 1);
lcd.print(Irms);
lcd.setCursor(15, 1);
lcd.print("A");
delay(6000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("P =");
lcd.setCursor(4, 0);
lcd.print(P);
lcd.setCursor(15, 0);
lcd.print("W");
lcd.setCursor(0, 1);
lcd.print("S =");
lcd.setCursor(4, 1);
lcd.print(Irms*Vrms);
lcd.setCursor(14, 1);
lcd.print("VA");
delay(5000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("PF =");
lcd.setCursor(5, 0);
lcd.print(P/(Irms*Vrms));
lcd.setCursor(0, 1);
lcd.print("kWh =");
lcd.setCursor(6, 1);
lcd.print(sum_kWh);
delay(3000);  

}
}
ISR(TIMER1_COMPA_vect) {
k++;
current = analogRead(I_analogPin);
voltage = analogRead(V_analogPin);

sum_zero+= current*0.0025; 
current-=zero; 
sum_current+= (float)(0.00073*current*current);

ZeroV = analogRead(Voffset_analogPin);
voltage-=ZeroV;
sum_voltage+=(float)(0.93*voltage*voltage);
sum_power+=(float)(0.0258*voltage*current);
if (k==400) {
  k=0;
  Irms=sqrt(0.0025*sum_current);
  Vrms=sqrt(0.0025*sum_voltage);
  P=0.0025*sum_power;
  zero=sum_zero;
  sum_zero=0;
  sum_current=0;
  sum_voltage=0;
  sum_power=0;
  sum_kWh+=P*0.00000412;
  TIMSK1 = B00000000;
}
}
