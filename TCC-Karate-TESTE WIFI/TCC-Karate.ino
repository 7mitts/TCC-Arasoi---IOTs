#include <WiFi.h>

//const definition - Variaveis
const int ledPin = 2;
const char* ssid = "Neymarelindo"; //wifi id definition
const char* password = "Neymar2026"; //wifi password

NetworkServer server(80);


//wifi
void setup() {
   // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200); //serial start (115200) transfer speed
  delay(1000);

  Serial.println("Conectando..."); //conecting text - isso aqui tá conectando no wifi e fazendo verificação dos bgl da rede
  WiFi.begin(ssid, password); //start conection to wifi by ssid and password

  unsigned long startAttemptTime = millis(); //time to error - tempo até dar erro de conexão

  //waiting for conection
  while (WiFi.status() != WL_CONNECTED) { //se der erro é isso aqui q aparece
    if (millis() - startAttemptTime >= 10000) {
      Serial.println("Falha ao conectar");
      digitalWrite(ledPin, LOW);
      return; 
    }  
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado ao Wifi!"); // se funcionar isso aqui aparece
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP()); //ESP32 ID
  digitalWrite(ledPin, HIGH);
}


//web
void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(ledPin, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(ledPin, LOW);  // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
