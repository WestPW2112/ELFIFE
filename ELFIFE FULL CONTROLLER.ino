#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "RTClib.h"
#include <Wire.h>          
#include <WiFiClientSecure.h>  

const char* ssid = "Samsul1"; // SSID WIFI
const char* password = "Yoyo2001"; //PASSWORD WIFI
const char* mqtt_server = "broker.hivemq.com";

const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; 
 
String GAS_ID = "AKfycbwPfTeh4gIOjzA7Uijj9d3FItf23HE7iVVL_pPHwLISfxx6eoMbQqQihs_8rsyAfte0"; 

WiFiClient espClient;
PubSubClient klient(espClient);

unsigned long previousMillis = 0;
const long interval = 5000;
 
String msgStr = "";

RTC_DS3231 rtc;

char days[7][12] = {"MINGGU", "SENIN", "SELASA", "RABU", "KAMIS", "JUMAT", "SABTU"};

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;
Servo esc;

int Kanan = 0;
int Oke = 0;
int Kiri = 0;

int pilihbobot, pilihjumlah;
int setwaktu1, setwaktu2, setwaktu3, setwaktu4, setwaktu5, setpakan1, setpakan2, setpakan3, setpakan4, setpakan5;
int sisapakan, nilaisensor;
int serv = 0;

int datajam,datamenit,databerat,datasisa;
String datahari, datajenis;

long durasi;
int jarak;

int sudutservo = 180; // 0-180
int kecesc = 180; // 0-180

#define pbkanan 14
#define pboke 15
#define pbkiri 13


void rumus() {

  int hasilrumus =(pilihbobot*pilihjumlah*3)/5;

  setwaktu1 = 6;
  setwaktu2 = 10;
  setwaktu3 = 14;
  setwaktu4 = 18;
  setwaktu5 = 22;
  setpakan1 = hasilrumus;
  setpakan2 = hasilrumus;
  setpakan3 = hasilrumus;
  setpakan4 = hasilrumus;
  setpakan5 = hasilrumus;

  String dataaksi = "Jadwal_diperbarui";
  String datanull = "-";

  if (!client.connect(host, httpsPort)) { return;}

  String url = "/macros/s/" + GAS_ID + "/exec?datajenis=" + String(dataaksi) + "&databerat=" + String(datanull) + "&datasisa=" + String(datanull);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
}

void setup_wifi() {

  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

    if(WiFi.status() == WL_CONNECTED) {
      digitalWrite(3,HIGH);delay(300);digitalWrite(3,LOW);delay(300);
      digitalWrite(3,HIGH);delay(300);digitalWrite(3,LOW);delay(300);
      digitalWrite(3,HIGH);delay(300);digitalWrite(3,LOW);
    }

  randomSeed(micros());
  client.setInsecure();

  //Serial.println("");
  //Serial.println("WiFi Terhubung");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  String termes = "";
  long NOW = millis();
  long LAST_SENT = 0;


  for (int i = 0; i < length; i++) {
    termes += (char)payload[i];
  }
  if (termes == "EROR"){

  }

  else if (termes == "RST") {
    digitalWrite(1, LOW);                   
    delayMicroseconds(2);
    digitalWrite(1, HIGH);                  
    delayMicroseconds(10);
    digitalWrite(1, LOW);                   

    durasi = pulseIn(16, HIGH);
    jarak = durasi/58;

    sisapakan = map(jarak,0,55,100,0);
    
    unsigned long currentMillis = millis(); 
 
    if (currentMillis - previousMillis >= interval) { 
    previousMillis = currentMillis;

      msgStr = String(sisapakan)+","+String(datahari)+","+String(datajam)+","+String(datamenit)+","+String(datajenis)+","+String(databerat)+","+String(datasisa)+","+String(setwaktu1)+","+String(setpakan1)+","+String(setwaktu2)+","+String(setpakan2)+","+String(setwaktu3)+","+String(setpakan3)+","+String(setwaktu4)+","+String(setpakan4)+","+String(setwaktu5)+","+String(setpakan5);
      byte arrSize = msgStr.length() + 1;
      char msg[arrSize];

      msgStr.toCharArray(msg, arrSize);
      klient.publish("ELFIFE_FEEDER_1_TRANSMITTER_EF1T", msg);
      msgStr = "";
      delay(50);
    }
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);  
  }

  else if (termes == "M50") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);
    serv = 19; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 50; database();} 

  else if (termes == "M100") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);
    serv = 17; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 100; database();} 

  else if (termes == "M150") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);
    serv = 15; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 150; database();} 

  else if (termes == "M200") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW); 
    serv = 13; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 200; database();}

  else if (termes == "M250") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW); 
    serv = 11; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 250; database();}

  else if (termes == "M300") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW); 
    serv = 9; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 300; database();}

  else if (termes == "M400") { 
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);
    serv = 5; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 400; database();}

  else if (termes == "M500") {
    digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW); 
    serv = 1; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 500; database();}

  else if (termes == "TEST") {
    digitalWrite(3,HIGH);delay(300);digitalWrite(3,LOW);delay(300);
    digitalWrite(3,HIGH);delay(300);digitalWrite(3,LOW);
  }

  else {
  digitalWrite(3,HIGH);delay(500);digitalWrite(3,LOW);
  int st0 = termes.indexOf('#');
  int st1 = termes.indexOf('#',st0 + 1);
  int st2 = termes.indexOf('#',st1 + 1);
  int st3 = termes.indexOf('#',st2 + 1);
  int st4 = termes.indexOf('#',st3 + 1);
  int st5 = termes.indexOf('#',st4 + 1);
  int st6 = termes.indexOf('#',st5 + 1);
  int st7 = termes.indexOf('#',st6 + 1);
  int st8 = termes.indexOf('#',st7 + 1);
  int st9 = termes.indexOf('#',st8 + 1);
  int st10 = termes.indexOf('#',st9 + 1);

  String stv1 = termes.substring(st0 + 1, st1);
  String stv2 = termes.substring(st1 + 1, st2);
  String stv3 = termes.substring(st2 + 1, st3);
  String stv4 = termes.substring(st3 + 1, st4);
  String stv5 = termes.substring(st4 + 1, st5);
  String stv6 = termes.substring(st5 + 1, st6);
  String stv7 = termes.substring(st6 + 1, st7);
  String stv8 = termes.substring(st7 + 1, st8);
  String stv9 = termes.substring(st8 + 1, st9);
  String stv10 = termes.substring(st9 + 1, st10);
  
  setwaktu1 = stv1.toInt();
  setpakan1 = stv2.toInt();
  setwaktu2 = stv3.toInt();
  setpakan2 = stv4.toInt();
  setwaktu3 = stv5.toInt();
  setpakan3 = stv6.toInt();
  setwaktu4 = stv7.toInt();
  setpakan4 = stv8.toInt();
  setwaktu5 = stv9.toInt();
  setpakan5 = stv10.toInt();

  String dataaksi = "Jadwal_diperbarui";
  String datanull = "-";

  if (!client.connect(host, httpsPort)) { return;}

  String url = "/macros/s/" + GAS_ID + "/exec?datajenis=" + String(dataaksi) + "&databerat=" + String(datanull) + "&datasisa=" + String(datanull);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  
  }
}

