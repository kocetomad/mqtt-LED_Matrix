#include <SPI.h>
#include "LedMatrix.h"
#include "WiFi.h"
#include "EspMQTTClient.h"
#define NUMBER_OF_DEVICES 4 //number of led matrix connect in series
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //we do not use this pin just fill to match constructor
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
float intensity = 0;
int multiply = 1;

const char* ssid = "POCOPHONE";
const char* password =  "testspot";

String MATRIX_TEXT = "temp";
int test[]={0};
EspMQTTClient client(
  "http://51.105.190.58/",//SERVER
  8883,// MQTT Broker server ip
  "TestClient"      // Client name that uniquely identify your device
);
//MQTT
void onConnectionEstablished() {

  client.subscribe("mytopic/test", [] (const String & payload)  {
    MATRIX_TEXT = payload;
    Serial.println(payload);
    if(getValue(MATRIX_TEXT, ',', 1)==NULL){
                ledMatrix.setText(MATRIX_TEXT);
    }else{
      for(int i=0;i<10000;i++){
        String temp=getValue(MATRIX_TEXT, ',', i);
        Serial.println(temp);
        if(temp==NULL){
          break;
        }
      }
    }
    

 //   ledMatrix.setText(split);

  });

}

//MATRIX AND WIFI SETUP
void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }


  Serial.println("Connected to the WiFi network");
  ledMatrix.init();
  ledMatrix.setText(MATRIX_TEXT);
}
//SHAPES
void drawHeart(int x, int y) {
  ledMatrix.setPixel(x, y + 1);
  ledMatrix.setPixel(x + 1, y + 1);
  ledMatrix.setPixel(x + 4, y + 1);
  ledMatrix.setPixel(x + 5, y + 1);
  for (int i = 0; i < 8; i++) {
    ledMatrix.setPixel(x - 1 + i, y + 2);

  }
  for (int i = 0; i < 8; i++) {
    ledMatrix.setPixel(x - 1 + i, y + 3);

  }
  for (int i = 1; i < 7; i++) {
    ledMatrix.setPixel(x - 1 + i, y + 4);

  }
  for (int i = 2; i < 6; i++) {
    ledMatrix.setPixel(x - 1 + i, y + 5);

  }
  for (int i = 3; i < 5; i++) {
    ledMatrix.setPixel(x - 1 + i, y + 6);

  }
  intensity += 0.1 * multiply;
  if (intensity >= 6) {
    multiply = -1;
  }
  if (intensity <= 0.1) {
    multiply = 1;
  }
  ledMatrix.setIntensity(intensity);

}


void drawShape(int input[]) {
  for (int i = 0; i < sizeof(input); i += 2) {
    ledMatrix.setPixel(input[i], input[i + 1]);
  }
  
}

void loop() {
  client.loop();
  ledMatrix.clear();

  

  
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
 // drawShape(test);


  //drawHeart(5, 0);
  ledMatrix.commit();
  delay(40);
}

//String splitter
 String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
