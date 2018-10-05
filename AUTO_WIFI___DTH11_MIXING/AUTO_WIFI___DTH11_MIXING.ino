#include <stdlib.h>
#include <dht11.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#if defined(ESP8266)
#include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library   
#include <ESP8266mDNS.h>      

#else
#include <WiFi.h>      //ESP32 Core WiFi Library    
#endif


#if defined(ESP8266)

#include <ESP8266WebServer.h>  //Local WebServer used to serve the configuration portal

#else

#include <WebServer.h> //Local DNS Server used for redirecting all requests to the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#endif

#include <DNSServer.h> //Local WebServer used to serve the configuration portal (  https://github.com/zhouhan0126/DNSServer---esp32  )
#include <WiFiManager.h>   // WiFi Configuration Magic (  https://github.com/zhouhan0126/DNSServer---esp32  ) >>  https://github.com/zhouhan0126/DNSServer---esp32  (ORIGINAL)

const int PIN_AP = 2;

dht11 DHT11;

#define PIN 22

#define DHT11PIN 14


Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede</p><p>}</p><p>//callback que indica que salvamos uma nova rede para se conectar (modo estação)
}

void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String header1;
String header2;


// Decode HTTP GET value
String valueString = String(5);
String valueString1 = String(5);
String valueString2 = String(5);
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;


int r = 0;
int g = 0;
int b = 0;

int count = 0;



void setup() {
  Serial.begin(115200);

 #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

   strip.begin();
  strip.show(); // Initialize all pixels to 'off'//colorWipe(strip.Color(255, 0, 0), 50); // Red
  
  pinMode(PIN_AP, INPUT);
  //declaração do objeto wifiManager
  WiFiManager wifiManager;

//utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
//  wifiManager.resetSettings();
 
//callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 

 
//cria uma rede de nome ESP_AP com senha 12345678
  wifiManager.autoConnect("ESP_AP", "12345678"); 
   Serial.println("Connected.");
   server.begin();//big problem + mdns
   
   

     
      // start a server
      //server.begin();//big problem + mdns
      //Serial.println("Server started");
  
}

void loop(){
   WiFiManager wifiManager;
   
  //se o botão foi pressionado

   if ( digitalRead(PIN_AP) == HIGH ) {
      Serial.println("resetar"); //tenta abrir o portal
      //server.stop();
      if(!wifiManager.startConfigPortal("ESP_AP", "12345678") ){
        Serial.println("Falha ao conectar");
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      Serial.println("Conectou ESP_AP!!!");
   }

   
   



 WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    int chk = DHT11.read(DHT11PIN);
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        header1 += c;
        header2 += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
   
             client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");

            
            client.println(".slider { width: 300px; }");
   
            client.println(".slider1 { width: 300px; }");
        
            client.println(".slider2 { width: 300px; }");

            
            client.println("</style>");


                     client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
      
                     
            // Web Page
            client.println("</head><body><h1>식물 관리 서버</h1>");
            client.println("<p>RED : <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            client.println("<p>GREEN: <span id=\"servoPos1\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider1\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value1=\""+valueString+"\"/>");
            client.println("<p>BLUE : <span id=\"servoPos2\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider2\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value2=\""+valueString+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

            client.println("<script>var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo1(pos1) { ");
            client.println("$.get(\"/?value1=\" + pos1 + \"&\"); {Connection: close};}</script>");

            client.println("<script>var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servoP2.innerHTML = slider2.value;");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo2(pos2) { ");
            client.println("$.get(\"/?value2=\" + pos2 + \"&\"); {Connection: close};}</script>");

            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println((float)DHT11.temperature,2);
            client.println(" *C</span></td></tr>");       
            client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
            client.println((float)DHT11.humidity,2);
            client.println(" %</span></td></tr>");
           
            client.println("</body></html>");     
            
            //GET /?value=255& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              r =  valueString.toInt();                 
              //Rotate the servo
              Serial.print(" R = ");
              Serial.println(valueString); 
            }
            if(header.indexOf("GET /?value1=")>=0) {
              pos3 = header.indexOf('=');
              pos4 = header.indexOf('&');
              valueString1 = header.substring(pos3+1, pos4);
              g =  valueString1.toInt();        
              //Rotate the servo
              Serial.print(" G = ");
              Serial.println(valueString1); 
            }
            
            if(header.indexOf("GET /?value2=")>=0) {
              pos5 = header.indexOf('=');
              pos6 = header.indexOf('&');
              valueString2 = header.substring(pos5+1, pos6);
              b = valueString2.toInt();   
              //Rotate the servo
              Serial.print(" B = ");
              Serial.println(valueString2); 
            }

            
       
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.print(" r = ");
    Serial.println(r);
     Serial.print(" g = ");
    Serial.println(g);
     Serial.print(" b = ");
    Serial.println(b);
    colorWipe(strip.Color(g, r, b), 0); // Red
  }
    

   
 }



