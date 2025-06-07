#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int potPin = A0;
const int buzzerPin = 8;

const int ledVermelho = 13;
const int ledAmarelo = 12;
const int ledVerde = 11;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(potPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin);
  int nivel = map(potValue, 0, 1023, 0, 300);
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler DHT22");
    return;
  }

  String statusTexto;

  // LEDs e buzzer
  if (nivel < 100) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    noTone(buzzerPin);
    statusTexto = "Seguro";
  } else if (nivel < 200) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    tone(buzzerPin, 1000);
    statusTexto = "Atencao";
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    tone(buzzerPin, 2000);
    statusTexto = "ALERTA";
  }

  // Serial output
  Serial.println("Nivel: " + String(nivel) + "cm");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println("°C");
  Serial.print("Umidade: ");
  Serial.print(umidade, 0);
  Serial.println("%");
  Serial.println("Status: " + statusTexto);
  Serial.println();

  // LCD - Tela 1: Nivel e Status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nivel: ");
  lcd.print(nivel);
  lcd.print("cm");

  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(statusTexto);

  delay(2000);

  // LCD - Tela 2: Temperatura e Umidade
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatura, 1);
  lcd.print((char)223); // símbolo de grau
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Umidade: ");
  lcd.print(umidade, 0);
  lcd.print("%");

  delay(2000);
}
