#include "Wire.h"
#include "SSD1306Wire.h"
#include "nxtstepLogo.h"
#include "robotoCondensed_24.h"
#include "robotoCondensed_14.h"
#include "WiFi.h"
#include <UniversalTelegramBot.h>
#include "WiFiClientSecure.h"
#include "time.h"

#define SDA1 13
#define SCL1 14
#define SDA2 15
#define SCL2 16

// Initialize the OLED display using Wire library
SSD1306Wire displayLeft(0x3c, SCL1, SDA1);
SSD1306Wire displayRight(0x3c, SCL2, SDA2);

// WLAN Settings
const char *ssid = "LAPTOP-D040LLQ9 9163";
const char *password = "7668@x5E";

// Initialise Telegram Bot
const char BotToken[] = "1671540030:AAEAMUBDjfS4-w4_DSkct0HQmb23lozGyxo";
WiFiClientSecure net_ssl;
UniversalTelegramBot bot(BotToken, net_ssl);

int botRequestDelay = 100;
unsigned long lastTimeBotRan;

int lockScreenDelay = 10000;
unsigned long lastTimeLockScreenShown;

// Sync datetime with NTP server
const char *ntpServer = "time.google.com";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

void setup()
{
  Serial.begin(115200);
  splashScreen();
}

void loop()
{
  if (millis() > lastTimeLockScreenShown + lockScreenDelay) {
    lockScreen();
    lastTimeLockScreenShown = millis();
  }
  if (millis() > lastTimeBotRan + botRequestDelay)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void lockScreen()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    // return;
  }

  Serial.println(&timeinfo, "%H:%M\n%A\n%B %d %Y");

  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M %A %B %d %Y", &timeinfo);
  //print like "const char*"
  Serial.println(timeStringBuff);

  //Optional: Construct String object
  String asString(timeStringBuff);

  displayLeft.clear();
  displayRight.clear();

  displayLeft.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  displayLeft.setFont(Roboto_Condensed_14);
  displayLeft.drawStringMaxWidth(64, 24, 125, timeStringBuff);

  displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
  displayRight.setFont(ArialMT_Plain_10);
  displayRight.drawString(16, 8, "powered by");

  displayRight.setTextAlignment(TEXT_ALIGN_CENTER);
  displayRight.setFont(Roboto_Condensed_Light_24);
  displayRight.drawString(64, 24, "nxtSTEP");

  displayLeft.display();
  displayRight.display();
}

void splashScreen()
{

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
    displayLeft.clear();
    displayRight.clear();

    displayLeft.setFont(ArialMT_Plain_10);
    displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
    displayLeft.drawString(64, 48, "Connecting...");
    displayLeft.setFont(Roboto_Condensed_Light_24);
    displayLeft.drawString(64, 16, "nxt+");

    displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
    displayRight.setFont(ArialMT_Plain_10);
    displayRight.drawString(16, 8, "powered by");
    displayRight.drawString(64, 48, "ver 1.0.0 alpha");

    displayRight.setTextAlignment(TEXT_ALIGN_CENTER);
    displayRight.setFont(Roboto_Condensed_Light_24);
    displayRight.drawString(64, 16, "nxtSTEP");

    displayLeft.display();
    displayRight.display();
  }
  displayLeft.clear();
  displayRight.clear();

  displayLeft.setFont(ArialMT_Plain_10);
  displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
  displayLeft.drawString(64, 48, "Connected to nxt network");
  displayLeft.setFont(Roboto_Condensed_Light_24);
  displayLeft.drawString(64, 16, "nxt+");

  displayRight.setFont(ArialMT_Plain_10);
  displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
  displayRight.drawString(16, 8, "powered by");

  displayRight.setTextAlignment(TEXT_ALIGN_CENTER);
  displayRight.setFont(Roboto_Condensed_Light_24);
  displayRight.drawString(64, 16, "nxtSTEP");

  displayRight.setFont(ArialMT_Plain_10);
  displayRight.drawString(64, 48, "ver 1.0.0 alpha");

  displayLeft.display();
  displayRight.display();

  delay(1500);
  displayLeft.clear();
  displayLeft.setFont(ArialMT_Plain_10);
  displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
  displayLeft.drawString(64, 48, "Synchronising time...");
  displayLeft.setFont(Roboto_Condensed_Light_24);
  displayLeft.drawString(64, 16, "nxt+");
  displayLeft.display();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void showDualDisplayCentered(String lString, String rString)
{
  displayLeft.clear();
  displayRight.clear();
  displayLeft.setTextAlignment(TEXT_ALIGN_CENTER);
  displayLeft.setFont(ArialMT_Plain_10);
  displayLeft.drawString(64, 8, "New message from");
  displayLeft.setFont(Roboto_Condensed_Light_24);
  displayLeft.drawString(64, 24, lString);

  displayRight.setTextAlignment(TEXT_ALIGN_LEFT);
  displayRight.setFont(Roboto_Condensed_14);
  displayRight.drawStringMaxWidth(0, 0, 125, rString);

  displayLeft.display();
  displayRight.display();
}

void handleNewMessages(int numNewMessages)
{
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {

    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    Serial.println(text);

    showDualDisplayCentered(from_name, text);
    bot.sendMessage(bot.messages[i].chat_id, "Message Delivered");

    if (text == "/start")
    {
      String welcome = "Welcome, " + from_name + ".\n";
      bot.sendMessage(bot.messages[i].chat_id, welcome, "");
    }
  }
}