void reconnect() {
  while (!klient.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    String klientId = "ESP8266Client-";
    klientId += String(random(0xffff), HEX);
    if (klient.connect(klientId.c_str())) {
      Serial.println("Terhubung");
      klient.subscribe("ELFIFE_FEEDER_1_RECEIVER_EF1R");
    } 
    else {
      Serial.print("GAGAL !!!, rc =");
      Serial.print(klient.state());
      Serial.println(" Ulangi dalam 5 detik lagi...");
      delay(5000);
    }
  }
}

void database(){

  digitalWrite(1, LOW); delayMicroseconds(2); digitalWrite(1, HIGH); delayMicroseconds(10); digitalWrite(1, LOW); durasi = pulseIn(16, HIGH); jarak = durasi/58; sisapakan = map(jarak,0,55,100,0); 
  DateTime now = rtc.now();
  datahari = days[now.dayOfTheWeek()]; datajam = now.hour(); datamenit = now.minute(); datasisa = sisapakan;

  if (!client.connect(host, httpsPort)) { return;}

  String url = "/macros/s/" + GAS_ID + "/exec?datajenis=" + String(datajenis) + "&databerat=" + String(databerat) + "&datasisa=" + String(datasisa);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  
}

void beripakan(){

  DateTime now = rtc.now();
//range 0-75gr ....50gr/2x 119/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 <= 75 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 <= 75 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 <= 75 || setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 <= 75 || setwaktu5 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan5 <= 75){
    serv = 119; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 50; datajenis = "Rutin"; database();}
//range 75-125gr ....100gr/4x 117/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 75 && setpakan1 <= 125 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 75 && setpakan2 <= 125 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 75 && setpakan3 <= 125 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 75 && setpakan4 <= 125|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 75 && setpakan5 <= 125){
    serv = 117; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 100; datajenis = "Rutin"; database();}
//range 125-175gr ....150gr/6x 115/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 125 && setpakan1 <= 175 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 125 && setpakan2 <= 175 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 125 && setpakan3 <= 175 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 125 && setpakan4 <= 175 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 125 && setpakan5 <= 175){
    serv = 115; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 150; datajenis = "Rutin"; database();}
//range 175-225gr ....200gr/8x 113/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 175 && setpakan1 <= 225 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 175 && setpakan2 <= 225 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 175 && setpakan3 <= 225 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 175 && setpakan4 <= 225 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 175 && setpakan5 <= 225){
    serv = 113; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 200; datajenis = "Rutin"; database();}
//range 225-275gr ....250gr/10x 111/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 225 && setpakan1 <= 275 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 235 && setpakan2 <= 275 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 225 && setpakan3 <= 275 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 225 && setpakan4 <= 275|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 225 && setpakan5 <= 275){
    serv = 111; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 250; datajenis = "Rutin"; database();}
//range 275-325gr ....300gr/12x 109/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 275 && setpakan1 <= 325 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 275 && setpakan2 <= 325 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 275 && setpakan3 <= 325 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 275 && setpakan4 <= 325|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 275 && setpakan5 <= 325){
    serv = 109; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 300; datajenis = "Rutin"; database();}  
//range 325-375gr ....350gr/14x 107/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 325 && setpakan1 <= 375 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 325 && setpakan2 <= 375 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 325 && setpakan3 <= 375 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 325 && setpakan4 <= 375|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 325 && setpakan5 <= 375){
    serv = 107; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 350; datajenis = "Rutin"; database();}  
//range 375-425gr ....400gr/16x 105/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 375 && setpakan1 <= 425 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 375 && setpakan2 <= 425 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 375 && setpakan3 <= 425 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 375 && setpakan4 <= 425|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 375 && setpakan5 <= 425){
    serv = 105; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 400; datajenis = "Rutin"; database();}  
//range 425-475gr ....450gr/18x 103/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 425 && setpakan1 <= 475 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 425 && setpakan2 <= 475 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 425 && setpakan3 <= 475 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 425 && setpakan4 <= 475|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 425 && setpakan5 <= 475){
    serv = 103; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 450; datajenis = "Rutin"; database();}  
