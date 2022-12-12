#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   5
#define DATA_PIN  3
#define CS_PIN    4

const int buzzer = 6;

// Hardware SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

struct signal{
byte score_1 = 0;
byte score_2 = 0;
byte a = 0;
byte b = 0;
byte c = 0;
byte d = 0;
};

signal terima;

const uint64_t pipeIn = 0xE9E8F0F0E1LL;

RF24 radio(7, 53); 
#define mulai 100
#define berhenti 101

bool angka_0[7] = {1,1,1,1,1,1,0};
bool angka_1[7] = {0,1,1,0,0,0,0}; 
bool angka_2[7] = {1,1,0,1,1,0,1}; 
bool angka_3[7] = {1,1,1,1,0,0,1}; 
bool angka_4[7] = {0,1,1,0,0,1,1}; 
bool angka_5[7] = {1,0,1,1,0,1,1}; 
bool angka_6[7] = {1,0,1,1,1,1,1}; 
bool angka_7[7] = {1,1,1,0,0,0,0}; 
bool angka_8[7] = {1,1,1,1,1,1,1}; 
bool angka_9[7] = {1,1,1,1,0,1,1};  

int digit_1[7] = {19,18,17,16,15,14,2};
int digit_2[7] = {48,46,44,42,40,38,36};
int digit_3[7] = {A9,A10,A11,A12,A13,A14,A15};
int digit_4[7] = {A1,A2,A3,A4,A6,A5,A7};

unsigned long previousMillis = 0;        // will store last time 7 segment was updated
const long interval = 1;           // Harusnya 1000
int a = 0, b = 0, c = 0, d = 0;

int buff_score_1 = 0, buff_score_2 = 0;
long panjang;
int buff_waktu = berhenti, waktustate = berhenti;

void setup() {
    
  // put your setup code here, to run once:
  //Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();

  //Mulai Dot Matrix
  P.begin();

  //installasi pin buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  
  // digit 1
  for(int i = 0; i<7; i++){
      pinMode(digit_1[i], OUTPUT);
    }

  //digit 2
  for(int i = 0; i<7; i++){
      pinMode(digit_2[i], OUTPUT);
    } 

  //digit 3
  for(int i = 0; i<7; i++){
      pinMode(digit_3[i], OUTPUT);
    }

  //digit 4
  for(int i = 0; i<7; i++){
      pinMode(digit_4[i], OUTPUT);
    }

  while(millis() < 3000){}
}

void loop() {
  //put your main code here, to run repeatedly:
  recvData();

  if(terima.score_1 != buff_score_1 || terima.score_2 != buff_score_2){digitalWrite(buzzer,LOW); delay(200); digitalWrite(buzzer,HIGH); buff_score_1 = terima.score_1; buff_score_2 = terima.score_2;}
 

  if(terima.d == 3 && terima.c == 0 && terima.b == 0 && terima.a == 0){panjang = millis();}
  if(terima.d == 0 && terima.c == 0 && terima.b == 0 && terima.a == 1){panjang = millis();}

  if(millis() - panjang < 3000){digitalWrite(buzzer,LOW);} else{digitalWrite(buzzer,HIGH);}

  
  score_display(1,terima.score_1/10%10);
  score_display(2,terima.score_1%10);
  score_display(3,terima.score_2/10%10);
  score_display(4,terima.score_2%10);
  
  P.print((String)terima.d+(String)terima.c+" : "+(String)terima.b+(String)terima.a);
}

void score_display(unsigned int digit_state, unsigned int angka_state){
  if(digit_state == 1){
     if(angka_state == 0){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_0[i]);}}
     else if(angka_state == 1){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_1[i]);}}
     else if(angka_state == 2){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_2[i]);}}
     else if(angka_state == 3){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_3[i]);}}
     else if(angka_state == 4){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_4[i]);}}
     else if(angka_state == 5){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_5[i]);}}
     else if(angka_state == 6){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_6[i]);}}
     else if(angka_state == 7){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_7[i]);}}
     else if(angka_state == 8){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_8[i]);}}
     else if(angka_state == 9){for(int i= 0; i < 7; i++){digitalWrite(digit_1[i], angka_9[i]);}}
    }

  else if(digit_state == 2){
     if(angka_state == 0){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_0[i]);}}
     else if(angka_state == 1){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_1[i]);}}
     else if(angka_state == 2){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_2[i]);}}
     else if(angka_state == 3){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_3[i]);}}
     else if(angka_state == 4){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_4[i]);}}
     else if(angka_state == 5){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_5[i]);}}
     else if(angka_state == 6){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_6[i]);}}
     else if(angka_state == 7){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_7[i]);}}
     else if(angka_state == 8){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_8[i]);}}
     else if(angka_state == 9){for(int i= 0; i < 7; i++){digitalWrite(digit_2[i], angka_9[i]);}}
    }

  else if(digit_state == 3){
     if(angka_state == 0){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_0[i]);}}
     else if(angka_state == 1){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_1[i]);}}
     else if(angka_state == 2){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_2[i]);}}
     else if(angka_state == 3){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_3[i]);}}
     else if(angka_state == 4){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_4[i]);}}
     else if(angka_state == 5){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_5[i]);}}
     else if(angka_state == 6){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_6[i]);}}
     else if(angka_state == 7){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_7[i]);}}
     else if(angka_state == 8){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_8[i]);}}
     else if(angka_state == 9){for(int i= 0; i < 7; i++){digitalWrite(digit_3[i], angka_9[i]);}}
    }
    
  else if(digit_state == 4){
     if(angka_state == 0){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_0[i]);}}
     else if(angka_state == 1){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_1[i]);}}
     else if(angka_state == 2){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_2[i]);}}
     else if(angka_state == 3){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_3[i]);}}
     else if(angka_state == 4){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_4[i]);}}
     else if(angka_state == 5){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_5[i]);}}
     else if(angka_state == 6){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_6[i]);}}
     else if(angka_state == 7){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_7[i]);}}
     else if(angka_state == 8){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_8[i]);}}
     else if(angka_state == 9){for(int i= 0; i < 7; i++){digitalWrite(digit_4[i], angka_9[i]);}}
    }
  }

void recvData(){
  while ( radio.available() ) {
    radio.read(&terima, sizeof(signal));
  }

//  Serial.print(terima.score_1);
//  Serial.print("    ");
//  Serial.print(terima.score_2);
//  Serial.print("    ");
//  Serial.print(terima.a);
//  Serial.print("    ");
//  Serial.print(terima.b);
//  Serial.print("    ");
//  Serial.print(terima.c);
//  Serial.print("    ");
//  Serial.print(terima.d);
//  Serial.println("    ");
}
