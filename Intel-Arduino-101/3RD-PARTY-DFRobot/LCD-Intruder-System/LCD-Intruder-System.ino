/*
 
  Copyright (c) 2016 TechBubble Technologies and other Contributors.
  
  Contributors:  
  Adam Milton-Barker - TechBubble Technologies Limited
  For this project you will need to use the TechBubble IoT JumpWay Python MQTT Serial Library:
  https://github.com/TechBubbleTechnologies/IoT-JumpWay-Python-MQTT-Serial-Client
  
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);    

int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

String JumpWaySensorType = "LCD Keypad";
String JumpWaySensorID = "1";

String JumpWaySensorType2 = "PIR Sensor";
String JumpWaySensorID2 = "2";

byte DFRobotMotionPin = 2;

int debounceWait = 150;
int armedStatus = 0;

int read_LCD_buttons(){  

    adc_key_in = analogRead(0);  

    if (adc_key_in > 1000) return btnNONE; 
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 350)  return btnUP; 
    if (adc_key_in < 650)  return btnDOWN; 
    if (adc_key_in < 950)  return btnLEFT; 
    
    return btnNONE;
}

void setup(){  
  
   pinMode(DFRobotMotionPin,INPUT);
   
   Serial.begin(9600); 
   
   lcd.begin(16, 2);               
   lcd.setCursor(0,0);             
   lcd.print("ONLINE");  
   
}
 
void loop(){
   
   String jsonString = "";
             
   lcd_key = read_LCD_buttons();
   
   if(armedStatus == 1){
   
     lcd.setCursor(0,1); 
     lcd.print("ARMED    ");
     byte state = digitalRead(DFRobotMotionPin);
     
     if(state == 1){
      
        lcd.setCursor(0,1);          
        lcd.print("INTRUDER!");
        
        jsonString = "{\"Sensor\":\""+JumpWaySensorType2+"\",\"SensorID\":\""+JumpWaySensorID2+"\",\"SensorValue\": \"INTRUDER\"}";
        Serial.println(jsonString);
      
        jsonString = "{\"WarningType\":\""+JumpWaySensorType2+"\",\"WarningOrigin\":\""+JumpWaySensorID2+"\",\"WarningValue\": \"INTRUDER\",\"WarningMessage\": \"An intruder has been detected\"}";
        Serial.println(jsonString);
        
     } else if(state == 0){
      
        lcd.setCursor(0,1); 
        lcd.print("ARMED    ");
        jsonString = "{\"Sensor\":\""+JumpWaySensorType2+"\",\"SensorID\":\""+JumpWaySensorID2+"\",\"SensorValue\": \"OK\"}";
        Serial.println(jsonString);
        
     }
      
   } else {
    
      lcd.setCursor(0,1); 
      lcd.print("NOT ARMED"); 
   
   }
   
   switch (lcd_key){   
       
       case btnUP:{
        
             lcd.setCursor(0,1);
             lcd.print("ARMED    ");
             jsonString = "{\"Sensor\":\""+JumpWaySensorType+"\",\"SensorID\":\""+JumpWaySensorID+"\",\"SensorValue\": \"ARMED\"}";
             delay(debounceWait);
             armedStatus = 1;
             Serial.println(jsonString);
             break;
             
       }
       case btnDOWN:{
        
             lcd.setCursor(0,1);
             lcd.print("NOT ARMED"); 
             jsonString = "{\"Sensor\":\""+JumpWaySensorType+"\",\"SensorID\":\""+JumpWaySensorID+"\",\"SensorValue\": \"NOT ARMED\"}";
             delay(debounceWait);
             armedStatus = 0;
             Serial.println(jsonString);
             break;
             
       }
       case btnLEFT:{
        
             lcd.setCursor(0,1);
             lcd.print("COMMAND 3");  
             jsonString = "{\"Sensor\":\""+JumpWaySensorType+"\",\"SensorID\":\""+JumpWaySensorID+"\",\"SensorValue\": \"3\"}";
             delay(debounceWait);
             Serial.println(jsonString);
             break;
             
       } 
       case btnRIGHT:{          
             
             lcd.setCursor(0,1);
             lcd.print("COMMAND 4"); 
             jsonString = "{\"Sensor\":\""+JumpWaySensorType+"\",\"SensorID\":\""+JumpWaySensorID+"\",\"SensorValue\": \"4\"}";
             delay(debounceWait);
             Serial.println(jsonString);
            break;
            
       }  
       case btnNONE:{          
             
            lcd.setCursor(0,1);
            lcd.print("         "); 
            break;
            
       }
       
       delay(5000);
       
   }
}