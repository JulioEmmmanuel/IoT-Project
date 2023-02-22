#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#include <Keypad.h>

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

unsigned long count = 0;

const int PIRPin = 23;         // sensor de movimiento
const int touchPin = 22;     // interruptor touch

int pirState = 0;  // estado actual de movimiento
int pirLast = 0;  // estado anterior de movimiento

int touchState = 0; // estado actual de touch
int touchLast = 0; // estado anterior de touch

//variables para el keypad
const byte FILAS = 4;
const byte COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[FILAS] = {13, 12, 14, 27};
byte pinesColumnas[COLUMNAS] = {26, 25, 33, 32};

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

char TECLA;
char CLAVE[7];
byte INDICE = 0;

void setup()
{

  Serial.begin(9600);
  pinMode(PIRPin, INPUT);
  pinMode(touchPin, INPUT);
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

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    /*
    sensorPA = analogRead(sensorPin);
    if (sensorPA > sensorValue + 2  || sensorPA < sensorValue - 2 ) {
      sensorValue = sensorPA;
        //Serial.printf("Get bool ref... %s\n", Firebase.RTDB.getBool(&fbdo, F("/test/bool"), &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());
    
        Serial.printf("Set POT... %s\n", Firebase.RTDB.setInt(&fbdo, F("/test/POT"), sensorValue) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, F("/test/POT")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    }
    */


    // Sensor de movimiento
    pirState = digitalRead(PIRPin);
    if (pirState != pirLast ) {

        pirLast = pirState;
        Serial.printf("Set Sensor de movimiento... %s\n", Firebase.RTDB.setInt(&fbdo, F("/sensores/movimiento"), pirState) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get Sensor de movimiento... %s\n", Firebase.RTDB.getInt(&fbdo, F("/sensores/movimiento")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    }

    // Sensor de tacto
    touchState = digitalRead(touchPin);
    if (touchState != touchLast ) {

        touchLast = touchState;
        Serial.printf("Set Interruptor tactil... %s\n", Firebase.RTDB.setInt(&fbdo, F("/sensores/tactil"), touchState) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get Interruptor tactil... %s\n", Firebase.RTDB.getInt(&fbdo, F("/sensores/tactil")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    }

    //logica del teclado
    TECLA = teclado.getKey();
    if (TECLA) {
      CLAVE[INDICE] = TECLA;
      Serial.printf("Set Password attempt... %s\n", Firebase.RTDB.setString(&fbdo, F("/sensores/passAttempt"), CLAVE) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get Password attempt... %s\n", Firebase.RTDB.getString(&fbdo, F("/sensores/passAttempt")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());
      INDICE++;
    }
    if (INDICE == 6) {
      INDICE = 0;
      memset(CLAVE, 0, sizeof(CLAVE));
    }


  }
}