//range 475-550gr ....500gr/20x 101/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 475 && setpakan1 <= 550 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 475 && setpakan2 <= 550 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 475 && setpakan3 <= 550 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 475 && setpakan4 <= 550|| setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 475 && setpakan5 <= 550){
    serv = 101; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 500; datajenis = "Rutin"; database();}  
//range 550-650gr ....600gr/24x 97/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 550 && setpakan1 <= 650 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 550 && setpakan2 <= 650 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 550 && setpakan3 <= 650 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 550 && setpakan4 <= 650 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 550 && setpakan5 <= 650){
    serv = 97; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 600; datajenis = "Rutin"; database();}  
//range 650-750gr ....700gr/28x 93/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 650 && setpakan1 <= 750 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 650 && setpakan2 <= 750 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 650 && setpakan3 <= 750 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 650 && setpakan4 <= 750 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 650 && setpakan5 <= 750){
    serv = 93; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 700; datajenis = "Rutin"; database();}  
//range 750-850gr ....800gr/32x 89/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 750 && setpakan1 <= 850 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 750 && setpakan2 <= 850 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 750 && setpakan3 <= 850 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 750 && setpakan4 <= 850 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 750 && setpakan5 <= 850){
    serv = 89; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 800; datajenis = "Rutin"; database();}  
//range 850-950gr ....900gr/36x 85/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 850 && setpakan1 <= 950 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 850 && setpakan2 <= 950 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 850 && setpakan3 <= 950 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 850 && setpakan4 <= 950 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 850 && setpakan5 <= 950){
    serv = 85; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 900; datajenis = "Rutin"; database();}  
//range 950-1050gr ....1000gr/40x 81/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 950 && setpakan1 <= 1050 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 950 && setpakan2 <= 1050 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 950 && setpakan3 <= 1050 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 950 && setpakan4 <= 1050 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 950 && setpakan5 <= 1050){
    serv = 81; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1000; datajenis = "Rutin"; database();}  
//range 1050-1150gr ....1100gr/44x 77/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1050 && setpakan1 <= 1150 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1050 && setpakan2 <= 1150 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1050 && setpakan3 <= 1150 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1050 && setpakan4 <= 1150 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1050 && setpakan5 <= 1150){
    serv = 77; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1100; datajenis = "Rutin"; database();}  
//range 1150-1250gr ....1200gr/48x 73/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1150 && setpakan1 <= 1250 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1150 && setpakan2 <= 1250 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1250 && setpakan3 <= 1250 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1150 && setpakan4 <= 1250 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1150 && setpakan5 <= 1250){
    serv = 73; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1200; datajenis = "Rutin"; database();}  
//range 1250-1350gr ....1300gr/52x 69/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1250 && setpakan1 <= 1350 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1250 && setpakan2 <= 1350 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1250 && setpakan3 <= 1350 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1250 && setpakan4 <= 1350 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1250 && setpakan5 <= 1350){
    serv = 69; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1300; datajenis = "Rutin"; database();}  
//range 1350-1450gr ....1400gr/56x 65/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1350 && setpakan1 <= 1450 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1350 && setpakan2 <= 1450 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1350 && setpakan3 <= 1450 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1350 && setpakan4 <= 1450 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1350 && setpakan5 <= 1450){
    serv = 65; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1400; datajenis = "Rutin"; database();}  
//range 1450-1550gr ....1500gr/60x 61/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1450 && setpakan1 <= 1550 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1450 && setpakan2 <= 1550 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1450 && setpakan3 <= 1550 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1450 && setpakan4 <= 1550 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1450 && setpakan5 <= 1550){
    serv = 61; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1500; datajenis = "Rutin"; database();}  
//range 1550-1650gr ....1600gr/64x 57/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1550 && setpakan1 <= 1650 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1550 && setpakan2 <= 1650 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1550 && setpakan3 <= 1650 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1550 && setpakan4 <= 1650 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1550 && setpakan5 <= 1650){
    serv = 57; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1600; datajenis = "Rutin"; database();}  
//range 1650-1750gr ....1700gr/68x 53/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1650 && setpakan1 <= 1750 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1650 && setpakan2 <= 1750 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1650 && setpakan3 <= 1750 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1650 && setpakan4 <= 1750 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1650 && setpakan5 <= 1750){
    serv = 53; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1700; datajenis = "Rutin"; database();}  
//range 1750-1850gr ....1800gr/72x 49/120
    if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1750 && setpakan1 <= 1850 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1750 && setpakan2 <= 1850 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1750 && setpakan3 <= 1850 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1750 && setpakan4 <= 1850 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1750 && setpakan5 <= 1850){
    serv = 49; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1800; datajenis = "Rutin"; database();}  
//range 1850-1950gr ....1900gr/76x 45/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1850 && setpakan1 <= 1950 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1850 && setpakan2 <= 1950 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1850 && setpakan3 <= 1950 || 
  setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1850 && setpakan4 <= 1950 || setpakan5 == now.hour() && now.minute() == 1 && now.second() == 1 && setwaktu5 > 1850 && setpakan5 <= 1950){
    serv = 45; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 1900; datajenis = "Rutin"; database();}  
//diatas 1950 ....2000gr/80x 41/120
  if(setwaktu1 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan1 > 1950 || setwaktu2 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan2 > 1950 || setwaktu3 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan3 > 1950 || setwaktu4 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan4 > 1950|| setwaktu5 == now.hour() && now.minute() == 1 && now.second() == 1 && setpakan5 > 1950){
    serv = 40; while (serv <=  120) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); databerat = 2000; datajenis = "Rutin"; database();}     

