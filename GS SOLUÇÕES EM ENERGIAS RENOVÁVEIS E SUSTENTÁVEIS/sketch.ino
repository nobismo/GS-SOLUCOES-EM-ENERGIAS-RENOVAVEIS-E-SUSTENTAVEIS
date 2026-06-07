#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  // Leitura do sensor NTC
  int leituraTemp = analogRead(tempPin);

  // Faixa simulada de -20°C a 80°C
  float temperatura = 80.0 - ((float)leituraTemp * 100.0 / 1023.0);

  // Energia simulada por potenciômetro
  int leituraEnergia = analogRead(energiaPin);
  int energia = map(leituraEnergia, 0, 1023, 0, 100);

  // Comunicação
  String comunicacao = (energia > 30) ? "OK" : "FALHA";

  // Reset dos indicadores
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  noTone(buzzer);

  String status;
  String acao;

  // FRIO EXTREMO
  if (temperatura < 0) {

    status = "CRITICO";
    acao = "AQUECIMENTO";

    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 1200);

  }

  // SUPERAQUECIMENTO
  else if (temperatura > 40) {

    status = "CRITICO";
    acao = "RESFRIAMENTO";

    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 1000);

  }

  // ENERGIA BAIXA
  else if (energia < 20) {

    status = "ECONOMIA";
    acao = "DESLIGA MODULOS";

    digitalWrite(ledAmarelo, HIGH);
    tone(buzzer, 700);

  }

  // NORMAL
  else {

    status = "NORMAL";
    acao = "OPERANDO";

    digitalWrite(ledVerde, HIGH);
  }

  // Troca de tela a cada 3 segundos
  if (millis() - trocaTela >= 3000) {
    tela = !tela;
    trocaTela = millis();
  }

  lcd.clear();

  if (!tela) {

    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print((int)temperatura);
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
    lcd.print(acao);
  }

  // Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C | Energia: ");
  Serial.print(energia);
  Serial.print("% | Comunicacao: ");
  Serial.print(comunicacao);
  Serial.print(" | Status: ");
  Serial.print(status);
  Serial.print(" | Acao: ");
  Serial.println(acao);

  delay(500);
}