#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// Incluímos la librería para poder controlar el servo
#include <Servo.h>

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>



/* 1. Define the WiFi credentials */
#define WIFI_SSID "AndroidAPdfb6"
#define WIFI_PASSWORD "kjcn1322"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDneXPOW70Dne0Q9VDoh_tt8ghv9Wz5FFo"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://timbre-inteligente-b20fd-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "juliomeza2510@outlook.com"
#define USER_PASSWORD "julio123"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

//unsigned long count = 0;

int buzzer = D1; //buzzer
Servo servoMotor;

String CLAVE = ""; //contraseña ingresada
String CLAVEANT = "";

int alexaSwitch = 0;
int alexaSwitchLast = 0;

int reconocido = 0;
int reconocidoAnt = 0;

void abrir(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  servoMotor.write(180);
}

void cerrar(){
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  servoMotor.write(0);
}


void setup()
{

  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  servoMotor.attach(D4);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h



#if defined(ESP8266)
  // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;

}

void loop()
{
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Keyboard
    if (Firebase.RTDB.getString(&fbdo, "/sensores/passAttempt")) {
      CLAVE  = fbdo.to<String>();

      if(CLAVE != CLAVEANT && CLAVE.length() == 6){
        CLAVEANT = CLAVE;

        Serial.println("La clave ingresada es: " + CLAVE);

        if(Firebase.RTDB.getString(&fbdo, "/sensores/password")){
          if(CLAVE == String(fbdo.to<String>())){
            Serial.println("La clave es correcta");
            abrir();
            Serial.printf("Set Password attempt... %s\n", Firebase.RTDB.setString(&fbdo, F("/sensores/passAttempt"), " ") ? "ok" : fbdo.errorReason().c_str());
  CLAVEANT = "";
          }
          else{
            Serial.println("La clave es incorrecta");
            Serial.println("Vuelve a introducir la contraseña");
          }
          CLAVE = "";
        }
      }

    }

    //Reconocimiento
    if (Firebase.RTDB.getInt(&fbdo, "/fotos/reconocido")) {
      reconocido  = fbdo.to<int>();

      if(reconocido != reconocidoAnt){
        reconocidoAnt = reconocido;

        if(reconocido == 1){
          Serial.println("Usuario reconocido");
          abrir();
          Serial.printf("Set Fotos reconocido... %s\n", Firebase.RTDB.setInt(&fbdo, F("/fotos/reconocido"), 0) ? "ok" : fbdo.errorReason().c_str());
          reconocidoAnt = 0;

        } else {
          Serial.println("No se reconocio al usuario");
        }

      }

    }

    //Alexa
    if(Firebase.RTDB.getInt(&fbdo, "/alexa/switch")){
      alexaSwitch = fbdo.to<int>();

      if(alexaSwitch != alexaSwitchLast){
        alexaSwitchLast = alexaSwitch;

        if(alexaSwitch == 1){
          abrir();
        } else {
          cerrar();
        }
      }
    }
  
  }
}