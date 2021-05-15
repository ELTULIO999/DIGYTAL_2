/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
  https://github.com/luisllamasbinaburo/ESP32-Examples
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "julio room";  // Enter your SSID here
const char* password = "Tokyo2233";  //Enter your Password here
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


//************************************************************************************************
// Configuración
//************************************************************************************************
// SETUP PINES DE LED
#define led1 22
#define led2 21
#define led3 19
#define led4 18
#define led5 5
#define led6 17
#define led7 16
// input cables from tiva 
#define input1 36
#define input2 39
#define input3 34
#define input4 35

//var
bool var1,var2,var3,var4;
int num_free_space;



//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  num_free_space=4;
  var1=0;
  var2=0;
  var3=0;
  var4=0;
  
  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  
  pinMode(led1 , OUTPUT);
  pinMode(led2 , OUTPUT);
  pinMode(led3 , OUTPUT);
  pinMode(led4 , OUTPUT);
  pinMode(led5 , OUTPUT);
  pinMode(led6 , OUTPUT);
  pinMode(led7 , OUTPUT);
  
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  
  WiFi.begin(ssid, password); // Connect to your wi-fi modem
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.onNotFound([]() {                  // If the client requests any URI
    if (!HandleFileRead(server.uri()))      // send it if it exists
      handleNotFound();             // otherwise, respond with a 404 (Not Found) error
  });
  //server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
  
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  int val_P1 = digitalRead(input1);
  int val_P2 = digitalRead(input2);
  int val_P3 = digitalRead(input3);
  int val_P4 = digitalRead(input4);
  if(val_P1==1&&var1==0){num_free_space--; var1=1;}
  else if(val_P1==0&&var1==1){num_free_space++;var1=0;}
  
  if(val_P2==1&&var2==0){num_free_space--;var2=1;}
  else if(val_P2==0&&var2==1){num_free_space++;var2=0;}
  
  if(val_P3==1&&var3==0){num_free_space--;var3=1;}
  else if(val_P3==0&&var3==1){num_free_space++;var3=0;}
  
  if(val_P4==1&&var4==0){num_free_space--;var4=1;}
  else if(val_P4==0&&var4==1){num_free_space++;var4=0;}
  
  num_table(num_free_space);
  handle_OnConnect();
}
//************************************************************************************************
// TABLA DE DIOS
//************************************************************************************************
void num_table (uint8_t n){
  switch(n){
    case 0:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, HIGH);//T-T
      digitalWrite(led3, HIGH);//L-T
      digitalWrite(led4, LOW);// M
      digitalWrite(led5, HIGH);//L-B
      digitalWrite(led6, HIGH);//B-B
      digitalWrite(led7, HIGH);//B-R
      break;
    case 1:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, LOW);//T-T
      digitalWrite(led3, LOW);//L-T
      digitalWrite(led4, LOW);// M
      digitalWrite(led5, LOW);//L-B
      digitalWrite(led6, LOW);//B-B
      digitalWrite(led7, HIGH);//B-R
      break;
    case 2:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, HIGH);//T-T
      digitalWrite(led3, LOW);//L-T
      digitalWrite(led4, HIGH);// M
      digitalWrite(led5, HIGH);//L-B
      digitalWrite(led6, HIGH);//B-B
      digitalWrite(led7, LOW);//B-R
      break;
    case 3:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, HIGH);//T-T
      digitalWrite(led3, LOW);//L-T
      digitalWrite(led4, HIGH);// M
      digitalWrite(led5, LOW);//L-B
      digitalWrite(led6, HIGH);//B-B
      digitalWrite(led7, HIGH);//B-R
      break;
    case 4:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, LOW);//T-T
      digitalWrite(led3, HIGH);//L-T
      digitalWrite(led4, HIGH);// M
      digitalWrite(led5, LOW);//L-B
      digitalWrite(led6, LOW);//B-B
      digitalWrite(led7, HIGH);//R-B
      break;
    default:
      digitalWrite(led1, HIGH);//R-T
      digitalWrite(led2, HIGH);//T-T
      digitalWrite(led3, HIGH);//L-T
      digitalWrite(led4, HIGH);// M
      digitalWrite(led5, HIGH);//L-B
      digitalWrite(led6, HIGH);//B-B
      digitalWrite(led7, HIGH);//R-B
    break;

 }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {server.send(200, "text/html",SendHTML(var1,var2,var3,var4));}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
  String SendHTML(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\">\n";
  ptr += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">\n";
  ptr += "<title>Parqueo-matic</title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Parqueo-Matic</h1>\n";
  
  ptr += "<table class=\"table\">\n";
  ptr += "<thead>\n";
  ptr += "<tr>\n";
  ptr += "<th scope='col'># De Parqueo</th>\n";
  ptr += "<th scope='col'>Estado De Parqueo</th>\n";
  ptr += "</tr>\n"; 
  ptr += "</thead>\n";
  
  ptr += "<tbody>\n";
  ptr += "<tr>\n";
  ptr += "<th scope=\"row\">1</th>\n";
  if (p1==0){ptr += "<td class=\"table-success\">LIBRE</td>";}
  else   {ptr += "<td class=\"table-danger\">OCUPADO</td>";}
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<th scope=\"row\">2</th>\n";
  if (p2==0){ptr += "<td class=\"table-success\">LIBRE</td>";}
  else   {ptr += "<td class=\"table-danger\">OCUPADO</td>";}
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<th scope=\"row\">3</th>\n";
  if (p3==0){ptr += "<td class=\"table-success\">LIBRE</td>";}
  else   {ptr += "<td class=\"table-danger\">OCUPADO</td>";}
  ptr += "</tr>\n";

  ptr += "<tr>\n";
  ptr += "<th scope=\"row\">4</th>\n";
  if (p4==0){ptr += "<td class=\"table-success\">LIBRE</td>";}
  else   {ptr += "<td class=\"table-danger\">OCUPADO</td>";}
  ptr += "</tr>\n";
  ptr += "</tbody>\n";
  ptr += "</table>\n";
  ptr += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4\" crossorigin=\"anonymous\"></script>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
/*String SendHTML(uint8_t led1stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server &#128664</h1>\n";
  ptr += "<h3>Ejemplo de Web Server</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }
   

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}*/
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
