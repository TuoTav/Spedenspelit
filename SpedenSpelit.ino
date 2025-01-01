#include <Arduino.h>
//kytkinten variablet
const int buttonPin = 2;  
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 5;
const int buttonPin5 = 6;
volatile bool PINa=false;
volatile bool PINb=false;
volatile bool PINc=false;
volatile bool PINd=false;
volatile bool PINe=false;
unsigned long T1 = 0, T2 = 0, T3= 0,T4=0, T5=0, T6=0, T7=0, T8=0, T9=0, T10=0;
//segmentti näytön variablet
const int RESET_PIN = 12;         //pinni 10, master reset (active low)
const int SHIFT_CLOCK_PIN = 11;   //pinni 11, shift register clock input (SH_CP)
const int LATCH_CLOCK_PIN = 10;   //pinni 12, storage register clock input (ST_CP)
const int OUT_ENABLE_PIN = 9;     //pinni 13, output enable (active low)
const int SERIAL_INPUT_PIN = 8; 
//gamelogiikan variabelt
int randomNumbers[100];
int userNumbers[100];
bool timeToCheckGameStatus;
volatile int arrayMatchCounter;
volatile int speedIncrease;
volatile int randomCounter;
volatile int correctPressCount;
volatile int a;
bool stop=true;

void setup()
{
  
//alustetaan kaikki modulet
  
  initializeLeds();
  initializeDisplay();
  initButtonsAndButtonInterrupts();

}

void loop()
{
    T2 = millis();                //Loop() tarkistaa että debounce on mennyt läpi ja bool arvoa käytetään tämän merkkinä
  if( (T2-T1) >= 6 )
  {
    if(debounce())
     {

          PINa=true;
        
     }
    T1 = millis();
  }
      T3= millis();
    if( (T3-T4) >= 6 )
  {
     if (debounce2())
    {
  
           PINb=true;
        
    }

    T4 = millis();
  }
     T5= millis();
  if( (T5-T6) >= 6 )
  {
      if (debounce3())
     {

            PINc=true;
        
     }
      T6 = millis();
  }
      T7= millis();
if( (T7-T8) >= 6 )
  {
    if (debounce4())
   {
    
        PINd=true;
      
   }

    T8 = millis();
 }
 T9 = millis();
  if( (T9-T10) >= 6 )
  {
    if(debounce5())
     {
        
    
          PINe=true;
        
     }
    T10 = millis();
  }

if(timeToCheckGameStatus==true)               // tässä katsotaan että bool arvo on mennyt todeksi eli nappia on painettu
  {
          checkGame();
          timeToCheckGameStatus=false;
  }

}

ISR(PCINT2_vect) 
{
  if(digitalRead(buttonPin)==LOW&&PINa==true)               //Kytkinten ISR:ssä erotellaan mitä kytkintä on painettu ja käytetään bool arvoa että tiedetään debouncen olevan ohi
  {
    userNumbers[a]=2;                                       // Pinni numero lisätään arrayhin userNumbers
    a++;
    PINa=false;
    timeToCheckGameStatus=true;                             //Jokainen pelipinni asettaa timeToCheckGameStatus todeksi
    

    
  }
  if(digitalRead(buttonPin2)==LOW&&PINb==true)
  {
    userNumbers[a]=3;
    a++;
    PINb=false;
    timeToCheckGameStatus=true;
    
    
  }
  if(digitalRead(buttonPin3)==LOW&&PINc==true)
  {
    userNumbers[a]=4;
    a++;
    PINc=false;
    timeToCheckGameStatus=true;
    
    
  }
  if(digitalRead(buttonPin4)==LOW&&PINd==true)
  {
    userNumbers[a]=5;
    a++;
    PINd=false;
    timeToCheckGameStatus=true;
    
    
  }
  if(digitalRead(buttonPin5)==LOW&&PINe==true)
  { 

 
    startTheGame();                                             //buttonPin5 toimii aloitus nappina eli se kutsuu startTheGame()-funktiota
    PINe=false;

  }
  

}
void startTheGame()                                     //startTheGame()-funktio kutsuu initializeGame()- funktiota ja arpoo randomNumbers- arrayn 
{
  
  initializeGame();
  for(int i=0; i<100;i++)
  {
    Serial.println(randomNumbers[i]);
  }
  showResults(0);                                         // showResults nollataan jolloin pisteet nollaantuvat segmentti näytöltä 
  initializeTimer();                                          // timer alustetaan uudelleen sen otettu pois käytöstä
  stop=true;                                                
  
  
}


