//-------------------------------------------------LIBRERÍAS---------------------------------------------------------------------
#include <iostream>
#include <string>
#include <WiFi.h> // Para la conexion wifi
#include <HTTPClient.h> // Para hacer la conexion arduino/pagina Web (php)
#include "GM65_scanner.h" // para el scanner
#include <SoftwareSerial.h>
#include <LiquidCrystal.h> // para la LCD
#include <ESP32Servo.h> // Para incluir la libreria de 'Kevin'

//-----------------------------------------------DEFINICION VARIABLES-------------------------------------------------------------

// Instanciamos nuestro servo
Servo servoMotor;
const int pinServo = 2;

// Ajustes wifi
const char* WIFI_SSID = "Tacher";
const char* WIFI_PASSWORD = "planeta123";

String user = "Tacher";
String pass = "planeta123";

SoftwareSerial mySerial(26, 27); // RX, TX VER BIEN QUE PINES TIENE EL PUERTO SERIE DE LA ESP32

GM65_scanner scanner(& mySerial);

LiquidCrystal lcd_1(23, 22, 19, 18, 5, 17);

const int sensor_magnetico = 15; // Definir bien que PIN va a ser el sensor magnetico

unsigned long tiempoInicio; // Declaración de tiempoInicio en el ámbito global
unsigned long duracion = 60000;

//-----------------------------------------FUNCION LCD---------------------------------------------------------

void mensaje_LCD(int numero)
{
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  // * saqué los breaks
  switch (numero)
  {
    case 0:
      lcd_1.clear();
      break;
    case 1:
      lcd_1.print("Escanee el Dni");
      lcd_1.setCursor(0,1);
      lcd_1.print("horizontalmente");
      break;
    case 2:
      lcd_1.print("No se encontró");
      lcd_1.setCursor(0,1);
      lcd_1.print("un código barras");
      break;
    case 3:
      lcd_1.print("El DNI ");
      lcd_1.setCursor(0,1);
      lcd_1.print("no se encuentra");
      break;
    case 4:
      lcd_1.print("Ingrese producto");
      lcd_1.setCursor(0,1);
      lcd_1.print("y cierre puerta");
      break;
    case 5:
      lcd_1.print("Cierre la puerta");
      break;
    case 6:
      lcd_1.print("No ha ingresado");
      lcd_1.setCursor(0,1);
      lcd_1.print("un producto");
      break;
    case 7:
      lcd_1.print("Abra la puerta");
      lcd_1.setCursor(0,1);
      lcd_1.print("para escanear");
      break;
    case 8:
      lcd_1.print("Colocar producto");
      lcd_1.setCursor(0,1);
      lcd_1.print("debajo del escaner");
      break;
    case 9:
      lcd_1.print("Producto no"); // encontrado, pero si el producto es reciclable tirelo en un tacho verde!");
      lcd_1.setCursor(0,1);
      lcd_1.print("registrado");
      break;
    case 10:
      lcd_1.print("Cargando...");
      break;
    default:
      lcd_1.print("Se acredito:");
      lcd_1.setCursor(0,1);
      lcd_1.print(numero);
      break;
  }
}

/* checked */

//-----------------------------------------ESCANEAR DNI---------------------------------------------------------

String scannerDNI() 
{
  scanner.clear_buffer();
  String cadena = "";
  while (cadena.length() < 10) 
  {
    delay(700);
    scanner.scan_once();
    cadena = scanner.get_info();
    if(cadena.length() > 1)
      Serial.println(cadena);

  }
  lcd_1.clear();
  delay(750);
  cadena = cadena.substring(7);
  String delimitador = "@";
  int inicio = 0;
  int fin = cadena.indexOf(delimitador);
  std::vector<String> tokens;
  delay(250);

  if (fin < 4)
    return "-1"; // *  (Valor escaneado no válido, menos de 5 índices)

  while (fin != -1) 
  {
    String token = cadena.substring(inicio, fin);
    tokens.push_back(token);
    inicio = fin + delimitador.length();
    fin = cadena.indexOf(delimitador, inicio);
  }
  String token = cadena.substring(inicio);  // Agrega el último token al vector/lista tokens
  tokens.push_back(token);
  delay(250);
  return tokens[4];  // Imprime el DNI (quinta posición)
}
  /* checked */

