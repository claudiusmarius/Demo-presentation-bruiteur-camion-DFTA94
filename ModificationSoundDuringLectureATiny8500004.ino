 // EN COURS le 02/05/2022 20H02, ça fonctionne les 2 entrées étant censées arriver  de ports numériques d'autres MCU, elles sont tirées à la masse via des résistances
 // Plus d'actionnement aléatoire en démarrage, uniquement les 2BP agissent
 // Il faudra gérer les cas d'association de BP non conformes
  
  // c'est le bon il faut retarder la mise à zero du DAC pour l'arrêt moteur DFT 03/05/2022 18H17
  // En fait non il fallait mettre une temp de durée sur arrêt moteur (piste 11) DFT Le 04/052022 15H41
  
  #include "Arduino.h"
  #include <SoftwareSerial.h>
  #include "DFRobotDFPlayerMini.h"
  
  #define TX 0
  
  #define BrocheAnalog A1
  
  #define BPStart 3
  
  #define BPStop 4

     
   SoftwareSerial mySoftwareSerial (1,0);
 

  DFRobotDFPlayerMini myDFPlayer; 
  byte volumeLevel = 0;
  bool StateBPStop = LOW;                                                          // état du BP Stop ou Stop géré par le DFPlayer01
  bool LastStateBPStop = LOW;
  
  bool StateBPStart = LOW;                                                          // état du BP Stop ou Stop géré par le DFPlayer01
  bool LastStateBPStart = LOW;
  
  bool EnMarche = LOW;
  
  float ValueAnalog;
  float ValueAnalog5;
   
  void setup()
  {
  
  EnMarche = LOW;                                                                        // Dés que l'ATtiny84 est démarré, le moteur est déclaré en marche
  pinMode (TX, OUTPUT); 
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin (mySoftwareSerial);
  
  pinMode (BPStop, INPUT);
  pinMode (BPStart, INPUT);
  
  volumeLevel = 0;                                                                  // Niveau sonore pour jouer le son du démarreur
  myDFPlayer.volume(0);
  delay(5);
  /*myDFPlayer.play(7);       //7                                                       // Jouer la piste 7 du DFPlayer01 une seule fois (son du démarreur)                                     
  
  delay (1240); 
  
  myDFPlayer.volume(0);
  delay(5);
  myDFPlayer.loop(6);                                     //---------------------------------------------------Sélection de la piste 6 et Lecture en boucle
  //delay (5);                                           //----------------------------------------------------------Tempo pour nécessaire à l'éxécution de la sélection de la piste
  pinMode (TX, INPUT_PULLUP);  */                           //---------------------------------------------Une fois la piste sélectionnée, le port TX est positionné en haute impédance
  
 
  }
 
  void loop()
  {
  delay(5);
  
  pinMode (TX, OUTPUT);                                                                  // Le port TX1 est ouvert (positionné en sortie)
  //volumeLevel = map(analogRead(BrocheAnalog), 0, (1023*3.3/5), 3, 30);                              // Calibrage du niveau sonore en fonction de la valeur du potentiomètre
  //volumeLevel = map(analogRead(BrocheAnalog), 0, 1023, 3, 30);

 
  
  delay(5);
  
  //volumeLevel = map(5*(ValueAnalog/3.3), 0, 1023, 3, 30);
  
  ValueAnalog = analogRead (BrocheAnalog);
  
  ValueAnalog5 = (5/3.3)*(ValueAnalog);
  volumeLevel = map(ValueAnalog5, 0, 1023, 3, 30);
  
  myDFPlayer.volume(volumeLevel);  
  delay(5);
  pinMode (TX, INPUT_PULLUP); 
  
  
  StateBPStop = digitalRead(BPStop); 
  StateBPStart = digitalRead(BPStart); 
  
  //digitalRead (BPStop);

  if (LastStateBPStop != StateBPStop)
  {
  if (StateBPStop == LOW)
  {
  LastStateBPStop = StateBPStop;
  EnMarche = LOW;
  }
  }

  if (LastStateBPStart != StateBPStart)
  {
  if (StateBPStart == LOW)
  {
  LastStateBPStart = StateBPStart;
  EnMarche = HIGH;
  }
  }

  if (StateBPStop == HIGH && EnMarche == HIGH )
  {
  delay(5);
  pinMode (TX, OUTPUT);                                                                    // Ouverture de la liaison série pour passer des commandes sur DFPlayer01
  myDFPlayer.volume(30);
  delay(5);
  myDFPlayer.disableLoopAll(); // Ne pas laisser cette ligne évite un léger blanc sonore indésirable// Comme une demande de Stop est interprétée, désactivation de toutes les pistes en cours sur le DFPlayer01
  delay(100);
  myDFPlayer.play(11);                                                                    // Jouer la piste 11 du DFPlayer01 (son de l'arrêt moteur)
  pinMode (TX, INPUT_PULLUP);
  delay (2000);    ///////////////                                                                            
  EnMarche = LOW;                                                                             // L'état du moteur est déclaré à l'arrêt 
  }


  if (StateBPStart == HIGH && EnMarche == LOW )
  {
  delay(5);
  pinMode (TX, OUTPUT);                                                                    // Ouverture de la liaison série pour passer des commandes sur DFPlayer01
  myDFPlayer.volume(30);
  //delay(5);
  //myDFPlayer.disableLoopAll(); // Ne pas laisser cette ligne évite un léger blanc sonore indésirable// Comme une demande de Stop est interprétée, désactivation de toutes les pistes en cours sur le DFPlayer01
  delay(100);
  myDFPlayer.play(7);
  delay(1240);
  
  myDFPlayer.volume(volumeLevel);
  myDFPlayer.play(6);                                                                    // Jouer la piste 11 du DFPlayer01 (son de l'arrêt moteur)
 // myDFPlayer.play(0066); 
  
  pinMode (TX, INPUT_PULLUP);
  delay (5);                                                                                
  EnMarche = HIGH;                                                                             // L'état du moteur est déclaré à l'arrêt 
  }

  
  }
