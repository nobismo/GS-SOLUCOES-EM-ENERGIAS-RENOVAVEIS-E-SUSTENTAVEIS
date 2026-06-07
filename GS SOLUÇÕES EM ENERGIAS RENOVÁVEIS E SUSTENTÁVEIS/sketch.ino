#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos
const int tempPin = A0;
const int energiaPin = A1;

const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;
const int buzzer = 5;

unsigned long trocaTela = 0;
bool tela = false;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("CAPSULA ORION");
  lcd.setCursor(0, 1);
  lcd.print("INICIANDO...");
  delay(2000);
}

void loop() {

  // Temperatura simulada (0 a 50°C)
  int leituraTemp = analogRead(tempPin);
  float temperatura = map(leituraTemp, 0, 1023, 0, 50);

  // Energia simulada (0 a 100%)
  int leituraEnergia = analogRead(energiaPin);
  int energia = map(leituraEnergia, 0, 1023, 0, 100);

  // Comunicação simulada
  String comunicacao;

  if (energia > 30) {
    comunicacao = "OK";
  } else {
    comunicacao = "FALHA";
  }

  // Reset dos indicadores
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  noTone(buzzer);

  String status = "OPERANDO";

  // ALERTA DE TEMPERATURA
  if (temperatura > 40) {
    status = "SUPERAQUEC.";
    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 1000);
  }

  // ALERTA DE ENERGIA
  else if (energia < 20) {
    status = "ECONOMIA";
    digitalWrite(ledAmarelo, HIGH);
    tone(buzzer, 700);
  }

  // NORMAL
  else {
    status = "OPERANDO";
    digitalWrite(ledVerde, HIGH);
  }

  // Troca de tela a cada 3 segundos
  if (millis() - trocaTela > 3000) {
    tela = !tela;
    trocaTela = millis();
  }

  lcd.clear();

  if (!tela) {

    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temperatura, 1);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Ener:");
    lcd.print(energia);
    lcd.print("%");

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Com:");
    lcd.print(comunicacao);

    lcd.setCursor(0, 1);
    lcd.print(status);
  }

  // Monitor Serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C | Energia: ");
  Serial.print(energia);
  Serial.print("% | Comunicacao: ");
  Serial.print(comunicacao);
  Serial.print(" | Status: ");
  Serial.println(status);

  delay(500);
}