//-----------------------------------------ESCANEAR PRODUCTO---------------------------------------------------------

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

    if (digitalRead(sensor_magnetico) == LOW) {
      delay(250);
      scanner.scan_once();
      producto = scanner.get_info();
      delay(1000);
      Serial.println(producto);

      delay(200);
      producto = producto.substring(7);
      if (producto.length() - 1 == 13) {
        return producto;
      }
    } else {
      Serial.println("CIERRE LA PUERTA!");
    }
  }
}

/* checked */

int consultaScanner(String producto, String dni) // En el caso que numero sea 1, se lee si el dni esta en la db y la var String dni no se usa para la función
{
  HTTPClient http;
  String valor_escaneado = "code_bar=" + producto + "&user_dni=" + dni;
  http.begin("http://192.168.43.161/consulta_scanner.php?"); // Aca va la pagina a la que se conecta
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Se prepara el header (esto no se toca)
  int respuesta_consulta = obtenerRespuestaDeServer(&http, valor_escaneado);
  http.end();
  return respuesta_consulta;
}


int query_barcode(String producto, String dni)
{
  if (producto != "-1")
  {
    int retorno_query_scanner = consultaScanner(producto, dni); // Hace la consulta en el PHP, se le manda un 1 indicando que es para el DNI
    if (retorno_query_scanner == 0)
    {
      Serial.println("No se encontro al producto (lcd 9)");
      mensaje_LCD(9);
      delay(3000);
    }
    else if (retorno_query_scanner > 0) // el valor en puntos tiene que ser mayor a 0, ejemplo 1 punto por una botella
    {
      Serial.println("Se leyo el producto correctamente");
    }
    else
    {
      Serial.println("ERRRRRROR (lcd 2)");
      mensaje_LCD(2);
      delay(3000);
    }
    Serial.println("puntos = "+ retorno_query_scanner);
    return retorno_query_scanner;
  }
  else
  {
    Serial.println("No se escaneó un dni (lcd 2)");
    mensaje_LCD(2);
    delay(3000);
  }
}

//----------------------------------------------------CONSULTA AL PHP---------------------------------------------------------------------------------------------------

typedef enum{
  // * ERROR = 0, (si retorna 0 puede ser que se haya leido bien el dni pero no esté registrado)
  ERROR_SERVER_NO_RESPONDE = -1, // Error enviar POST. RESPUESTA DE LA WEB
  ERROR_SERVER_RESPUESTA_INCORRECTA = -2 // Error server respondio incorrectamente
}codigosDeErrores;


int obtenerRespuestaDeServer(HTTPClient* http, String finDeUrl) {
  int retorno;
  int respuesta = http->POST(finDeUrl);

  if (respuesta > 0)  // Se comprueba si el sv respondio
  {
    if (respuesta == 200)  // Si es igual a 200 significa que encontro
    {
      Serial.println("Conexion Exitosa!");
      String retornoServer = http->getString();
      retorno = retornoServer.toInt();
    } else {
      Serial.print("Error server respondio incorrectamente: ");
      retorno = ERROR_SERVER_RESPUESTA_INCORRECTA;
    }
  } else {
    Serial.print("Error enviar POST. RESPUESTA DE LA WEB:");
    int retorno = ERROR_SERVER_NO_RESPONDE;
  }
  Serial.println(retorno);
  return retorno;
}


//--------------------------------------------------RETORNO DNI------------------------------------------------------------------------------------------------------


int consultaDNI(String dni)  // En el caso que numero sea 1, se lee si el dni esta en la db y la var String dni no se usa para la función
{
  int respuesta_consulta;
  HTTPClient http;
  String valor_escaneado = "user_dni=" + dni;

  http.begin("http://192.168.43.161/consulta_dni.php?");         //  Aca va la pagina a la que se conecta
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //  Se prepara el header (esto no se toca)

  respuesta_consulta = obtenerRespuestaDeServer(&http, valor_escaneado);
  http.end();

  return respuesta_consulta;
}

