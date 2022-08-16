//including the required libraries
#include <ESP8266WiFi.h>
#include <espnow.h>

//Defining the node MCU pins accoeding to the Ultrasonic sensors connections
#define TRIG_PIN 12
#define ECHO_PIN 14

long duration;
int distance;

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x60, 0x01, 0x94, 0x73, 0x77, 0x45};

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

// Structure example to send data
// Must match the receiver structure
typedef struct sensor_data {
    int id;
    int x;
//    int y;
} sensor_data;

// Create a struct_message called test to store variables to be sent
sensor_data myData;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
// Extra code adding----------------------------------------------------------
//constexpr char WIFI_SSID[] = "OnePlus 6";

//int32_t getWiFiChannel(const char *ssid) {
//  if (int32_t n = WiFi.scanNetworks()) {
//      for (uint8_t i=0; i<n; i++) {
//          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
//              return WiFi.channel(i);
//          }
//      }
//  }
//  return 0;
//}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor at baudrate 115200
  Serial.begin(115200);
  // Defining the pin mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
//  WiFi.mode(WIFI_STA);

//  int32_t channel = getWiFiChannel(WIFI_SSID);
//
//  WiFi.printDiag(Serial); // Uncomment to verify channel number before
//  wifi_promiscuous_enable(1);
//  wifi_set_channel(channel);
//  wifi_promiscuous_enable(0);
//  WiFi.printDiag(Serial);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    myData.id = BOARD_ID;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
 // Read the returned wave
    duration = pulseIn(ECHO_PIN, HIGH);
 // Calculate the delay into cm
    distance = duration*0.034/2;
    myData.x = distance;
//    Serial.println("Distance in sender ");
//    Serial.println(distance);
    
    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();
  }
}