//range 2050-2150gr ....2100gr/84x 37/120
//range 2150-2250gr ....2200gr/88x 33/120
//range 2250-2350gr ....2300gr/92x 29/120
//range 2350-2450gr ....2400gr/96x 25/120
//range 2450-2550gr ....2500gr/100x 21/120
//range 2550-2650gr ....2600gr/104x 17/120
//range 2650-2750gr ....2700gr/108x 13/120
//range 2750-2850gr ....2800gr/112x 9/120
//range 2850-2950gr ....2900gr/116x 5/120
//range 2950-3050gr ....3000gr/120x 1/120
                  
}

void setup() {

  Serial.begin (115200);
  lcd.begin();
  rtc.begin();

  pinMode(pbkanan,INPUT);//PB1
  pinMode(pboke,INPUT);//PB2
  pinMode(pbkiri,INPUT);//PB3
  pinMode(16, INPUT);//ECHO
  pinMode(1, OUTPUT);//TRIG
  pinMode(3,OUTPUT);//LED INDIKATOR

  myservo.attach(2, 500, 2400);//Servo
  esc.attach(0);//Brushless

  myservo.write(0);
  esc.write(0);

  setup_wifi();
  klient.setServer(mqtt_server, 1883);
  klient.setCallback(callback);

  String dataaksi = "Reset";
  String datanull = "-";

  if (!client.connect(host, httpsPort)) { return;}

  String url = "/macros/s/" + GAS_ID + "/exec?datajenis=" + String(dataaksi) + "&databerat=" + String(datanull) + "&datasisa=" + String(datanull);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  //if(rtc.lostPower()){rtc.adjust(DateTime(2023,2,10,19,46,0));}
}

void loop() {

  menu();

}

void menu(){

  Home:
  while(1){

    lcd.setCursor(0,0); lcd.print("       ELFIFE       ");
    lcd.setCursor(0,1); lcd.print("Electric Fish Feeder");
    lcd.setCursor(0,2); lcd.print("--------------------");
    lcd.setCursor(0,3); lcd.print("Cek   CekWaktu   Set");

    if (!klient.connected()) { reconnect(); } klient.loop();
    beripakan();

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear();goto Pakanlangsung;}
      if (Oke == HIGH) { delay(100); lcd.clear();goto Cekwaktu;}
      if (Kiri == HIGH) { delay(100); lcd.clear();goto Cekjadwal1;}
  }
  
  Cekwaktu:
  while(1){

    DateTime now = rtc.now();
    
    lcd.clear();
    lcd.setCursor(1,0); lcd.print(days[now.dayOfTheWeek()]);
    lcd.setCursor(7,0); lcd.print(",");
    lcd.setCursor(9,0); lcd.print(now.day(), DEC);
    lcd.setCursor(12,0); lcd.print(now.month(), DEC);
    lcd.setCursor(15,0); lcd.print(now.year(), DEC);
    lcd.setCursor(6,1); lcd.print(now.hour(), DEC);
    lcd.setCursor(8,1); lcd.print(":");
    lcd.setCursor(9,1); lcd.print(now.minute(), DEC);
    lcd.setCursor(11,1); lcd.print(":");
    lcd.setCursor(12,1); lcd.print(now.second(), DEC);
    lcd.setCursor(0,3);
    lcd.print("Wifi    Back    Sisa");
    delay(1000);

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Ceksisapakan;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihwifi;}
  }
  
  Ceksisapakan:
  while(1){

    digitalWrite(1, LOW);                   
    delayMicroseconds(2);
    digitalWrite(1, HIGH);                  
    delayMicroseconds(10);
    digitalWrite(1, LOW);                   

    durasi = pulseIn(16, HIGH);
    jarak = durasi/58;

    sisapakan = map(jarak,0,55,100,0);

    lcd.clear();
    lcd.setCursor(0,0); lcd.print("     SISA PAKAN     ");
    lcd.setCursor(9,1); lcd.print(sisapakan);
    lcd.setCursor(0,2); lcd.print("       Persen       ");
    lcd.setCursor(0,3); lcd.print("        Back        ");
    delay(500);

    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    //Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto Cekjadwal1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Home;}
      //if (Kiri == HIGH) { delay(100); lcd.clear(); goto Home;}
  }

  Cekjadwal1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    JADWAL AKTIF    ");
    lcd.setCursor(0,1); lcd.print("JDWL 1:");
    lcd.setCursor(8,1); lcd.print(setwaktu1);
    lcd.setCursor(10,1); lcd.print(":00/");
    lcd.setCursor(14,1); lcd.print(setpakan1);
    lcd.setCursor(18,1); lcd.print("gr");
    lcd.setCursor(0,2); lcd.print("JDWL 2:");
    lcd.setCursor(8,2); lcd.print(setwaktu2);
    lcd.setCursor(10,2); lcd.print(":00/");
    lcd.setCursor(14,2); lcd.print(setpakan2);
    lcd.setCursor(18,2); lcd.print("gr");
    lcd.setCursor(0,3); lcd.print("Back            Next");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);
    
      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Cekjadwal2;}
      //if (Oke == LOW) { delay(100); lcd.clear(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Home;}
  }

  Cekjadwal2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("JDWL 3:");
    lcd.setCursor(8,0); lcd.print(setwaktu3);
    lcd.setCursor(10,0); lcd.print(":00/");
    lcd.setCursor(14,0); lcd.print(setpakan3);
    lcd.setCursor(18,0); lcd.print("gr");
    lcd.setCursor(0,1); lcd.print("JDWL 4:");
    lcd.setCursor(8,1); lcd.print(setwaktu4);
    lcd.setCursor(10,1); lcd.print(":00/");
    lcd.setCursor(14,1); lcd.print(setpakan4);
    lcd.setCursor(18,1); lcd.print("gr");
    lcd.setCursor(0,2); lcd.print("JDWL 5:");
    lcd.setCursor(8,2); lcd.print(setwaktu5);
    lcd.setCursor(10,2); lcd.print(":00/");
    lcd.setCursor(14,2); lcd.print(setpakan5);
    lcd.setCursor(18,2); lcd.print("gr");
    lcd.setCursor(0,3); lcd.print("Back                ");

    //Kanan = digitalRead(pbkanan);
    //Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);
    
      //if (Kanan == HIGH) { delay(100); lcd.clear(); goto Ceksisapakan;}
      //if (Oke == LOW) { delay(100); lcd.clear(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Cekjadwal1;}
  }