int query_dni(String dni)
{
  if (dni != "-1")
  {
    int retorno_query_dni = consultaDNI(dni); // Hace la consulta en el PHP, se le manda un 1 indicando que es para el DNI
    if (retorno_query_dni == 0)
    {
      Serial.println("No se encontro dni (lcd 3)");
      mensaje_LCD(3);
      delay(3000);
    }
    else
      if (retorno_query_dni == 1)
      {
        Serial.println("Se leyo el dni correctamente");
      }
    else
    {
      Serial.println("ERRRRRROR (lcd 2)");
      mensaje_LCD(2);
      delay(3000);
    }
    return retorno_query_dni;
  }
  else
  {
    Serial.println("No se escaneó un dni (lcd 2)");
    mensaje_LCD(2);
    delay(3000);
  }
}


// --------------------------------------------------------TRAMPILLA------------------------------------------------------------------

void trampilla(){
  Serial.println("(Se abren las compuertas)");
  SERVOMOTOR.write(78);
  delay(500);
  SERVOMOTOR.write(0);
  delay(2500);
}

// -------------------------------------------------------SETUP------------------------------------------------------------------

void setup() {

  pinMode(sensor_magnetico, INPUT_PULLUP);

  //-------------------------------------SCANNER---------------------------------------------------
  delay(10);
  Serial.begin(115200);  // Se inicializa el puerto serial para la depuracion
  delay(100);

  mySerial.begin(9600);

  scanner.init();
  Serial.println("initialize the scanner...");
  Serial.println("enable the setting QR code");
  scanner.enable_setting_code();

  scanner.set_working_mode(1);

  //-------------------------------------WIFI CONEXION----------------------------------------------
  // Conexion a la red Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectandose a WiFi...");
  }
  Serial.println("Conexion exitosa a WiFi ");
  Serial.println(WiFi.localIP());

  //--------------------------------------SERVO-------------------------------------------------
  SERVOMOTOR.attach(PINSERVO);
  //trampilla(); 
  SERVOMOTOR.write(0);
  delay(3000);
  //---------------------------------------LCD----------------------
  lcd_1.begin(16, 2); 
  lcd_1.clear();
  mensaje_LCD(10);
 }

void loop()
{
  scanner.clear_buffer();
  mensaje_LCD(1);
  String dni = "";
  dni = scannerDNI();
  Serial.println("Esto es el dni: " + dni);
  int retorno_sv = query_dni(dni); // Hace la consulta en el PHP, se le manda un 1 indicando que es para el DNI
  if (retorno_sv == 1)
  // Si la consulta fue exitosa, va a poder ingresar los productos. Esto es el retorno del dni
  {
    mensaje_LCD(7); // si la puerta esta cerrada, espera a que se abra
    if (digitalRead(sensor_magnetico) == HIGH)
    {
      mensaje_LCD(4);
      // una vez que se abre, el usuario deberia ingresar productos (ya deberia estar abierta luego de escanear el dni)
      while (digitalRead(sensor_magnetico) == HIGH)
      // hasta que no la cierre:
      {
      
      // se le indica al usuario que ingrese productos y luego la cierre
      if (digitalRead(sensor_magnetico) == LOW) 
        break; // si al cierra se sale del while para escanear el barcode (linea 285)
      }
    }
    mensaje_LCD(10);
    delay(1000);
    String producto = scannerProducto(); // ESTO NO SE MODIFICA
    Serial.println("Esto es el producto: " + producto);
    int retorno_sv = query_barcode(producto, dni); // Compara el resultado de la consulta para saber si es un producto registrado
    if (retorno_sv > 1)
    {
      trampilla(); // Se abre la trampilla
      Serial.println("Puntos acreditados: " + retorno_sv);
      mensaje_LCD(retorno_sv);
      delay(3000);
    }
    else
    {
      mensaje_LCD(9); // Printear que escaneo un codigo de barras que no se encuentra en la db
      delay(3000);
    }
  }
  else
  {
    mensaje_LCD(3); // printearle por el LCD que no se encontro un dni
    delay(3000);
  }
}