#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ESP_MICRO.h" // importing our library

const char* ssid = "OnePlus 6";
const char* password = "Tejas1414";

// Structure example to receive data
// Must match the sender structure
typedef struct sensor_data {
    int id;
    int x;
//    int y;
} sensor_data;

// Create a struct_message called myData
sensor_data myData;

// Create a structure to hold the readings from each board
sensor_data board1;
sensor_data board2;
sensor_data board3;

// Create an array with all the structures
sensor_data boardsStruct[3] = {board1, board2, board3};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  int i;
//  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
//  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
//  returnThisInt(myData.x);
//  boardsStruct[myData.id-1].y = myData.y;
  for(i=0; i<3; i++)
  {
    Serial.printf("Board%d: %d ",i, boardsStruct[i].x);
    delay(2000);
   }
//  Serial.printf("Board0: %d Board1: %d Board2: %d", boardsStruct[0].x, boardsStruct[1].x, boardsStruct[2].x); 
//  Serial.printf("x value: %d", boardsStruct[myData.id-1].x);
//  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
  Serial.flush();

}
 
void setup() {
  // Initialize Serial Monitor at baudrate 115200
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.mode(WIFI_STA);
  
  // Set device as a Wi-Fi Station
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Setting as a Wi-Fi Station..");
//  }
//  Serial.print("Station IP Address: ");
//  Serial.println(WiFi.localIP());
//  Serial.print("Wi-Fi Channel: ");
//  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){

  
  // Access the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  */
}
