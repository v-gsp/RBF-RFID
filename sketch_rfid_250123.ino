// RFID - Leitor 
// Vitor Gasparetto
// Apex Rocketry
// Testado com um Arduino Uno
// 25/01/2023

// Explicar protocolo SPI
#include <SPI.h> // protocolo SPI

// Explicar bibilioteca do rfid
#include <MFRC522.h> // biblioteca do rfid

// PINOS
// SCK = 13
// MOSI = 11
// MISO = 12
// (esses sao padrao da biblioteca)
#define SS_PIN 10 // slave select SDA
#define RST_PIN 9 // reset

MFRC522 mfrc522(SS_PIN, RST_PIN); //instancia do MFRC

// Cartoes para comparar no RBF
// (compara so com o 1 byte e nao a UID inteira
//  pq em alguns cartoes os outros bytes variam)
String card1 = "83"; // cartao 1 (aeroestruturas)
String card2 = "73"; // cartao 2 (propulsao)
String card3 = "32"; // cartao 3 (eletronica)
String cardX = "3b"; // cartao X (ABORTAR!)

// Booleanas que controlam quais cartoes ja foram lidos
bool RBF1 = false;
bool RBF2 = false;
bool RBF3 = false;

// Booleana que controla se o foguete está pronto pra lancar
// (liga os reles e o que seja)
bool READY = false;

// Pino do Relé ou seja la o q for a saida
const int RelePin = 7;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");

  pinMode(RelePin, OUTPUT); // seta o pino do rele como output
  digitalWrite(RelePin, LOW);
  
  SPI.begin();  // inicia a biblioteca/protocolo spi
  mfrc522.PCD_Init(); // inicia o leitor
}

void loop() {
      // procura por novos cartões
    if ( ! mfrc522.PICC_IsNewCardPresent() )
      { return; }
      //seleciona o cartao
    if ( ! mfrc522.PICC_ReadCardSerial() )
      { return; }

      // testa se o cartão eh um dos cartoes validos
    averiguar();

      // imprime o valor do cartao lido
    cartaoprint();

      // AQUI COLOCA O QUE SAI COM O READY
   if (! READY) {
      digitalWrite(RelePin, LOW);
    }
   else {
      digitalWrite(RelePin, HIGH);
    }

      //delayzim
   delay(600);
}



void cartaoprint() {
  // mostra UID na serial como uma string
  String conteudo = "";
  byte letra;

  // itera pelos bytes, vai montando a string
  for (byte i = 0; i < mfrc522.uid.size; i++)
  { conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

   // imprime na serial quais já estão ligadas
   Serial.print(RBF1, DEC);
   Serial.print(RBF2, DEC);
   Serial.print(RBF3, DEC);
   Serial.print(READY, DEC);
   Serial.print(" :: ");
   Serial.print(conteudo);  // UID do cartão
   Serial.println();
}



void averiguar() {
  Serial.println("Averiguando...");

    // isso aqui ta feiasso kkkkk
    if (RBF1 = true) {
      if (RBF2 = true) {
        if (RBF3 = true) {
          READY = true;
        }
      }
    }
  
  if (String(mfrc522.uid.uidByte[0], HEX) == card1) {
    Serial.println("Valido: RBF 1 ");
    RBF1 = true;
   }
  else if (String(mfrc522.uid.uidByte[0], HEX) == card2) {
    Serial.println("Valido: RBF 2 ");
    RBF2 = true;
   }
  else if (String(mfrc522.uid.uidByte[0], HEX) == card3) {
    Serial.println("Valido: RBF 3 ");
    RBF3 = true;
   }
  else if (String(mfrc522.uid.uidByte[0], HEX) == cardX) {
    Serial.println("ABORTAR!!! ");
    RBF3 = false;
    RBF2 = false;
    RBF1 = false;
   }
  else {
    Serial.println("Cartao Invalido! ");
  }
}