//---------------------------------SET LANGSUNG-------------------------------------------

  Pakanlangsung:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("   PAKAN LANGSUNG   ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Setotomatis;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung50;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Home;}
  }

  Pakanlangsung50:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("  Berapa  Banyak ?  ");
    lcd.setCursor(0,1); lcd.print("     50  Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung100;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 19; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 50; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung;}
  }

  Pakanlangsung100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("  Berapa  Banyak ?  ");
    lcd.setCursor(0,1); lcd.print("     100 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 17; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 100; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung50;}
  }

  Pakanlangsung200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("  Berapa Banyak ?   ");
    lcd.setCursor(0,1); lcd.print("     200 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 13; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 200; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung100;}
  }

  Pakanlangsung300:
  while(1){

    lcd.setCursor(0,0); lcd.print("  Berapa Banyak ?   ");
    lcd.setCursor(0,1); lcd.print("     300 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 9; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 300; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung200;}
  }

  Pakanlangsung400:
  while(1){

    lcd.setCursor(0,0); lcd.print("  Berapa Banyak ?   ");
    lcd.setCursor(0,1); lcd.print("     400 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 5; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 400; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung300;}
  }

  Pakanlangsung500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("  Berapa Banyak ?   ");
    lcd.setCursor(0,1); lcd.print("     500 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto Pakanlangsung;}
      if (Oke == HIGH) { delay(100); lcd.clear(); lcd.setCursor(0,1); lcd.print("  Dalam Proses....  "); serv = 1; while (serv <=  20) { esc.write(kecesc); myservo.write(sudutservo); delay(3000); myservo.write(0); delay(3000); serv++;} esc.write(0); datajenis = "Langsung"; databerat = 500; database(); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung400;}
  }

//---------------------------------SET OTOMATIS-------------------------------------------

  Setotomatis:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("    SET OTOMATIS    ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");    
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == HIGH) { delay(100); lcd.clear(); goto Setmanual;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Pilihberat50;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pakanlangsung;}
  }

  Pilihberat50:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Bobot Rata-Rata Ikan");
    lcd.setCursor(0,1); lcd.print("      50 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihberat100;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihbobot = 5; goto Pilihjumlah100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Setotomatis;}
  }

  Pilihberat100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Bobot Rata-Rata Ikan");
    lcd.setCursor(0,1); lcd.print("     100 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihberat150;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihbobot = 10; goto Pilihjumlah100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihberat50;}
  }

  Pilihberat150:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Bobot Rata-Rata Ikan");
    lcd.setCursor(0,1); lcd.print("     150 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihberat200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihbobot = 15; goto Pilihjumlah100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihberat50;}
  }

  Pilihberat200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Bobot Rata-Rata Ikan");
    lcd.setCursor(0,1); lcd.print("     200 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihberat250;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihbobot = 20; goto Pilihjumlah100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihberat150;}
  }

  Pilihberat250:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Bobot Rata-Rata Ikan");
    lcd.setCursor(0,1); lcd.print("     250 Gram       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");    
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto Setotomatis;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihbobot = 25; goto Pilihjumlah100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihberat200;}
  }

  Pilihjumlah100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Jumlah Ikan Di Kolam");
    lcd.setCursor(0,1); lcd.print("     100 Ekor       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");    
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihjumlah = 10; rumus(); lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihberat50;}
  }

  Pilihjumlah200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Jumlah Ikan Di Kolam");
    lcd.setCursor(0,1); lcd.print("     200 Ekor       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");    
    
    Kanan = digitalRead(0);
    Oke = digitalRead(2);
    Kiri = digitalRead(14);

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);
  }

  Pilihjumlah300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Jumlah Ikan Di Kolam");
    lcd.setCursor(0,1); lcd.print("     300 Ekor       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");   
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihjumlah = 30; rumus(); lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah200;}
  }

  Pilihjumlah400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Jumlah Ikan Di Kolam");
    lcd.setCursor(0,1); lcd.print("     400 Ekor       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
     
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihjumlah = 40; rumus(); lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah300;}
  }

  Pilihjumlah500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("Jumlah Ikan Di Kolam");
    lcd.setCursor(0,1); lcd.print("     500 Ekor       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto Pilihberat50;}
      if (Oke == HIGH) { delay(100); lcd.clear(); pilihjumlah = 50; rumus(); lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjumlah400;}
  }

//---------------------------------SET MANUAL-------------------------------------------

  Setmanual:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("     SET MANUAL     ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK         ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihwifi;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Setotomatis;}
  }

