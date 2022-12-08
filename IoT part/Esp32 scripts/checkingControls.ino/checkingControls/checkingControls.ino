#include <WiFi.h>
#include <HTTPClient.h>
#define FIRST_BUTTON 15
#define SECOND_BUTTON 13
#define THIRD_BUTTON 21
#define FOURTH_BUTTON 12
#define GREEN_LED 26
#define RED_LED 27
#define LED_TIMING 1000

const char* ssid = "CtOS";
const char* password = "876543210";

String serverName = "http://192.168.1.153:222";

int firstLastState = LOW, secondLastState = LOW, thirdLastState = LOW, fourthLastState = LOW;
int firstCurrentState, secondCurrentState, thirdCurrentState, fourthCurrentState;

void setup() {
  Serial.begin(9600); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(FIRST_BUTTON, INPUT_PULLUP);
  pinMode(SECOND_BUTTON, INPUT_PULLUP);
  pinMode(THIRD_BUTTON, INPUT_PULLUP);
  pinMode(FOURTH_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

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
  //authenticate();

  vote();
}

bool isConnectedGanache(){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/isConnected";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
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

void vote(){


  firstCurrentState = digitalRead(FIRST_BUTTON);
  secondCurrentState = digitalRead(SECOND_BUTTON);
  thirdCurrentState = digitalRead(THIRD_BUTTON);
  fourthCurrentState = digitalRead(FOURTH_BUTTON);

  // parte un voto a cazzo all'inzio, da fixare
  
  if (firstLastState == LOW && firstCurrentState == HIGH)
    voteFor(0, 0);
  else if (secondLastState == LOW && secondCurrentState == HIGH)
    voteFor(0, 1);
  else if (thirdLastState == LOW && thirdCurrentState == HIGH)
    voteFor(0, 2);
  else if (fourthLastState == LOW && fourthCurrentState == HIGH)
    voteFor(0, 3);

  firstLastState = firstCurrentState;
  secondLastState = secondCurrentState;
  thirdLastState = thirdCurrentState;
  fourthLastState = fourthCurrentState;
}

void voteFor(int uid, int candidateID){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/vote/" + uid + "/" + candidateID;
    
      http.begin(serverPath.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
        digitalWrite(GREEN_LED, HIGH);
        delay(LED_TIMING);
        digitalWrite(GREEN_LED, LOW);
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

void authenticate(long RFID){
     //da fare quando arriva l'RFID 
}
