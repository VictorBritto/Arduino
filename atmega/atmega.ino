#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
unsigned long ultimoEnvio = 0;
const unsigned long intervalo = 10000;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  if (millis() - ultimoEnvio >= intervalo) {
    float temp = dht.readTemperature();
    float umid = dht.readHumidity();

    if (!isnan(temp) && !isnan(umid)) {
      // Envia os dados como uma única string: "temp,umid"
      Serial.print(temp);
      Serial.print(",");
      Serial.println(umid);
    }

    ultimoEnvio = millis();
  }
}