//____________________________JADWAL PERTAMA_________________________________

  Pilihjadwal1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    PILIH JADWAL    ");
    lcd.setCursor(0,1); lcd.print("      PERTAMA       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA0;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Setmanual;}
  }

  PilihwaktuA0:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  Kosongkan Jadwal  ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK    Tidak");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 0; setpakan1 = 0; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal1;}
  }

  PilihwaktuA1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     01.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");   
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 1; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA0;}
  }

  PilihwaktuA2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     02.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 2; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA1;}
  }

  PilihwaktuA3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     03.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 3; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA2;}
  }

  PilihwaktuA4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     04.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 4; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA3;}
  }

  PilihwaktuA5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     05.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA6;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 5; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA4;}
  }

  PilihwaktuA6:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     06.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA7;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 6; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA5;}
  }

  PilihwaktuA7:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     07.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA8;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 7; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA6;}
  }

  PilihwaktuA8:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     08.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA9;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 8; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA7;}
  }

  PilihwaktuA9:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     09.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA10;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 9; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA8;}
  }

  PilihwaktuA10:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     10.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA11;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 10; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA9;}
  }

  PilihwaktuA11:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     11.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 11; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA10;}
  }

  PilihwaktuA12:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     12.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA13;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 12; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA11;}
  }

  PilihwaktuA13:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     13.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA14;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 13; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA12;}
  }

  PilihwaktuA14:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     14.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA15;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 14; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA13;}
  }

  PilihwaktuA15:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     15.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA16;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 15; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA14;}
  }

  PilihwaktuA16:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     16.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA17;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 16; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA15;}
  }

  PilihwaktuA17:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     17.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA18;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 17; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA16;}
  }

  PilihwaktuA18:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     18.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA19;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 18; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA17;}
  }

  PilihwaktuA19:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     19.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA20;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 19; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA18;}
  }

  PilihwaktuA20:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     20.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA21;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 20; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA19;}
  }

  PilihwaktuA21:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     21.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA22;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 21; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA20;}
  }

  PilihwaktuA22:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     22.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA23;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 22; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA21;}
  }

  PilihwaktuA23:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     23.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA24;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 23; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA22;}
  }

  PilihwaktuA24:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     24.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu1 = 0; goto PilihbobotA100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuA23;}
  }

  PilihbobotA100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      100 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotA200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan1 = 100; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal1;}
  }

  PilihbobotA200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      200 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotA300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan1 = 200; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotA100;}
  }

  PilihbobotA300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      300 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotA400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan1 = 300; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotA200;}
  }

  PilihbobotA400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      400 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotA500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan1 = 400; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotA300;}
  }

  PilihbobotA500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      500 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihbobotA400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan1 = 500; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal1;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotA400;}
  }

//____________________________JADWAL KEDUA_________________________________

  Pilihjadwal2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    PILIH JADWAL    ");
    lcd.setCursor(0,1); lcd.print("       KEDUA        ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB0;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal1;}
  }

  PilihwaktuB0:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  Kosongkan Jadwal  ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK    Tidak");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 0; setpakan2 = 0; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal2;}
  }

  PilihwaktuB1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     01.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 1; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB0;}
  }

  PilihwaktuB2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     02.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 2; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB1;}
  }

  PilihwaktuB3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     03.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 3; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB2;}
  }

  PilihwaktuB4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     04.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 4; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB3;}
  }

  PilihwaktuB5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     05.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB6;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 5; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB4;}
  }

  PilihwaktuB6:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     06.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB7;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 6; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB5;}
  }

  PilihwaktuB7:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     07.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB8;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 7; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB6;}
  }

  PilihwaktuB8:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     08.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB9;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 8; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB7;}
  }

  PilihwaktuB9:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     09.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB10;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 9; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB8;}
  }

  PilihwaktuB10:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     10.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB11;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 10; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB9;}
  }

  PilihwaktuB11:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     11.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 11; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB10;}
  }

  PilihwaktuB12:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     12.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB13;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 12; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB11;}
  }

  PilihwaktuB13:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     13.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB14;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 13; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB12;}
  }

  PilihwaktuB14:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     14.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB15;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 14; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB13;}
  }

  PilihwaktuB15:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     15.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB16;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 15; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB14;}
  }

  PilihwaktuB16:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     16.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB17;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 16; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB15;}
  }

  PilihwaktuB17:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     17.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB18;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 17; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB16;}
  }

  PilihwaktuB18:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     18.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB19;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 18; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB17;}
  }

  PilihwaktuB19:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     19.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB20;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 19; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB18;}
  }

  PilihwaktuB20:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     20.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB21;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 20; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB19;}
  }

  PilihwaktuB21:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     21.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB22;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 21; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB20;}
  }

  PilihwaktuB22:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     22.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB23;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 22; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB21;}
  }

  PilihwaktuB23:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     23.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB24;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 23; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB22;}
  }

  PilihwaktuB24:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     24.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu2 = 0; goto PilihbobotB100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuB23;}
  }

  PilihbobotB100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      100 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotB200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan2 = 100; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal2;}
  }

  PilihbobotB200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      200 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotB300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan2 = 200; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotB100;}
  }

  PilihbobotB300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      300 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotB400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan2 = 300; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotB200;}
  }

  PilihbobotB400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      400 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);
    
      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotB500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan2 = 400; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotB300;}
  }

  PilihbobotB500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      500 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihbobotA400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan2 = 500; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal2;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotB400;}
  }

