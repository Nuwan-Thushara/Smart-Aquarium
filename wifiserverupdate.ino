#include <WiFi.h>        // Include the Wi-Fi library
#include <WebServer.h>  // standard library
//.............................................................................Configurations
const char* ssid = "Dialog 4G 809";   // The name of the Wi-Fi network  Dialog 4G 809
const char* password = "5F2A73c3" ; // The password of the Wi-Fi network 5F2A73c3
TaskHandle_t Task_A;
TaskHandle_t Task_B;

//The server responds to clients (in this case - web browsers) on port 80, 
//which is a standard port web browsers talk to web servers
WiFiServer server(80);

int temp = 30;
int humi = 10;
int pressure = 20;

//..............................................................................TASK A
void TaskA(void* pvParameters){
  Serial.print("Task A running on CORE_\n");
  Serial.print(xPortGetCoreID());
  WiFi.begin(ssid, password);             // Connect to the network 
  while (WiFi.status() != WL_CONNECTED);// Wait for the Wi-Fi to connect  
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
  xTaskCreatePinnedToCore(TaskB,
                          "Control 2 LEDS",
                          10000,
                          NULL,
                          1,
                          &Task_B,
                          1);
  vTaskDelete(Task_A); //deleting task
}

//TASK B ........................................................................TASK B
void TaskB(void * pvParameters){
  server.begin();  
  while(1) {    
    WiFiClient client = server.available();       
    if (client) {                             
      Serial.println("New Client.");          
      String currentLine = "";            
      while (client.connected()) {         
        if (client.available()) {           
          char c = client.read();             
          Serial.write(c); 
          if (c == '\n') {                    
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              
              client.print("<center>");
              client.print("<marquee><h1>Smart Fish Tank</h1></marquee>");
              client.print("<h2> Group 11 </h2>");
              client.print("<table border = 5 size= 5>");
              client.print("<tr><th><b><h4>Parameter </h4> </b></th>");
              client.print("<th> <b> <h3> Value </h4> </b> </th></tr>");
              client.print("<tr><td> <h4> Temperature (K) </h3> </td><td> <h4>");
              client.print(temp);
              client.print("</h4> </td></tr>");
              client.print("<tr><td> <h4> Humidity (g.kg-1) </h3> </td><td> <h4>");
              client.print(humi);
              client.print("</h4> </td></tr>");
              client.print("<tr><td> <h4> Pressure (Pa) </h3> </td><td> <h4>");
              client.print(pressure);
              client.print("</h4> </td></tr>");
              client.print("</table>");
              client.print("<br><br>");
              
              client.print("Click <button><a href=\"/H\"><h3>  ON  </h3></a></button> to turn the Lights ON.<br>");
              client.print("<br>Click <button><a href=\"/L\"><h3>  OFF  </h3></a></button> to turn the Lights OFF.<br>");           
              client.println();
              
              client.print("<br><br>"); 
              client.print("<ul><br><i>Project by </i><li> Kalpage W. H. C. N. T - EN20469578 </li><li> Madushan K. B. P - EN20408138 </li><li> De Silva A. K. S. N - EN20431280 </li></ul>");
              client.print("</center>");
              client.println();
              break;
            } else {   
              currentLine = "";
            }
          } else if (c != '\r') {  
            currentLine += c;      
          }
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(2, HIGH);                
          }
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(2, LOW);               
          }         
        }
      }      
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}


//..............................................................................SETUP
void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  pinMode(2 , OUTPUT);
  xTaskCreatePinnedToCore(TaskA,//function to be called
                          "Connect to the Network",// name of the task
                          10000,// Stack size (bytes in esp32, words in FreeRTOS)
                          NULL,// parameter to pass to function
                          1,// task priority (0 to configMAX_PRIORITIES=1 (24))
                          &Task_A,// assigned the handle task 1
                          0);  // Run on one core              
}

void loop() {
}
