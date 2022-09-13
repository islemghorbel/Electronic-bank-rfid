#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
//#include <string.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const String id1 = "63 BA 9E 95";  //03 3D 88 1B
const String id2 = "A3 DE BE 95";  //FC 2F E7 C0
const String id3 = "D3 6D BD 95";  //9E FB 67 2D
const String id4 = "03 B1 27 96";  //2C 61 EF C0
const String id5 = "23 2F 11 96";  //DE 08 CE 7A
const String id6 = "D3 B0 9E 95";  //2E 4B A7 7B
const String idb = "EA D9 F1 B8";  //2B 63 BC 0D

int beepTime = 2700;
int currentNum;
int initialamount = 1500;
int StartPmoney = 200;
int piezoPin = 0;
int card1=0;
int card2;
int card3;
int card4;
int card5;
int card6;
int interSt;
int lineSt = 0;
int Step=0;
byte i = 0;
byte data_count = 0; 
char customKey;
String currentCard;
String clearTr = "C";
String Data;
String firstCh;
String content= "";
String content2="";
String cardNum="";
const byte ROWS  = 4;
const byte COLS = 4;


char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', 'E'},
  {'*', '0', '#', 'C'}
};

byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2, 1};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x3F,16,2);   // 0X27
 

void setup() {
  
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
    lcd.init();
  lcd.begin(16,2);
  lcd.backlight ();
  lcd.print("Monopoly Banque");
  lcd.setCursor(0,1);
  lcd.print(" Electronique  ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);   //  (0,1)

 
  

}

void loop() {
   

  noTone(piezoPin);

  customKey = customKeypad.getKey(); //shortening the getKey function
  Data = customKey;   
  
  if(Step==0){
   
 
    
     
    
    getCard();                                                                  //main loop <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                                                    
    receiveNum();
  }
  else if(Step==1){
     
    lcd.setCursor(0,0);
    lcd.print("PASSER");
    lcd.setCursor(0,1);
    lcd.print("LA 2eme Carte");
    intSec();
      
  }
    
    
 
    
   
    
  

  if(Data == clearTr)               //clear with D
    {
     
     lineSt=0;
     clear();
     firstCh="";
      }


}



void receiveNum(){
  
   if (customKey){          //printing character to lcd
     
     
     lcd.setCursor(data_count,lineSt);
     Data = customKey; 
  
     
     lcd.print(Data);
     if(Data == "-"){
      tone(piezoPin, 500);
     }
     else if(Data == "+"){
      tone(piezoPin, 2500);
     }
     else{
      tone(piezoPin, beepTime);
      
     }
     delay(100);
     if(Data == "#"){
       noTone(piezoPin);
       clear();
       lcd.setCursor(0,0);
       
       GiveST();
       Data="";
       //clear();
       lcd.setCursor(0,1);
       lcd.print(StartPmoney);
       lcd.print(" $ Ajouter");
       delay(2000);
       clear();
       lcd.setCursor(0,0);
     }
     if(Data == "*"){
      Data="";
      lcd.print("Initialisation ?");
      lcd.setCursor(0,1);
      lcd.print("Appuyer 0");
      lcd.setCursor(0,0);
      while (Data == ""){
        
      
      
      if (customKey){          //printing character to lcd
     
     
     lcd.setCursor(data_count,lineSt);
     Data = customKey; 

     lcd.print(Data);
     if(Data == "0"){
      
      noTone(piezoPin);
      card1=initialamount;
      card2=initialamount;
      card3=initialamount;
      card4=initialamount;
      card5=initialamount;
      card6=initialamount;
      
      Data="";
      
      
      clear();
      lcd.setCursor(0,0);
      lcd.print(initialamount);
      lcd.print("$ A Chaque");
      lcd.setCursor(0,1);
      lcd.print("Joueur");
      delay(3000);
      clear();
      lcd.setCursor(0,0);

     }
      }
     else {
        Data="";
        clear();
        lcd.setCursor(0,0);
       }

      }

     }
    if(Data == "E"){
      currentNum=firstCh.toInt();
      noTone(piezoPin); 
      clear();
      everyPl();
      clear();
       lcd.setCursor(0,0);
       lcd.print(currentNum);
       lcd.print("$ ajouter a");
       lcd.setCursor(0,1);
       lcd.print("chaque Joueur");
       delay(2000);
       clear();
       lcd.setCursor(0,0);
       Data="";
     }
     Serial.print("current:");
     Serial.println(currentNum);
     addMoney();
     minMoney();
      
     data_count++; 
     
     firstCh.concat(Data);
     
     
     
     
     if(data_count== 16){
         
         lineSt++;
         data_count=0;
         }
         
     
    
   
    }
    else{
      noTone(piezoPin);
    }
}

void clear(){                      //clear void
   lcd.clear();
   data_count=0;
   
   
      }

void getCard(){
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
    tone(piezoPin, beepTime);
    delay(100);
    noTone(piezoPin);
    
  
  content="";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  
  content.toUpperCase();
 
    if(Step==0){
     displayCard();
  }
 
        
}

void addMoney(){
 
       if(Data == "+"){
        currentNum=firstCh.toInt();
        noTone(piezoPin);
        
        clear();
        interSt=1;
        if (content.substring(1) == id1){
          card1=card1+currentNum;
          
          lcd.print("carte1:");
          lcd.print(card1);
          lcd.print(" $");
          delay(2000);
          clear();
         
          Step=1;
          return;
        }
        else if(content.substring(1) == id2){
          card2=card2+currentNum;
          
          lcd.print("carte2:");
          lcd.print(card2);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }                                                           /////////////correction
        else if(content.substring(1) == id3){
          card3=card3+currentNum;
          
          lcd.print("carte3:");
          lcd.print(card3);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }
        else if(content.substring(1) == id4){
          card4=card4+currentNum;
          
          lcd.print("carte4:");
          lcd.print(card4);
          lcd.print(" $");
          delay(2000);
          clear();
        
                    Step=1;

          return;
        }
        else if(content.substring(1) == id5){
          card5=card5+currentNum;
          
          lcd.print("carte5:");
          lcd.print(card5);
          lcd.print(" $");
          delay(2000);
          clear();
          Step=1;
          return;
        }
        else if(content.substring(1) == id6){
          card6=card6+currentNum;
          
          lcd.print("carte6:");
          lcd.print(card6);
          lcd.print(" $");
          delay(2000);
          clear();
          Step=1;
          return;
        }
        else{
          noTone(piezoPin);
          lcd.print("Pas de Carte");
          delay(2000);
          clear();
          content="";
          currentNum=0;
          return;
        }
        
        
     }
}

void minMoney(){
  
  if(Data == "-"){
        currentNum=firstCh.toInt();
        noTone(piezoPin);
        clear();
        interSt=0;
        if (content.substring(1) == id1){
          card1=card1-currentNum;
          
          lcd.print("carte1:");
          lcd.print(card1);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }
        else if(content.substring(1) == id2){
          card2=card2-currentNum;
          
          lcd.print("carte2:");
          lcd.print(card2);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }
        else if(content.substring(1) == id3){
          card3=card3-currentNum;
          
          lcd.print("carte3:");
          lcd.print(card3);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }
        else if(content.substring(1) == id4){
          card4=card4-currentNum;
          
          lcd.print("carte4:");
          lcd.print(card4);
          lcd.print(" $");
          delay(2000);
          clear();
          Step=1;
          return;
        }
        else if(content.substring(1) == id5){
          card5=card5-currentNum;
          
          lcd.print("carte5:");
          lcd.print(card5);
          lcd.print(" $");
          delay(2000);
          clear();
          
                    Step=1;

          return;
        }
        else if(content.substring(1) == id6){
          card6=card6-currentNum;
          
          lcd.print("carte6:");
          lcd.print(card6);
          lcd.print(" $");
          delay(2000);
          clear();
          Step=1;
          return;
        }
        else{
          noTone(piezoPin);
          lcd.print("Pas de Carte");
          delay(2000);
          clear();
          content="";
          return;
        }
        
     
     }
}

void intSec(){
  content="";
  
  getCard();
  
  
  if (content==""){
    return;
  }
  
       Serial.print("boom");
       if (content.substring(1) == id1){
          if(interSt==1){
            card1=card1-currentNum;
            
          }
          else{
            card1=card1+currentNum;
            
          }
          Step=0;
        }
      if(content.substring(1) == id2){
          if(interSt==1){
            card2=card2-currentNum;
          }
          else{
            card2=card2+currentNum;
          }
          Step=0;
        }
        if(content.substring(1) == id3){
          if(interSt==1){
            card3=card3-currentNum;
            
          }
          else{
            card3=card3+currentNum;
          }
          Step=0;
        }
        if(content.substring(1) == id4){
          if(interSt==1){
            card4=card4-currentNum;
          }
          else{
            card4=card4+currentNum;
          }
          Step=0;
        }
       if(content.substring(1) == id5){
          if(interSt==1){
            card5=card5-currentNum;
          }
          else{
            card5=card5+currentNum;
          }      
          Step=0;    
        }
         if(content.substring(1) == id6){
         if(interSt==1){
            card6=card6-currentNum;
            
          }
          else{
            card6=card6+currentNum;
            
          }
          Step=0;
        }
       if(content.substring(1) ==idb){
           Step=0;
           lcd.setCursor(0,0);
           lcd.print("transaction");
           lcd.setCursor(0,1);
           lcd.print("confirmee avec le# ");
        }
        else{
          lcd.print("erreur");
          cardNum="";
           
          Step=0;
          
        }
        
        
        clear();
        lcd.clear();
        displayCard();
        firstCh="";
       
  
}

void displayCard(){
    noTone(piezoPin);
    if(content.substring(1) == id1){
    lcd.print("Carte1:");
    lcd.print(card1);
    lcd.print("$");
    
  }
  if(content.substring(1) == id2){
    lcd.print("Carte2:");
    lcd.print(card2);
    lcd.print("$");
    
  }
  if(content.substring(1) == id3){
    lcd.print("Carte3:");
    lcd.print(card3);
    lcd.print("$");
   
  }
  if(content.substring(1) == id4){
    lcd.print("Carte4:");
    lcd.print(card4);
    lcd.print("$");
   
  }
  if(content.substring(1) == id5){
    lcd.print("Carte5:");
    lcd.print(card5);
    lcd.print("$");
    
  }
  if(content.substring(1) == id6){
    lcd.print("Carte6:");
    lcd.print(card6);
    lcd.print("$");
    
  }
  if(content.substring(1) == idb){
    lcd.print("Cle de Banque");
    lcd.setCursor(0,1);
    lcd.print("Detectee");
    
    
  }
  delay(2000);
  clear();
  lcd.setCursor(0,0);
}
void GiveST (){
 if (content.substring(1) == id1){
          card1=card1+StartPmoney;
          
          lcd.print("carte1:");
          lcd.print(card1);
          lcd.print(" $");
          //delay(2000);
          //clear();
          
            
          return;
        }
        else if(content.substring(1) == id2){
          card2=card2+StartPmoney;
          
          lcd.print("carte2:");
          lcd.print(card2);
          lcd.print(" $");
          //delay(2000);
          //clear();
          
                   

          return;
        }
        else if(content.substring(1) == id3){
          card3=card3+StartPmoney;
          
          lcd.print("carte3:");
          lcd.print(card3);
          lcd.print(" $");
          //delay(2000);
          //clear();
          
               

          return;
        }
        else if(content.substring(1) == id4){
          card4=card4+StartPmoney;
          
          lcd.print("carte4:");
          lcd.print(card4);
          lcd.print(" $");
          //delay(2000);
          //clear();
         
          return;
        }
        else if(content.substring(1) == id5){
          card5=card5+StartPmoney;
          
          lcd.print("carte5:");
          lcd.print(card5);
          lcd.print(" $");
          //delay(2000);
         // clear();
          
                

          return;
        }
        else if(content.substring(1) == id6){
          card6=card6+StartPmoney;
          
          lcd.print("carte6:");
          lcd.print(card6);
          lcd.print(" $");
          //delay(2000);
          //clear();
        
          return;
        }
        else{
          noTone(piezoPin);
          lcd.print("Pas de Carte");
          delay(2000);
          clear();
          content="";
          return;
        }
        
        
     }

void everyPl(){
   if (content.substring(1) == id1){
          card1=card1-currentNum;
          card2=card2+currentNum;
          card3=card3+currentNum;
          card4=card4+currentNum;
          card5=card5+currentNum;
          card6=card6+currentNum;
          lcd.print("carte1:");
          lcd.print(card1);
          delay(2000);
          clear();
          
            
          return;
        }
        else if(content.substring(1) == id2){
          card1=card1+currentNum;
          card2=card2-currentNum;
          card3=card3+currentNum;
          card4=card4+currentNum;
          card5=card5+currentNum;
          card6=card6+currentNum;
          
          lcd.print("carte2:");
          lcd.print(card2);
          delay(2000);
          clear();
          
                   

          return;
        }
        else if(content.substring(1) == id3){
          card1=card1+currentNum;
          card2=card2+currentNum;
          card3=card3-currentNum;
          card4=card4+currentNum;
          card5=card5+currentNum;
          card6=card6+currentNum;
          
          lcd.print("carte3:");
          lcd.print(card3);
          delay(2000);
          clear();
          
               

          return;
        }
        else if(content.substring(1) == id4){
         card1=card1+currentNum;
          card2=card2+currentNum;
          card3=card3+currentNum;
          card4=card4-currentNum;
          card5=card5+currentNum;
          card6=card6+currentNum;
          
          lcd.print("carte4:");
          lcd.print(card4);
          delay(2000);
          clear();
         
          return;
        }
        else if(content.substring(1) == id5){
          card1=card1+currentNum;
          card2=card2+currentNum;
          card3=card3+currentNum;
          card4=card4+currentNum;
          card5=card5-currentNum;
          card6=card6+currentNum;
          
          lcd.print("carte5:");
          lcd.print(card5);
          delay(2000);
          clear();
          
                

          return;
        }
        else if(content.substring(1) == id6){
          card1=card1+currentNum;
          card2=card2+currentNum;
          card3=card3+currentNum;
          card4=card4+currentNum;
          card5=card5+currentNum;
          card6=card6-currentNum;
          
          lcd.print("carte6:");
          lcd.print(card6);
          delay(2000);
          clear();
        
          return;
        }
        else{
          noTone(piezoPin);
          lcd.print("Pas de Carte");
          delay(2000);
          clear();
          content="";
          return;
        }
}
