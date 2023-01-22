#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

// Define the pin numbers for the buttons and LEDs
#define FIRST_BUTTON 15
#define SECOND_BUTTON 13
#define THIRD_BUTTON 21
#define FOURTH_BUTTON 12
#define GREEN_LED 26
#define RED_LED 27
#define LED_TIMING 1000
#define SS_PIN  5 
#define RST_PIN 4 

// Initialize the MFRC522 RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);

// WiFi credentials
const char* ssid = "CtOS";
const char* password = "876543210";

// Server URL
String serverName = "https://192.168.1.153:443";

// Variables to store the Blockchain ID and RFID card ID
String actualBlockchainID, actualRFID;

// State variable to keep track of the voting process switching between auth and vote
bool state = true;

// Secure WiFi client for the implementation of HTTPS
WiFiClientSecure client;
 
// Root CA Certificate
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"AxGz6Lnx0X5moDFG\n"
"-----END CERTIFICATE-----\n";

// That's the token that the Arduino will use to comunicate with the Flask Web Server
// With this, only the Arduino can communicate and vote
// It's a bad practice to hard code this, but the memory of the Arduino is almost full and JWT for Arduino is discontinued
String secureToken = "R3,u=t?_~LRrPycS";
       
// States of the buttons
int firstLastState = LOW, secondLastState = LOW, thirdLastState = LOW, fourthLastState = LOW;
int firstCurrentState, secondCurrentState, thirdCurrentState, fourthCurrentState;

