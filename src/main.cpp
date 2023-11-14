#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define DEVICE_UID "1X"
// SSID Credentials
#define WIFI_SSID "Bradlee's Iphone 14 pro"
#define WIFI_PASSWORD "z0S3]!s0"

// Firebase Credentials
#define API_KEY "AIzaSyDmY751Jvo14S6WXHz0QCjjUznbtIgZt_8"
#define DATABASE_URL "https://smarthome-1c8f9-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String device_location = "Living Room";
String databasePath = "";
String fuid = "";
unsigned long elapsedMillis = 0;
unsigned long update_interval = 10000;
int count = 0;
bool isAuthenticated = false;

void wifi_Init()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP:");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void firebase_init()
{
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  Serial.println("______________________");
  Serial.println("Sign up new user");

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Success");
    isAuthenticated = true;
    databasePath = "/" + device_location;
    fuid = auth.token.uid.c_str();
  }
  else
  {
    Serial.printf("Failed, %s\n", config.signer.signupError.message.c_str());
    isAuthenticated = false;
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
}

// put function declarations here:
int myFunction(int, int);

void setup()
{
  Serial.begin(115200);
  wifi_Init();
  firebase_init();
}

void database_Test()
{
  if (millis() - elapsedMillis > update_interval && isAuthenticated && Firebase.ready())
  {
    elapsedMillis = millis();
    Serial.println("_________________________________");
    Serial.println("Set int test...");
    String node = path + "/value";

    if (Firebase.set(fbdo, node.c_str(), count++))
    {
      Serial.println("Passed");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE " + fbdo.dataType());
      Serial.println("ETAG: " + fbdo.ETag());
      Serial.print("VALUE: ");
      printResult(fbdo);
      Serial.println('_________________________');
      Serial.println();
    }
    else
    {
      Serial.println("Failed");
      Serial.println("Reason: " + fbdo.errorReason());
      Serial.println("___________________________________");
      Serial.println();
    }
  }
}

void loop() {
  database_Test();
}