
//************************************************************** Includes

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "RingBuffer.h"

//************************************************************** Constants

const char* ssid     = "HOTBOX-974A";
const char* password = "0523088413";

//************************************************************** Variables

WiFiServer server(80);

const int gpio2 = 2;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 

const long timeoutTime = 2000;

RingBuffer buf(8);

//************************************************************** Setup

void setup()
{
	Serial.begin(115200);

	pinMode(gpio2, OUTPUT);
	digitalWrite(gpio2, LOW);

	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	server.begin();
}

//************************************************************** Loop

void loop()
{
	WiFiClient client = server.available();

	if (client)
	{
		Serial.println("New Client");

		String currentLine = "";

		currentTime = millis();
		previousTime = currentTime;

		buf.init();

		while (client.connected() && currentTime - previousTime <= timeoutTime)
		{
			currentTime = millis();

			if (client.available())
			{

				char c = client.read();
				buf.push(c);
				Serial.write(c);

				if (buf.endsWith("\r\n\r\n"))
				{
					sendHttpResponse(client);
					break;
				}

				if (buf.endsWith("/on"))
				{
					Serial.println("GPIO 2 on");
					digitalWrite(gpio2, HIGH);
				}
				else if (buf.endsWith("/off"))
				{
					Serial.println("GPIO 2 off");
					digitalWrite(gpio2, LOW);
				}

			}
		}

		client.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
	}
}

//************************************************************** sendHttpResponse

void sendHttpResponse(WiFiClient client)
{
	client.println("HTTP/1.1 200 OK");
	client.println("Content-type:application/json");
	client.println();

	String message = "{ ";
	message += " value=";
	message += digitalRead(gpio2);
	message += " }";

	client.println(message);

	client.println();
}
