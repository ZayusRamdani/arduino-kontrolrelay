#include <ESP8266WiFi.h>
//#include <FirebaseESP8266.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "kontrolrelay-d4445-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "E4xNNBVwNrLU4rN0zoDQ5PkI6tzCYMQLEU0GvrwX"
#define WIFI_SSID "TROJAN HORSE"
#define WIFI_PASSWORD "UTY12345"

unsigned long lastflash;
int RPM;
int relay = 2;//D2
int relay2 = 4;//D3
int relay3 = 0;//D4

void ICACHE_RAM_ATTR sens() {
  RPM++;
}
   
void setup() {
  Serial.begin(9600);
  // Menyambungkan ke wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan...");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(50);
  }

  Serial.println();
  Serial.print("Tersambung ke:");
  Serial.print(WiFi.localIP());

  pinMode(D1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D1), sens, RISING);

 pinMode(relay, OUTPUT);
 pinMode(relay2, OUTPUT);
 pinMode(relay3, OUTPUT);

  //ke Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("STATUS_DINAMO",0);
  Firebase.set("STATUS_DINAMO2",0);
  Firebase.set("STATUS_DINAMO3",0);
}
 int n = 0;
 int a = 0;
 int c = 0;

void loop() {

  n = Firebase.getInt("STATUS_DINAMO");
  a = Firebase.getInt("STATUS_DINAMO2");
  c = Firebase.getInt("STATUS_DINAMO3");

  noInterrupts();
  int wings = 2; 
  int RPMnew = RPM / wings;

Serial.println("");
  Serial.print(RPMnew);
  Serial.print(" Rot/sec: ");
  Serial.print((RPMnew * 60));
  Serial.print(" Rot/min.");
  Firebase.setInt("RPM", RPMnew * 60);

  RPM = 0;
  interrupts();

  //Kondisi
  if(n== 1) {
    Serial.print("\n Dinamo Menyala");
    digitalWrite(relay, HIGH);
    delay(100);
  } else{
    Serial.print("\n Dinamo Padam");
    digitalWrite(relay, LOW);
  }

  if(a==1) {
    Serial.print("\n Dinamo2 Menyala");
    digitalWrite(relay2, HIGH);
    delay(200);
  }
  else{
    Serial.print("\n Dinamo2 Padam");
    digitalWrite(relay2, LOW);
  }
  if(c==1) {
    Serial.print("\n Dinamo3 Menyala"); 
    digitalWrite(relay3, HIGH);
    delay(300);
  }
  else{
    Serial.print("\n Dinamo3 Padam");
    digitalWrite(relay3, LOW);
  }  

}
