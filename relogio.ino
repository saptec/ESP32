#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configurações do Wi-Fi
const char* ssid     = "SAMUEL";
const char* password = "sap@123456";

// Configurações do NTP (Fuso Horário de Brasília é -3 horas = -10800 segundos)
const long utcOffsetInSeconds = -10800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Inicializa o LCD (Endereço comum 0x27, 16 colunas, 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }

  lcd.clear();
  timeClient.begin();
}

void loop() {
  timeClient.update();
  
   // 1. Obtém o Epoch Time (Unix Timestamp)
  unsigned long epochTime = timeClient.getEpochTime();
  // 2. Converte para a estrutura "tm" do C
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  // 3. Extrai as variáveis individualmente
  int dia  = ptm->tm_mday;
  int mes  = ptm->tm_mon + 1;    // Meses começam em 0
  int ano  = ptm->tm_year + 1900; // Anos contados a partir de 1900

  // 4. Formatação Limpa (Evita números "grudados" no final)
  // O buffer de 20 caracteres garante espaço para "DD/MM/AAAA HH:MM:SS"
  char dataFormatada[20];
  sprintf(dataFormatada, "%02d/%02d/%04d", dia, mes, ano);


  lcd.setCursor(0, 0);
  lcd.print("Data: ");
  // lcd.print(timeClient.getFormattedTime().substring(0, 5)); // Exibe parte da data ou status
  lcd.print(dataFormatada);

  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  lcd.print(timeClient.getFormattedTime()); // Exibe HH:MM:SS

  delay(1000);
}
