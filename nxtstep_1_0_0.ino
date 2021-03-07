#include <Wire.h>
#include "SSD1306Wire.h"
#include "nxtstepLogo.h"
#include "robotoCondensed_24.h"
#include "WiFi.h"

#define SDA1 13
#define SCL1 14
#define SDA2 15
#define SCL2 16

// Initialize the OLED display using Wire library
SSD1306Wire displayLeft(0x3c, SCL1, SDA1);
SSD1306Wire displayRight(0x3c, SCL2, SDA2);

const char *ssid = "nxtPager";
const char *password = "licet@123";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  displayLeft.init();
  displayRight.init();

  // This will make sure that multiple instances of a display driver
  // running on different ports will work together transparently
  displayLeft.setI2cAutoInit(true);
  displayRight.setI2cAutoInit(true);

  displayLeft.flipScreenVertically();
  displayRight.flipScreenVertically();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    displayLeft.setTextAlignment(TEXT_ALIGN_LEFT);
    displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
    displayLeft.clear();
    displayRight.clear();
    displayLeft.setFont(ArialMT_Plain_10);
    displayRight.setFont(ArialMT_Plain_10);
    displayLeft.drawString(16, 8, "brand logo");

    displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
    displayLeft.drawString(64, 48, "Connecting...");
    displayLeft.setFont(Roboto_Condensed_Light_24);
    displayLeft.drawString(64, 16, "nxt+");

    displayRight.setFont(ArialMT_Plain_10);
    displayRight.drawString(16, 8, "powered by");

    displayRight.setTextAlignment(TEXT_ALIGN_CENTER);
    displayRight.setFont(Roboto_Condensed_Light_24);
    displayRight.drawString(64, 16, "nxtSTEP");

    displayRight.setFont(ArialMT_Plain_10);
    displayRight.drawString(64, 48, "ver 1.0.0 alpha");

    displayLeft.display();
    displayRight.display();
  }
  displayLeft.setTextAlignment(TEXT_ALIGN_LEFT);
  displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
  displayLeft.clear();
  displayRight.clear();
  displayLeft.setFont(ArialMT_Plain_10);
  displayRight.setFont(ArialMT_Plain_10);
  displayLeft.drawString(16, 8, "brand logo");

  displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
  displayLeft.drawString(64, 48, "Connected to nxt network");
  displayLeft.setFont(Roboto_Condensed_Light_24);
  displayLeft.drawString(64, 16, "nxt+");

  displayRight.setFont(ArialMT_Plain_10);
  displayRight.drawString(16, 8, "powered by");

  displayRight.setTextAlignment(TEXT_ALIGN_CENTER);
  displayRight.setFont(Roboto_Condensed_Light_24);
  displayRight.drawString(64, 16, "nxtSTEP");

  displayRight.setFont(ArialMT_Plain_10);
  displayRight.drawString(64, 48, "ver 1.0.0 alpha");

  displayLeft.display();
  displayRight.display();
}

void loop()
{
}