#include <SPI.h>
#include "LedMatrix.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "EspMQTTClient.h"
#define NUMBER_OF_DEVICES 4 //number of led matrix connect in series
#define CS_PIN 15
#define CLK_PIN 14
#define MISO_PIN 2 //we do not use this pin just fill to match constructor
#define MOSI_PIN 12
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

HTTPClient http;
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
float intensity = 0;
int multiply = 1;

const char* ssid = "novax";
const char* password =  "nova8800";
bool ShapeON = false;
String MATRIX_TEXT = "void";
int test[1000];
int GLOBAL_COUNTER = 0;
EspMQTTClient client(
  "51.105.190.58",//SERVER
  1883,// MQTT Broker server ip
  "sammy",    // Omit this parameter to disable MQTT authentification
  "pass",
  "TestClient"      // Client name that uniquely identify your
);
//MQTT
void onConnectionEstablished() {

  client.subscribe("mytopic/test", [] (const String & payload)  {
    MATRIX_TEXT = payload;
    Serial.println(payload);
    if (getValue(MATRIX_TEXT, ',', 1) == NULL) {
      ledMatrix.setText(MATRIX_TEXT);
      ShapeON = false;
    } else {
      for (int i = 0; i < 1000; i++) {
        String temp = getValue(MATRIX_TEXT, ',', i);

        if (temp == NULL) {
          ShapeON = true;
          break;
        }
        test[i] = temp.toInt();
        GLOBAL_COUNTER = i;
      }

    }

  });

}

//MATRIX AND WIFI SETUP
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  ledMatrix.init();
  http.begin("http://mqttvalche.westeurope.cloudapp.azure.com/refresh"); //Specify the URL
  int httpCode = http.GET();
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(payload);
        MATRIX_TEXT=payload;
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
  http.end();
  ledMatrix.setText(MATRIX_TEXT);
  client.publish("mytopic/test", "check");
   //Free the resources//Make the request
}

void drawShape(int input[]) {
  for (int i = 0; i < GLOBAL_COUNTER; i += 2) {
    ledMatrix.setPixel(input[i], input[i + 1]);
  }

}

void loop() {
  client.loop();
  ledMatrix.clear();

  if (WiFi.status() != WL_CONNECTED) {
    ledMatrix.setText("Restart me, internet lost");
  }


  if (ShapeON) {
    drawShape(test);
  } else {
    ledMatrix.scrollTextLeft();
    ledMatrix.drawText();
  }

  //


  //drawHeart(5, 0);
  ledMatrix.commit();
  delay(40);
}

//String splitter
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
