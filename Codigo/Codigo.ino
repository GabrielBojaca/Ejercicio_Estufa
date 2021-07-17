#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

float t0 = 0; //Tiempo de muestreo temperatura control
float t1 = 0; //Tiempo de visualizacion temperatura
long tiempoActual = 0;
long tiempoCoccionInicial = 0;
int intervalo = 100;
int intervaloVisu = 1000;
int cantidadMuestras = 0;
float acumuladorRef = 0;
float acumuladorAnalog = 0;
float ref;
boolean pin = true;
boolean enCurso = false;
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.display();
}

void loop() {
  
  if(digitalRead(2)== true){ //Iniciamos proceso de coccion
    enCurso = true;
    tiempoCoccionInicial = millis();
  }

  if(digitalRead(3) && digitalRead(2)){
    enCurso = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reiniciando...");
    delay(2000);
  }
  

  if(enCurso){
    tiempoActual = millis();
    if(millis()-t1 > intervaloVisu){
      t1 = millis();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cocinando...");
      lcd.setCursor(13,0);
      lcd.print(ref);
      lcd.setCursor(0,1);
      lcd.print("Temp. actual:");
      lcd.setCursor(13,1);  
      lcd.print(analogRead(A0)/10.23);
    }

    if(tiempoActual - tiempoCoccionInicial > 60000){

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Terminado.");
      ref = 0;
      delay(2000);
      enCurso = false;
            
    } else if(tiempoActual - tiempoCoccionInicial > 40000){
      ref = 30;
    } else if(tiempoActual - tiempoCoccionInicial > 20000){
      ref = 80;
    } else if(tiempoActual - tiempoCoccionInicial > 0){
      ref = 60;
    }
    
    /*Serial.print("TiempoCoccioninicial:");
    Serial.print(tiempoActual - tiempoCoccionInicial );
    Serial.print("  ");
    Serial.println(ref);*/
  } else{
    if(millis()-t1 > intervaloVisu){
    //Serial.println(millis()-t0);
    t1 = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ref:");
    lcd.setCursor(4,0);
    lcd.print(analogRead(A7)/10.23);
    
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.setCursor(5,1);
    lcd.print(analogRead(A0)/10.23);
    }
    
  }

  if(millis()-t0 > intervalo){
    //Serial.println(millis()-t0);
    t0 = millis();

    
    digitalWrite(13, pin);
  }

  if(!enCurso){
      ref = analogRead(A7)/10.23;
    } 
    
    if(analogRead(A0)/10.23 > ref){
      pin = true;
    } else{
      pin = false;
    }
  
  if(cantidadMuestras <= 150){
    acumuladorRef += ref/151.0;
    acumuladorAnalog += (analogRead(A0)/10.23)/151;
    cantidadMuestras++;
  }
  else{
    Serial.print("0,100,");
    Serial.print(acumuladorRef);
    Serial.print(",");
    Serial.println(acumuladorAnalog);
    acumuladorRef = 0;
    acumuladorAnalog = 0;
    cantidadMuestras = 0;

  }
 /*
  Serial.print("0,100,");
  Serial.print(ref);
  Serial.print(",");
  Serial.println(analogRead(A0)/10.23);*/
  
  /*
  Serial.print(digitalRead(2));
  Serial.println(digitalRead(3));
*/
}