//____________________________JADWAL KETIGA_________________________________

  Pilihjadwal3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    PILIH JADWAL    ");
    lcd.setCursor(0,1); lcd.print("       KETIGA        ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC0;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal2;}
  }

  PilihwaktuC0:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  Kosongkan Jadwal  ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK    Tidak");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 0; setpakan3 = 0; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal3;}
  }

  PilihwaktuC1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     01.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 1; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC0;}
  }

  PilihwaktuC2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     02.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 2; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC1;}
  }

  PilihwaktuC3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     03.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 3; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC2;}
  }

  PilihwaktuC4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     04.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 4; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC3;}
  }

  PilihwaktuC5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     05.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC6;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 5; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC4;}
  }

  PilihwaktuC6:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     06.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC7;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 6; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC5;}
  }

  PilihwaktuC7:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     07.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC8;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 7; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC6;}
  }

  PilihwaktuC8:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     08.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC9;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 8; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC7;}
  }

  PilihwaktuC9:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     09.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC10;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 9; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC8;}
  }

  PilihwaktuC10:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     10.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC11;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 10; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC9;}
  }

  PilihwaktuC11:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     11.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 11; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC10;}
  }

  PilihwaktuC12:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     12.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC13;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 12; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC11;}
  }

  PilihwaktuC13:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     13.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC14;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 13; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC12;}
  }

  PilihwaktuC14:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     14.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC15;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 14; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC13;}
  }

  PilihwaktuC15:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     15.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC16;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 15; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC14;}
  }

  PilihwaktuC16:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     16.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC17;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 16; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC15;}
  }

  PilihwaktuC17:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     17.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC18;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 17; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC16;}
  }

  PilihwaktuC18:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     18.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC19;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 18; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC17;}
  }

  PilihwaktuC19:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     19.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC20;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 19; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC18;}
  }

  PilihwaktuC20:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     20.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC21;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 20; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC19;}
  }

  PilihwaktuC21:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     21.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC22;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 21; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC20;}
  }

  PilihwaktuC22:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     22.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC23;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 22; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC21;}
  }

  PilihwaktuC23:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     23.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC24;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 23; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC22;}
  }

  PilihwaktuC24:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     24.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu3 = 0; goto PilihbobotC100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuC23;}
  }

  PilihbobotC100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      100 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotC200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan3 = 100; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal3;}
  }

  PilihbobotC200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      200 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotC300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan3 = 200; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotC100;}
  }

  PilihbobotC300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      300 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotC400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan3 = 300; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotC200;}
  }

  PilihbobotC400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      400 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotC500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan3 = 400; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotC300;}
  }

  PilihbobotC500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      500 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihbobotA400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan3 = 500; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal3;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotC400;}
  }

//____________________________JADWAL KEEMPAT_________________________________

  Pilihjadwal4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    PILIH JADWAL    ");
    lcd.setCursor(0,1); lcd.print("      KEEMPAT       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD0;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal3;}
  }

  PilihwaktuD0:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  Kosongkan Jadwal  ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK    Tidak");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 0; setpakan4 = 0; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal4;}
  }

  PilihwaktuD1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     01.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 1; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD0;}
  }

  PilihwaktuD2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     02.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 2; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD1;}
  }

  PilihwaktuD3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     03.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 3; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD2;}
  }

  PilihwaktuD4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     04.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 4; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD3;}
  }

  PilihwaktuD5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     05.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD6;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 5; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD4;}
  }

  PilihwaktuD6:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     06.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD7;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 6; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD5;}
  }

  PilihwaktuD7:
  while(1){
    
    lcd.setCursor(0,0);
    lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     07.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD8;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 7; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD6;}
  }

  PilihwaktuD8:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     08.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD9;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 8; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD7;}
  }

  PilihwaktuD9:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     09.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD10;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 9; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD8;}
  }

  PilihwaktuD10:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     10.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD11;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 10; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD9;}
  }

  PilihwaktuD11:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     "); 
    lcd.setCursor(0,1); lcd.print("     11.00 WIB      "); 
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 11; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD10;}
  }

  PilihwaktuD12:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     12.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD13;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 12; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD11;}
  }

  PilihwaktuD13:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     13.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD14;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 13; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD12;}
  }

  PilihwaktuD14:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     14.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD15;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 14; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD13;}
  }

  PilihwaktuD15:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     15.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD16;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 15; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD14;}
  }

  PilihwaktuD16:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     16.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD17;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 16; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD15;}
  }

  PilihwaktuD17:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     17.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD18;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 17; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD16;}
  }

  PilihwaktuD18:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     18.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD19;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 18; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD17;}
  }

  PilihwaktuD19:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     19.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD20;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 19; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD18;}
  }

  PilihwaktuD20:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     20.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD21;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 20; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD19;}
  }

  PilihwaktuD21:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     21.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD22;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 21; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD20;}
  }

  PilihwaktuD22:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     22.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD23;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 22; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD21;}
  }

  PilihwaktuD23:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     23.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD24;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 23; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD22;}
  }

  PilihwaktuD24:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     24.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu4 = 0; goto PilihbobotD100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuD23;}
  }

  PilihbobotD100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      100 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotD200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan4 = 100; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal4;}
  }

  PilihbobotD200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      200 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotD300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan4 = 200; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotD100;}
  }

  PilihbobotD300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      300 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotD400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan4 = 300; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotD200;}
  }

  PilihbobotD400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      400 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotD500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan4 = 400; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotD300;}
  }

  PilihbobotD500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      500 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotA400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan4 = 500; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal4;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotD400;}
  }

