  // DFT Le 06/05/2022 16H11 Fonctionne
  // J'ai décidé de mettre les 2 fonctions son sur 2 ATtiny85 distincts
 
  
  #include "Arduino.h"
  #include <SoftwareSerial.h>
  #include "DFRobotDFPlayerMini.h"
  
  #define TX 0
  
  //#define BrocheAnalog A1
  
  #define BPKlaxon 3


  bool EtatKlaxon = LOW;
  bool LastEtatKlaxon = LOW;
  
  SoftwareSerial mySoftwareSerial (1,0);
 
  DFRobotDFPlayerMini myDFPlayer; 
  byte volumeLevel = 0;
     
  void setup()
  {
  
  pinMode (BPKlaxon, INPUT);
  
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin (mySoftwareSerial);
  }
 
  void loop()
  {
  
  EtatKlaxon = digitalRead(BPKlaxon);
  
  if (EtatKlaxon == HIGH)
  {
  delay(5);
  pinMode (TX, OUTPUT);                                                                    // Ouverture de la liaison série pour passer des commandes sur DFPlayer01
  myDFPlayer.volume(30);
  
  delay(100);
  myDFPlayer.play(8);
  delay(1240);
                                                                              // L'état du moteur est déclaré à l'arrêt 
  }
  
 
  }
  
  
