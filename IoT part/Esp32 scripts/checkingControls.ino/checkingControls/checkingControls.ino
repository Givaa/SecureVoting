// e la crittografia? basta soltanto questo?

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#define FIRST_BUTTON 15
#define SECOND_BUTTON 13
#define THIRD_BUTTON 21
#define FOURTH_BUTTON 12
#define GREEN_LED 26
#define RED_LED 27
#define LED_TIMING 1000
#define SS_PIN  5 
#define RST_PIN 4 

MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "CtOS";
const char* password = "876543210";

String serverName = "https://192.168.1.155:222";

String actualBlockchainID, actualRFID;

bool state = true;

//WiFiClientSecure client;

/* const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"AxGz6Lnx0X5moDFG\n"
"-----END CERTIFICATE-----\n";*/
       

int firstLastState = LOW, secondLastState = LOW, thirdLastState = LOW, fourthLastState = LOW;
int firstCurrentState, secondCurrentState, thirdCurrentState, fourthCurrentState;

void setup() {
  Serial.begin(9600); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(RED_LED, HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(RED_LED, LOW);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  digitalWrite(GREEN_LED, HIGH);
  delay(LED_TIMING);
  digitalWrite(GREEN_LED, LOW);
  //client.setCACert(root_ca);
  Serial.println(WiFi.localIP());

  pinMode(FIRST_BUTTON, INPUT_PULLUP);
  pinMode(SECOND_BUTTON, INPUT_PULLUP);
  pinMode(THIRD_BUTTON, INPUT_PULLUP);
  pinMode(FOURTH_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();

  //controlla connessione con il webserver Flask e controlla che Flask sia connesso correttamente con Ganache
  if(isConnectedGanache() == true){
    Serial.println("ESP32 connesso con Ganache e pronto a far votare!");
    digitalWrite(GREEN_LED, HIGH);
    delay(LED_TIMING);
    digitalWrite(GREEN_LED, LOW);
  } else {
    Serial.println("Errore! Controllare connessione con Ganache!");
    digitalWrite(RED_LED, HIGH);
    delay(LED_TIMING);
    digitalWrite(RED_LED, LOW);
    exit(-1);
  }
}

void loop() {
  if(state){
    authenticate();
  } else {
    vote(actualBlockchainID);
  } 
}

bool isConnectedGanache(){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/isConnected";
      
      http.begin(serverPath.c_str()/*, root_ca*/);
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode == 200) {
        String payload = http.getString();
        if(payload.equals("True")){
          return true;
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      } 
  
      http.end();

    }
    else {
      Serial.println("WiFi Disconnected");
    }

    return false;
}

void vote(String BlockchainID){

  firstCurrentState = digitalRead(FIRST_BUTTON);
  secondCurrentState = digitalRead(SECOND_BUTTON);
  thirdCurrentState = digitalRead(THIRD_BUTTON);
  fourthCurrentState = digitalRead(FOURTH_BUTTON);
  
  if (firstLastState == HIGH && firstCurrentState == LOW)
    voteForPost(actualBlockchainID, 0);
  else if (secondLastState == HIGH && secondCurrentState == LOW)
    voteForPost(actualBlockchainID, 1);
  else if (thirdLastState == HIGH && thirdCurrentState == LOW)
    voteForPost(actualBlockchainID, 2);
  else if (fourthLastState == HIGH && fourthCurrentState == LOW)
    voteForPost(actualBlockchainID, 3);

  firstLastState = firstCurrentState;
  secondLastState = secondCurrentState;
  thirdLastState = thirdCurrentState;
  fourthLastState = fourthCurrentState;

}

void voteForPost(String uid, int candidateID){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/vote";

      http.begin(serverPath.c_str()/*, root_ca*/);

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String httpRequestData = "uid=" + uid + "&RFID=" + actualRFID + "&candidateID=" + String(candidateID);

      int httpResponseCode = http.POST(httpRequestData);
      
      if (httpResponseCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
        digitalWrite(GREEN_LED, HIGH);
        delay(LED_TIMING);
        digitalWrite(GREEN_LED, LOW);
        state = true;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        digitalWrite(RED_LED, HIGH);
        delay(LED_TIMING);
        digitalWrite(RED_LED, LOW);
      }
  
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

void authenticate(){
    if (rfid.PICC_IsNewCardPresent()) { // leggi un nuovo tag RFID
      if (rfid.PICC_ReadCardSerial()) { 
        char RFID[8] = ""; 

        takeRFIDtoCharArray(rfid.uid.uidByte, rfid.uid.size, RFID);

        actualRFID = ""; 

        for(int i = 0; i < 8; i++){
            actualRFID += RFID[i];
        }

        if(WiFi.status()== WL_CONNECTED){
          HTTPClient http;

          String serverPath = serverName + "/authenticate";

          http.begin(serverPath.c_str()/*, root_ca*/);

          http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          String httpRequestData = "RFID=" + actualRFID;

          int httpResponseCode = http.POST(httpRequestData);
      
          if (httpResponseCode == 200) {
            String payload = http.getString();
            Serial.println(payload);
            actualBlockchainID = payload;
            digitalWrite(GREEN_LED, HIGH);
            delay(LED_TIMING);
            digitalWrite(GREEN_LED, LOW);
            state = false;
            return;
          } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            digitalWrite(RED_LED, HIGH);
            delay(LED_TIMING);
            digitalWrite(RED_LED, LOW);
            exit(-1);
          }
          http.end();
        } else {
        Serial.println("WiFi Disconnected");
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1(); 
      }
  }
}

void takeRFIDtoCharArray(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}