ISR(TIMER1_COMPA_vect)
{
  
 setLed(randomNumbers[randomCounter]);  
                  // Timer ISR:ssä katsotaan seuraava numero randomNumbers arraysta ja asetetaan se numero setLed()- funktiolla 
 if(speedIncrease==5)                                    // tarkistetaan jos speedIncrease variable viisi ja nopeutetaan sen mukaan peliä
 {
  OCR1A = OCR1A*0.9;
  speedIncrease=0;                                        //nollataan speedIncrease
 }
 
  
}



void initializeTimer(void)                              //alustetaan Timer1
{
	 cli(); 
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 15624; 
    TCCR1B |= (1 << WGM12); 
    TCCR1B |= (1 << CS12) | (1 << CS10); 
    TIMSK1 |= (1 << OCIE1A); 
    sei(); 
}

void checkGame()
{
  if(stop==true)                                                        //randomNumbers ja userNumbers arraytä verrataan keskenään jos stopTheGamea ei ole kutsuttu
  {
 if(userNumbers[arrayMatchCounter]==randomNumbers[arrayMatchCounter])
      {
        correctPressCount++;
        speedIncrease++;
      }
        else{stopTheGame();}                                            //jos tämä ei toteudu kutsutaan stopTheGamea

                  showResults(correctPressCount);                       //Näytetään tulos näytöllä
                  digitalWrite(OUT_ENABLE_PIN, LOW);
                  clearAllLeds();                                         //ledit pois päältä
                  arrayMatchCounter++;                                    // arrayMatchCounter incrementoi yhdellä jotta tiedetään mitä arrayn jäsentä verrata
       if(correctPressCount==99)                                          //jos piste määrä on 99 kutsutaan stopTheGamea
         {
                  stopTheGame();
         }
         
  }
 
  
}


void initializeGame()
{
    for(int i=0; i<100; i++)                                              //Luodaan randomNumbers taulukko kokonaan alussa
    {
      randomNumbers[i]=random(2,6);
    }
      correctPressCount=0;                                                    //nollataan kaikki pelin aikana kasvavat arvot
      speedIncrease=0;
      randomCounter=0;
      a=0;
      arrayMatchCounter=0;
}



void stopTheGame()                                                
{ 
  TIMSK1 = 0;                                               //otetaan timer1 pois käytöstä
  clearAllLeds();                                           //laitetaan ledit pois päältä
  stop=false;                                                //aktivoidaan lukko

}
void initButtonsAndButtonInterrupts(void)             //alustetaan pin change interrruptit ja käytettävät pinnit
{
  pinMode(buttonPin, INPUT_PULLUP) ;
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);

  PCICR  |=B00000100;
  
  PCMSK2 |=B01111100;
  

}
void initializeLeds(void)       //alustetaan analogPinnit ledien käyttöön
{
  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
}


void setLed(byte ledNumber)           //setLed()-funktiolla asetetaan ledi numeroilla 2-5
{
  randomCounter++;                    //ainakun timerin ISR:ssä kutsutaan setLediä randomCounter arvoa incrementoidaan
  // Tässä toteutetaan yksittäisen ledin sytytys
  switch (ledNumber) 
  {
    case 2:
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      digitalWrite(A4, LOW);
      digitalWrite(A5, LOW);
      break;
    case 3:
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      digitalWrite(A4, LOW);
      digitalWrite(A5, LOW);
      break;
    case 4:
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      digitalWrite(A4, HIGH);
      digitalWrite(A5, LOW);
      break;
    case 5:
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      digitalWrite(A4, LOW);
      digitalWrite(A5, HIGH);

      break;
    default:
     
      break;
  }
}

