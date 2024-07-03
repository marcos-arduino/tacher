#include <SoftwareSerial.h>
#include "GM65_scanner.h"
#include <vector>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>


Servo SERVOMOTOR;
const int PINSERVO = 35;
int buttonPin = 25;

const char* WIFI_SSID = "Tacher";
const char* WIFI_PASSWORD = "planeta123";

SoftwareSerial mySerial(27, 26); // RX, TX
GM65_scanner scanner(&mySerial);

unsigned long tiempoInicio; // Declaración de tiempoInicio en el ámbito global
unsigned long duracion = 60000;




void setup() {

  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(115200);
  mySerial.begin(9600);
  SERVOMOTOR.attach(PINSERVO);
  //trampilla();
  
  SERVOMOTOR.write(0);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectandose a WiFi...");
  }
  Serial.println("Conexion exitosa a WiFi ");
  Serial.println(WiFi.localIP());


  Serial.println("initialize the scanner...");
  scanner.init();
  Serial.println("enable the setting QR code");
  scanner.enable_setting_code();
  Serial.println("change to command trigger mode");
  scanner.set_working_mode(1);
  scanner.clear_buffer();
}

String scannerProducto() {
  tiempoInicio = millis();
  String producto = "";

  while (true) {
    unsigned long tiempoActual = millis();
    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
    delay(500);

    if (tiempoTranscurrido >= duracion) {
      Serial.println("Se quedó sin tiempo!");
      return "-1";  // Se quedó sin tiempo
    }

    if (digitalRead(buttonPin) == LOW) {
      delay(250);
      scanner.scan_once();
      producto = scanner.get_info();
      delay(1000);
      producto = modificadorCadena(producto);

      if (producto.length() == 13) {
        return producto;
      }
    } 
    else {
      Serial.println("CIERRE LA PUERTA!");
    }
  }
}

String consultaProducto(String dni, String producto)
{
  HTTPClient http;
  String datosEnviar = "user_dni=" + dni + "&code_bar=" + producto;

  http.begin("https://tacherweb.000webhostapp.com/consulta_scanner.php");      
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 

  int codigo_respuesta = http.POST(datosEnviar);   

  if(codigo_respuesta>0){
    Serial.println("Código HTTP ► " + String(codigo_respuesta));   

    if(codigo_respuesta == 200){
      String cuerpo_respuesta = http.getString();
      Serial.println("El servidor respondió ▼ ");
      Serial.println(cuerpo_respuesta);
      return cuerpo_respuesta;
    }

  }else{

    Serial.print("Error enviando POST, código: ");
    Serial.println(codigo_respuesta);
    return "-1";
  }
  http.end();

}

String obtenerDNI(String str){

  String delimitador = "@";
  int inicio = 0;
  int fin = str.indexOf(delimitador);
  std::vector<String> tokens;

  if (fin < 4)
    return "-1";

  while (fin != -1) {
    String token = str.substring(inicio, fin);
    tokens.push_back(token);

    inicio = fin + delimitador.length();
    fin = str.indexOf(delimitador, inicio);
  }

  String token = str.substring(inicio);// Agrega el último token al vector/lista tokens
  tokens.push_back(token);
  delay(250);
  // Imprime el DNI (quinta posición)
  return tokens[4];

}

String funcionDNI(){
  while(true){
    
  if(digitalRead(buttonPin) == HIGH){
    String dni = "";
    Serial.println(dni);
    delay(1000);
    scanner.scan_once();
    dni = scanner.get_info();
    Serial.println(dni);

    if (dni.indexOf("@") > 0){
      dni = obtenerDNI(dni);
      }

    dni = modificadorCadena(dni);
    if (dni.length() == 8)    
      return dni;
    }
    delay(500);
  }
}  


String modificadorCadena(String str){
  String strNuevo = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    
    // Verificar si el carácter es un dígito
    if (isDigit(c)) {
      strNuevo += c;
    }
  }

  if (strNuevo.startsWith("31")) {
    strNuevo = strNuevo.substring(2); 
  }

  if (strNuevo.endsWith("31")) {
    strNuevo = strNuevo.substring(0, strNuevo.length() - 2); 
  }
  return strNuevo;
}

String consultaDNI(String dni){
  HTTPClient http;
  String datosEnviar = "user_dni=" + dni;

  http.begin("https://tacherweb.000webhostapp.com/consulta_dni.php");      
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 

  int codigo_respuesta = http.POST(datosEnviar);   

  if(codigo_respuesta>0){
    Serial.println("Código HTTP ► " + String(codigo_respuesta));   

    if(codigo_respuesta == 200){
      String cuerpo_respuesta = http.getString();
      Serial.println("El servidor respondió ▼ ");
      Serial.println(cuerpo_respuesta);
      return cuerpo_respuesta;
    }

  }else{

    Serial.print("Error enviando POST, código: ");
    Serial.println(codigo_respuesta);
    return "-1";
  }


  http.end();
}

void loop(){
  String dni = funcionDNI();
  Serial.println(dni);
  delay(1000);
  if (consultaDNI(dni) == "1"){ // Significa que encontro el dni en la base de datos

    Serial.println("Se encontro el dni");
    String producto = scannerProducto();
    Serial.println(producto);
    String valorPuntaje = consultaProducto(dni, producto);
    if (valorPuntaje.toInt() > 0){
      Serial.println("Su puntaje es de :" + valorPuntaje);
    }
    else if (valorPuntaje.toInt() == 0)
      Serial.println("No se encontro el producto en la base de datos");
    else
      Serial.println("ERROR");
  }
  else{
    Serial.println("No encontro el dni");
  }
}




jaja soy giglio
