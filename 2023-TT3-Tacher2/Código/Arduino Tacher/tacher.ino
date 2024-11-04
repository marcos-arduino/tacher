#include <SoftwareSerial.h>
#include "GM65_scanner.h"
#include <vector>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>



const char* WIFI_SSID = "Tacher";
const char* WIFI_PASSWORD = "planeta123";

unsigned long tiempoInicio; // Declaración de tiempoInicio en el ámbito global
unsigned long duracion = 60000;

SoftwareSerial mySerial(27, 26); // RX, TX
GM65_scanner scanner(&mySerial);

Servo SERVOMOTOR;
const int PINSERVO = 33;

const int SENSOR_MAGNETICO = 23;

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {

  lcd.init();
  lcd.backlight();
  msj_lcd(1);

  SERVOMOTOR.attach(PINSERVO);
  SERVOMOTOR.write(0);

  pinMode(SENSOR_MAGNETICO,INPUT_PULLUP);
  Serial.begin(115200);
  mySerial.begin(9600);
  //trampilla();
  
  
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

void msj_lcd(int numero)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  // * saqué los breaks
  switch (numero)
  {
    case 0:
      lcd.clear();
      break;
    case 1:
      lcd.print("Inicializando");
      lcd.setCursor(0,1);
      lcd.print("el sistema");
      break;
    case 2:
      lcd.print("Ingrese su DNI");
      lcd.setCursor(0,1);
      lcd.print("de forma horizontal");
      break;
    case 3:
      lcd.print("DNI escaneado");
      break;
    case 4:
      lcd.print("Ingrese producto");
      lcd.setCursor(0,1);
      lcd.print("y cierre puerta");
      break;
    case 5:
      lcd.print("Escaneando");
      lcd.setCursor(0,1);
      lcd.print("...");
      break;
    case 6:
      lcd.print("Se escaneó");
      lcd.setCursor(0,1);
      lcd.print("correctamente");
      break;
    case 7:
      lcd.print("DNI ");
      lcd.setCursor(0,1);
      lcd.print("no encontrado");
      break;
    case 8:
      lcd.print("Cierre la puerta");
      break;
    case 9:
      lcd.print("Se quedó"); // encontrado, pero si el producto es reciclable tirelo en un tacho verde!");
      lcd.setCursor(0,1);
      lcd.print("sin tiempo");
      break;
    case 10:
      lcd.print("Cargando...");
      break;
    case 11:
      lcd.print("Producto no"); 
      lcd.setCursor(0,1);
      lcd.print("encontrado");
      break;
    case 12:
      lcd.print("ERROR 500");
      break;      
    default:
      lcd.print("Se acreditaron");
      lcd.setCursor(0,1);
      lcd.print(numero + " puntos");
      break;
  }
}

void trampilla(){
  SERVOMOTOR.write(100);
  delay(1000);
  SERVOMOTOR.write(0);
  delay(1000);
}

String scannerProducto() {
  
  msj_lcd(4);
  delay(1500);
  tiempoInicio = millis();
  String producto = "";

  while (true) {
    unsigned long tiempoActual = millis();
    unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;
    delay(500);

    if (tiempoTranscurrido >= duracion) {
      msj_lcd(9); // esto indica que se quedo sin tiempo
      delay(1000);
      Serial.println("Se quedó sin tiempo!");
      return "-1";  // Se quedó sin tiempo
    }

    if (digitalRead(SENSOR_MAGNETICO) == LOW) {
      msj_lcd(5); // esto indica que esta escaneando
      delay(250);
      scanner.scan_once();
      producto = scanner.get_info();
      delay(1000);
      producto = modificadorCadena(producto);

      if (producto.length() > 12) {
        return producto;
      }
    } 
    else {
      msj_lcd(8); // Esto indica que cierre la puerta
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
      Serial.println("Puntos: "+ cuerpo_respuesta);
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

  msj_lcd(2); // esto indica que se tiene que escanear el dni de forma horizontal
  
  if(digitalRead(SENSOR_MAGNETICO) == HIGH){
  
    msj_lcd(5); // esto indica que se esta escaneando algo
  
    String dni = "";
    delay(1000);
    scanner.scan_once();
    dni = scanner.get_info();
    Serial.println(dni);
    if (dni.indexOf("@") > 0){
      dni = obtenerDNI(dni);
      return dni;
      }
    dni = modificadorCadena(dni);
    Serial.println(dni);
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
  msj_lcd(10); // esto indica qye esta cargando
  delay(1000);
  if (consultaDNI(dni) == "1"){ // Significa que encontro el dni en la base de datos
    Serial.println("Se encontro el dni");
    msj_lcd(3); // esto indica que se leyo el dni correctamente
    delay(1000);

    String producto = scannerProducto();
    Serial.println(producto);

    String valorPuntaje = consultaProducto(dni, producto);

    if (valorPuntaje.toInt() > 0){
      Serial.println("Su puntaje es de :" + valorPuntaje);

      msj_lcd(valorPuntaje.toInt()); // esto indica que se acreditaron los puntos y los muestra por pantalla
      trampilla();
      delay(3000);

    }
    else if (valorPuntaje.toInt() == 0){
      Serial.println("No se encontro el producto en la base de datos");
      msj_lcd(11);
      delay(3000);
    }
    else{
      Serial.println("ERROR");
      msj_lcd(12);
      delay(3000);
    }
  }
  else{
    Serial.println("No encontro el dni");
    msj_lcd(7);
    delay(3000);
  }
}