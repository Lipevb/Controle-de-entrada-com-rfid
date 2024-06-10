#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27,20,4); // Inicializa o display no endereco 0x27 com as respectivas dimensões
const int rele = 7; // Define o pino do rele na saida digital 13
const int buzz = 6; // Define o pino do buzzer na saida digital 12

char st[20];

void setup()
{
  Serial.begin(9600); // Inicia a serial
  SPI.begin(); // Inicia SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  lcd.init(); // Inicia o display lcd
  pinMode (rele, OUTPUT); // Define o pino rele como saida
  pinMode(buzz, OUTPUT); // Define o pino buzz como saida
}
 
void loop()
{
  mensageminicial();
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  if (conteudo.substring(1) == "F3 D7 04 10") //UID 1 - Chaveiro
  {
    limparlcd();
    mensagementradapermitida();
    ativarrele();
    delay(500);
    limparlcd();
  }
  if (conteudo.substring(1) == "A3 47 EE 0D") //UID 2 - Cartao
  {
    limparlcd();
    mensagementradanegada();
    delay(3000);
    limparlcd();
  }
  if (conteudo.substring(1) == "04 A4 BC C2 5E 71 80") //UID 2 - Cartao
  {
    limparlcd();
    mensagementradapermitida();
    ativarrele();
    delay(500);
    limparlcd();
  }  
} 

void mensageminicial()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(8,0);
  lcd.print("Ola!");
  lcd.setCursor(1,1);
  lcd.print("Aproxime o cartao");
  lcd.setCursor(5,2);
  lcd.print("do leitor!");
}

void mensagementradapermitida(){
  lcd.setBacklight(HIGH);
  lcd.setCursor(1,0);
  lcd.print("Entrada permitida!");
  lcd.setCursor(3,1);
  lcd.print("Seja bem vindo!");
  lcd.setCursor(0,2);
  lcd.print("Deixe o sapato fora");
  lcd.setCursor(1,3);
  lcd.print("da casa! Obrigado!");
}

void mensagementradanegada(){
  lcd.setBacklight(HIGH);
  lcd.setCursor(2,0);
  lcd.print("Entrada negada!");
  lcd.setCursor(1,1);
  lcd.print("Por favor, contate");
  lcd.setCursor(2,2);
  lcd.print("o proprietario!");
}

void limparlcd(){
  lcd.clear();
  lcd.setBacklight(LOW);  
}
void ativarrele(){
  digitalWrite (rele, HIGH);
  tone(buzz, 1500); // Ligar o buzzer com frequencia de 1500Hz
  delay(1000);
  noTone(buzz); // Desliga o buzzer
  delay(3000);
  digitalWrite (rele, LOW);
}