void setup() {

  // Setting up the Baud rate
  Serial.begin(9600); 

  // Inizializing the WiFi Connection
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

  // Turn the Green Led on success
  digitalWrite(GREEN_LED, HIGH);
  delay(LED_TIMING);
  digitalWrite(GREEN_LED, LOW);

  // Setting the CA Certificate for HTTPS, client.setInsecure() because the cert is self-generated and not really certified
  client.setCACert(root_ca);
  client.setInsecure();

  // Print the local IP
  Serial.println(WiFi.localIP());

  // Set the pins
  pinMode(FIRST_BUTTON, INPUT_PULLUP);
  pinMode(SECOND_BUTTON, INPUT_PULLUP);
  pinMode(THIRD_BUTTON, INPUT_PULLUP);
  pinMode(FOURTH_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Set RFID 
  SPI.begin();
  rfid.PCD_Init();

  // Controls if Ganache is connected with the Flask Webserver
  if(isConnectedGanache()){
    
    // Turns on Green Led on success
    Serial.println("ESP32 connected with Ganache");
    digitalWrite(GREEN_LED, HIGH);
    delay(LED_TIMING);
    digitalWrite(GREEN_LED, LOW);
  } else {

    // Turns on Red Led on failure and stops the program
    Serial.println("Error! Check Ganache connection!");
    digitalWrite(RED_LED, HIGH);
    delay(LED_TIMING);
    digitalWrite(RED_LED, LOW);
    exit(-1);
  }
}

void loop() {

  // Controls if the boolean variable State is True o False, it's implemented like a mutex
  if(state){
    authenticate();
  } else {
    vote(actualBlockchainID);
  } 
}

// This function controls if Ganache is connected to the Flask Web Server by returning a boolean
bool isConnectedGanache(){

  if(WiFi.status()== WL_CONNECTED){

      // There we forge the HTTPS GET
      HTTPClient https;
      String serverPath = serverName + "/isConnectedGanache";
      https.begin(client, serverPath.c_str());

      // We add this token called "Authorization" because in the back-end of the webserver only
      // request with this secret token will be accepted
      https.addHeader("Authorization", secureToken);
      
      // Getting the response code after executing a GET
      int httpsResponseCode = https.GET();
      
      // If OK then read the payload
      if (httpsResponseCode == 200) {
        String payload = https.getString();
        if(payload.equals("True")){
          return true;
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpsResponseCode);
      } 

      // Close HTTPS connection
      https.end();

    }
    else {
      Serial.println("WiFi Disconnected");
    }

    return false;
}

// This function uses the Blockchain ID took after the authentication to vote
void vote(String BlockchainID){

  // Setting the states of Buttons
  firstCurrentState = digitalRead(FIRST_BUTTON);
  secondCurrentState = digitalRead(SECOND_BUTTON);
  thirdCurrentState = digitalRead(THIRD_BUTTON);
  fourthCurrentState = digitalRead(FOURTH_BUTTON);
  
  // Checking if any of them is pressed
  if (firstLastState == HIGH && firstCurrentState == LOW)
    voteForPost(actualBlockchainID, 0);
  else if (secondLastState == HIGH && secondCurrentState == LOW)
    voteForPost(actualBlockchainID, 1);
  else if (thirdLastState == HIGH && thirdCurrentState == LOW)
    voteForPost(actualBlockchainID, 2);
  else if (fourthLastState == HIGH && fourthCurrentState == LOW)
    voteForPost(actualBlockchainID, 3);

  // Keeping the state of Buttons update
  firstLastState = firstCurrentState;
  secondLastState = secondCurrentState;
  thirdLastState = thirdCurrentState;
  fourthLastState = fourthCurrentState;
}

// That's the real Vote function that does an HTTPS Post to the Flask Web Server
void voteForPost(String uid, int candidateID){
  if(WiFi.status()== WL_CONNECTED){

      // There we forge the HTTPS POST
      HTTPClient https;
      String serverPath = serverName + "/vote";
      https.begin(client, serverPath.c_str());
      https.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // We add this token called "Authorization" because in the back-end of the webserver only
      // request with this secret token will be accepted
      https.addHeader("Authorization", secureToken);

      // Assemble the data to be sent
      String httpsRequestData = "uid=" + uid + "&RFID=" + actualRFID + "&candidateID=" + String(candidateID);

      // Get the response code after executing a POST
      int httpsResponseCode = https.POST(httpsRequestData);
      
      // If OK then read the payload
      if (httpsResponseCode == 200) {
        String payload = https.getString();
        Serial.println(payload);

        // Check if this account has already voted        
        if(!payload.equals("Already Voted!")){
          digitalWrite(GREEN_LED, HIGH);
          delay(LED_TIMING);
          digitalWrite(GREEN_LED, LOW);
        } else {
          digitalWrite(RED_LED, HIGH);
          delay(LED_TIMING);
          digitalWrite(RED_LED, LOW);          
        }

        // Updating the state so we switch to authentication
        state = true;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpsResponseCode);
        digitalWrite(RED_LED, HIGH);
        delay(LED_TIMING);
        digitalWrite(RED_LED, LOW);
      }
  
      // Close the connection
      https.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

// This function assigns a Blockchain account given a valid RFID ID 
void authenticate(){
    if (rfid.PICC_IsNewCardPresent()) { // read a new RFID tag
      if (rfid.PICC_ReadCardSerial()) { 
        char RFID[8] = ""; 

        // Transform the RFID tag to a Char array
        takeRFIDtoCharArray(rfid.uid.uidByte, rfid.uid.size, RFID);

        actualRFID = ""; 

        for(int i = 0; i < 8; i++){
            actualRFID += RFID[i];
        }

        if(WiFi.status()== WL_CONNECTED){

          // There we forge the HTTPS POST          
          HTTPClient https;
          String serverPath = serverName + "/authenticate";
          https.begin(client, serverPath.c_str());
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");

          // We add this token called "Authorization" because in the back-end of the webserver only
          // request with this secret token will be accepted
          https.addHeader("Authorization", secureToken);

          // Assemble the data to be sent
          String httpsRequestData = "RFID=" + actualRFID;

          // Get the response code after executing a POST
          int httpsResponseCode = https.POST(httpsRequestData);
      
          // If OK then read the payload
          if (httpsResponseCode == 200) {
            String payload = https.getString();
            Serial.println(payload);

            // Assign the Blockchain ID given by the Flask Web Server
            actualBlockchainID = payload;
            digitalWrite(GREEN_LED, HIGH);
            delay(LED_TIMING);
            digitalWrite(GREEN_LED, LOW);

            // Updating the state so we switch to vote
            state = false;
          } else {
            Serial.print("Error code: ");
            Serial.println(httpsResponseCode);
            digitalWrite(RED_LED, HIGH);
            delay(LED_TIMING);
            digitalWrite(RED_LED, LOW);
            exit(-1);
          }

          // Close the connection
          https.end();
        } else {
          Serial.println("WiFi Disconnected");
        }
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1(); 
      }
  }
}

// This function transforms the RFID identifier to a Char Array
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