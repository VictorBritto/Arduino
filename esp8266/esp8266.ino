#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "Marta";
const char* password = "13070211";

const char* supabase_url = "https://vthvzyigxpzwfijeijsi.supabase.co/rest/v1/clima";  // URL da sua tabela
const char* supabase_api_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InZ0aHZ6eWlneHB6d2ZpamVpanNpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDY2NzU3MjMsImV4cCI6MjA2MjI1MTcyM30.LdXgGVo_je8d1gmFdruv4kpx2jCDS22pOiiNkYQwu1M";
const char* fingerprint = "9D:F5:E6:1B:14:EA:7B:57:95:08:63:BA:BA:3A:A3:5B:C4:B6:75:38";

unsigned long ultimoEnvio = 0;
const unsigned long intervalo = 10000;

void setup() {
  Serial.begin(9600);  // Comunicação com ATmega328p
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("WiFi conectado.");
}

void loop() {
  if (millis() - ultimoEnvio >= intervalo) {
    if (Serial.available()) {
      String leitura = Serial.readStringUntil('\n');
      leitura.trim();

      if (leitura.length() > 0) {
        float temp = 0.0, umid = 0.0;

        int sep = leitura.indexOf(',');
        if (sep != -1) {
          temp = leitura.substring(0, sep).toFloat();
          umid = leitura.substring(sep + 1).toFloat();

          enviarParaSupabase(temp, umid);
        }
      }
    }

    ultimoEnvio = millis();
  }
}

void enviarParaSupabase(float temperatura, float umidade) {
  WiFiClientSecure client;
  client.setFingerprint(fingerprint);

  if (!client.connect("vthvzyigxpzwfijeijsi.supabase.co", 443)) {
    Serial.println("Falha na conexão SSL.");
    return;
  }

  HTTPClient http;
  http.begin(client, supabase_url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabase_api_key);
  http.addHeader("Authorization", "Bearer " + String(supabase_api_key));

  // JSON com os campos esperados na sua tabela
  String json = "{\"temperatura\": " + String(temperatura, 2) + ", \"umidade\": " + String(umidade, 2) + "}";

  int httpCode = http.POST(json);

  if (httpCode > 0) {
    Serial.printf("Código Supabase: %d\n", httpCode);
    Serial.println(http.getString());
  } else {
    Serial.printf("Erro: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
