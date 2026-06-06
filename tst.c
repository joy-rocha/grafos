//código teste horus


// Bibliotecas para o GPS NEO-6M 
#include <TinyGPS.h>

// Bibliotecas para a MPU6050
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; 
TinyGPS GPS; 

// ADAPTAÇÃO PARA ESP8266: Usando os pinos D1 (GPIO5) e D2 (GPIO4)
SoftwareSerial SerialGPS(5, 4);  

// Variáveis de armazenamento do GPS
float lat, lon, vel;
unsigned short sat;

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(9600);

  // ADAPTAÇÃO PARA ESP8266: Inicializa o barramento I2C nos pinos D3 (SDA) e D4 (SCL)
  Wire.begin(5, 4); 

  // Inicialização do MPU6050
  if (!mpu.begin()) {
    Serial.println("Falha ao conectar o módulo MPU6050");
    while (1) {
      delay(10);
    }
  }

  // Configurações de sensibilidade
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); 
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); 
  Serial.println("Configurações aplicadas com sucesso no ESP8266!");
}

void loop() {
  // 1. Coleta dados do MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  imprimirDados(a, g, temp);   

  // 2. Processa dados do GPS
  while (SerialGPS.available()) {
    if (GPS.encode(SerialGPS.read())) {
      GPS.f_get_position(&lat, &lon);

      Serial.print("Latitude: ");  Serial.println(lat, 6);
      Serial.print("Longitude: "); Serial.println(lon, 6);

      vel = GPS.f_speed_kmph();
      Serial.print("Velocidade: "); Serial.println(vel);

      sat = GPS.satellites();
      if (sat != TinyGPS::GPS_INVALID_SATELLITES) {
        Serial.print("Satelites: "); Serial.println(sat);
      }
      Serial.println("-------------------------");
    }
  }
  
  delay(500); 
}

void imprimirDados(sensors_event_t acel, sensors_event_t giro, sensors_event_t term) {
  Serial.print("Aceleração X: "); Serial.print(acel.acceleration.x);
  Serial.print(", Y: ");          Serial.print(acel.acceleration.y);
  Serial.print(", Z: ");          Serial.println(acel.acceleration.z);

  Serial.print("Temperatura: ");  Serial.print(term.temperature);
  Serial.println(" °C\n");
}