void clearAllLeds()       
 {
  // Tässä toteutetaan kaikkien ledien sammutus
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
}

void setAllLeds()
 {
  // Tässä toteutetaan kaikkien ledien sytytys
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  digitalWrite(A5, HIGH);
}
  //pinni 14, serial data input DS
 
const uint8_t digitPatterns[10][8] = { // bittikartta, jossa määritetty jokaiselle 8bittinen numerosarja. 
  {0, 0, 0, 0, 0, 0, 1, 1}, //  0 // koska käytetään common anode segmentti näyttöjä niin segmentti syttyy low tilassa.
  {1, 0, 0, 1, 1, 1, 1, 1}, //  1
  {0, 0, 1, 0, 0, 1, 0, 1}, //  2
  {0, 0, 0, 0, 1, 1, 0, 1}, //  3
  {1, 0, 0, 1, 1, 0, 0, 1}, //  4
  {0, 1, 0, 0, 1, 0, 0, 1}, //  5
  {0, 1, 0, 0, 0, 0, 0, 1}, //  6
  {0, 0, 0, 1, 1, 1, 1, 1}, //  7
  {0, 0, 0, 0, 0, 0, 0, 1}, //  8
  {0, 0, 0, 1, 1, 0, 0, 1}, //  9
};
 
void initializeDisplay(void) 
{ // kaikkien pinnien tilan määritys
  pinMode(RESET_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(LATCH_CLOCK_PIN, OUTPUT);
  pinMode(OUT_ENABLE_PIN, OUTPUT);
  pinMode(SERIAL_INPUT_PIN, OUTPUT);
 
  digitalWrite(RESET_PIN, HIGH); // 
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(LATCH_CLOCK_PIN, LOW);
  digitalWrite(OUT_ENABLE_PIN, LOW);
  digitalWrite(SERIAL_INPUT_PIN, LOW);
}
 
void writeByte(uint8_t number, bool last)//sarjamuunnin tutkii numeron bitit ja sytyttää tarvittaessa näytön.
 { 
  digitalWrite(LATCH_CLOCK_PIN, LOW);
 
  for (int i = 7; i >= 0; i--) {
    digitalWrite(SERIAL_INPUT_PIN, digitPatterns[number][i]);
    digitalWrite(SHIFT_CLOCK_PIN, HIGH); 
    digitalWrite(SHIFT_CLOCK_PIN, LOW);
  }
 
  digitalWrite(LATCH_CLOCK_PIN, HIGH);
 
  if (last) {
    digitalWrite(OUT_ENABLE_PIN, HIGH);
  }
}
 
void writeHighAndLowNumber(uint8_t tens, uint8_t ones) //writebyte funktio
{ 
  writeByte(tens, false);
  
  writeByte(ones, true);
  
  
  
}
 
void showResults(byte number) //tämä kutsuu kyseistä funktiota ja  näyttää numerot näytöillä
{ 
  
  writeHighAndLowNumber(number / 10, number % 10);
  
}

bool debounce(void)       //debounce funktiossa hyödynnetään bitshiftingiä ja verrataan bitwise OR:in avulla HIGH ja LOW arvoista muodostuvaa bitti sekuenssia maskiin 0xFFFF
{
    static uint16_t btnState = 0;
    btnState = (btnState<<1) | (digitalRead(buttonPin));
    return (btnState == 0xFFFF);
}
bool debounce2(void)
{
  static uint16_t btnState = 0;
  btnState = (btnState<<1) | (digitalRead(buttonPin2));
  return (btnState == 0xFFFF);
}
bool debounce3(void)
{
  static uint16_t btnState = 0;
 btnState = (btnState<<1) | (digitalRead(buttonPin3));
  return (btnState == 0xFFFF);
}
bool debounce4(void)
{
  static uint16_t btnState = 0;
  btnState = (btnState<<1) | (digitalRead(buttonPin4));
  return (btnState == 0xFFFF);
}
bool debounce5(void)
{
  static uint16_t btnState = 0;
  btnState = (btnState<<1) | (digitalRead(buttonPin5));
  return (btnState == 0xFFFF);
}