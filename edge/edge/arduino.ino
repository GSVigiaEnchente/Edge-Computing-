#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// === PINOS ===
#define DHTPIN 2
#define DHTTYPE DHT22
#define LED_VERDE 11
#define LED_AMARELO 12
#define LED_VERMELHO 13
#define BUZZER 8

// === OBJETOS ===
DHT_Unified dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === VARIÁVEIS DE MÉDIA ===
int leituraCount = 0;
float somaUmid = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Iniciado");

  Serial.println("Sistema Iniciado");
  Serial.println("-------------------------");
}

void loop() {
  sensors_event_t umidEvent;
  dht.humidity().getEvent(&umidEvent);

  float umid = umidEvent.relative_humidity;

  if (!isnan(umid)) {
    somaUmid += umid;
    leituraCount++;

    Serial.println("Leitura feita");
    Serial.print("Umid: ");
    Serial.println(umid);
    Serial.print("Contagem de leituras: ");
    Serial.println(leituraCount);
  } else {
    Serial.println("Erro ao ler sensor DHT.");
    return;
  }

  if (leituraCount >= 5) {
    float umidMedia = somaUmid / leituraCount;

    Serial.println("==== MÉDIAS CALCULADAS ====");
    Serial.print("Umid Média: ");
    Serial.println(umidMedia);
    Serial.println("-------------------------");

    // Reset para próxima média
    leituraCount = 0;
    somaUmid = 0;

    // === EXIBE UMIDADE COM A LÓGICA DE LED + BUZZER ===
    lcd.clear();
    lcd.setCursor(0, 0);

    // Desliga todos os LEDs e buzzer antes de acionar o correto
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    noTone(BUZZER);

    if (umidMedia < 50) {
      digitalWrite(LED_VERDE, HIGH);
      lcd.print("Umidade Baixa");
    } else if (umidMedia <= 70) {
      digitalWrite(LED_AMARELO, HIGH);
      tone(BUZZER, 1000);
      lcd.print("Umidade Media");
    } else {
      digitalWrite(LED_VERMELHO, HIGH);
      tone(BUZZER, 1000);
      lcd.print("Umidade Alta");
    }

    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(umidMedia, 1);
    lcd.print(" %");
    delay(2000);

    // Desliga buzzer no final do ciclo
    noTone(BUZZER);
  }

  delay(1000); // Aguarda entre as leituras
}