//____________________________JADWAL KELIMA_________________________________

  Pilihjadwal5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    PILIH JADWAL    ");
    lcd.setCursor(0,1); lcd.print("       KELIMA       ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Home");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Home;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE0;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal4;}
  }

  PilihwaktuE0:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  Kosongkan Jadwal  ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK    Tidak");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE1;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 0; setpakan5 = 0; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000); goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal5;}
  }

  PilihwaktuE1:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     01.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 1; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE0;}
  }

  PilihwaktuE2:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     02.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 2; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE1;}
  }

  PilihwaktuE3:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     03.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 3; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE2;}
  }

  PilihwaktuE4:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     04.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE5;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 4; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE3;}
  }

  PilihwaktuE5:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     05.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE6;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 5; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE4;}
  }

  PilihwaktuE6:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     06.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE7;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 6; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE5;}
  }

  PilihwaktuE7:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     07.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE8;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 7; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE6;}
  }

  PilihwaktuE8:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     08.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE9;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 8; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE7;}
  }

  PilihwaktuE9:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     09.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE10;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 9; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE8;}
  }

  PilihwaktuE10:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     10.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE11;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 10; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE9;}
  }

  PilihwaktuE11:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     11.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 11; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE10;}
  }

  PilihwaktuE12:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     12.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE13;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 12; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE11;}
  }

  PilihwaktuE13:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     13.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE14;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 13; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE12;}
  }

  PilihwaktuE14:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     14.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE15;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 14; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE13;}
  }

  PilihwaktuE15:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     15.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE16;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 15; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE14;}
  }

  PilihwaktuE16:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     16.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE17;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 16; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE15;}
  }

  PilihwaktuE17:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     17.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE18;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 17; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE16;}
  }

  PilihwaktuE18:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     18.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE19;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 18; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE17;}
  }

  PilihwaktuE19:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     19.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE20;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 19; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE18;}
  }

  PilihwaktuE20:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     20.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE21;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 20; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE19;}
  }

  PilihwaktuE21:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     21.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE22;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 21; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE20;}
  }

  PilihwaktuE22:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     22.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE23;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 22; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE21;}
  }

  PilihwaktuE23:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     23.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE24;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 23; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE22;}
  }

  PilihwaktuE24:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Waktu     ");
    lcd.setCursor(0,1); lcd.print("     24.00 WIB      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");
    
    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihwaktuA12;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setwaktu5 = 0; goto PilihbobotE100;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihwaktuE23;}
  }

  PilihbobotE100:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      100 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("Back     OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotE200;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan5 = 100; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihjadwal5;}
  }

  PilihbobotE200:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      200 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotE300;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan5 = 200; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotE100;}
  }

  PilihbobotE300:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      300 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotE400;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan5 = 300; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotE200;}
  }

  PilihbobotE400:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      400 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK       + ");
    
    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto PilihbobotE500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan5 = 400; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotE300;}
  }

  PilihbobotE500:
  while(1){
    
    lcd.setCursor(0,0); lcd.print("    Pilih Bobot     ");
    lcd.setCursor(0,1); lcd.print("      500 Gram      ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print(" -       OK         ");

    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto PilihbobotE500;}
      if (Oke == HIGH) { delay(100); lcd.clear(); setpakan5 = 500; lcd.setCursor(0,1); lcd.print(" Tunggu Sebentar... "); delay(2000);goto Pilihjadwal5;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto PilihbobotE400;}
  }

  Pilihwifi:
  while(1){

    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("     SETUP WIFI     ");
    lcd.setCursor(0,2); lcd.print("                    ");
    lcd.setCursor(0,3); lcd.print("         OK     Back");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    //Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Home;}
      if (Oke == HIGH) { delay(100); lcd.clear(); goto Pilihwifi1;}
      //if (Kiri == HIGH) { delay(100); lcd.clear(); goto Home;}
  }

  Pilihwifi1:
  while(1){

    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("  BERKAH SAYUR !!!  ");
    lcd.setCursor(0,2); lcd.print("      ********      ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihwifi2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); ssid = "BERKAH SAYUR !!!"; password = "SayurBerkah"; setup_wifi(); klient.setServer(mqtt_server, 1883); klient.setCallback(callback); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihwifi;}
  }

  Pilihwifi2:
  while(1){

    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("     vivo 1802      ");
    lcd.setCursor(0,2); lcd.print("      *******       ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihwifi3;}
      if (Oke == HIGH) { delay(100); lcd.clear(); ssid = "vivo 1802"; password = "WestPW2112"; setup_wifi(); klient.setServer(mqtt_server, 1883); klient.setCallback(callback); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihwifi1;}
  }

  Pilihwifi3:
  while(1){

    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print(" Galaxy A52s 5GA8C8 ");
    lcd.setCursor(0,2); lcd.print("      ********      ");
    lcd.setCursor(0,3); lcd.print("Back     OK     Next");

    Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      if (Kanan == HIGH) { delay(100); lcd.clear(); goto Pilihwifi4;}
      if (Oke == HIGH) { delay(100); lcd.clear(); ssid = "Galaxy A52s 5GA8C8"; password = "obgn9197"; setup_wifi(); klient.setServer(mqtt_server, 1883); klient.setCallback(callback); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihwifi2;}
  }

  Pilihwifi4:
  while(1){

    lcd.setCursor(0,0); lcd.print("                    ");
    lcd.setCursor(0,1); lcd.print("     indopride      ");
    lcd.setCursor(0,2); lcd.print("      *******       ");
    lcd.setCursor(0,3); lcd.print("Back     OK         ");

    //Kanan = digitalRead(pbkanan);
    Oke = digitalRead(pboke);
    Kiri = digitalRead(pbkiri);

      //if (Kanan == LOW) { delay(100); lcd.clear(); goto Pilihwifi2;}
      if (Oke == HIGH) { delay(100); lcd.clear(); ssid = "indopride"; password = "BAB@2000"; setup_wifi(); klient.setServer(mqtt_server, 1883); klient.setCallback(callback); goto Home;}
      if (Kiri == HIGH) { delay(100); lcd.clear(); goto Pilihwifi3;}
  }

}
