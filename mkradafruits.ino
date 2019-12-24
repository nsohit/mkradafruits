#define SECRET_PINNUMBER     ""
#define SECRET_GPRS_APN      "internet" // replace your GPRS APN
#define SECRET_GPRS_LOGIN    ""    // replace with your GPRS login
#define SECRET_GPRS_PASSWORD "" // replace with your GPRS password
#define IO_USERNAME    "NUR_SOHIT17"
#define IO_KEY         "98b36ea5b70a42818fdcdde6453f9c3c"
//#include "AdafruitIO_WiFi.h"
AdafruitIO_Feed *sent = io.feed("tes");

#include <MKRGSM.h>
const char PINNUMBER[]     = SECRET_PINNUMBER;
// APN data
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

GSMSSLClient client;
GPRS gprs;
GSM gsmAccess;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
 }

 Serial.println("Starting Arduino GPRS ping.");
 // connection state
 bool connected = false;

 // After starting the modem with GSM.begin()
 // attach the shield to the GPRS network with the APN, login and password
 while (!connected) {
   if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
       (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD,IO_USERNAME,IO_KEY) == GPRS_READY)) {
     connected = true;
   } else {
     Serial.println("Not connected");
     delay(1000);
   }
 }

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
}
 Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  // put your main code here, to run repeatedly:
    io.run();
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(2000);
    return;
  }
  Serial.print("sending -> ");
  Serial.println((float)temperature);
  sent->save((float)temperature);

  // increment the count by 1
//  count++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
