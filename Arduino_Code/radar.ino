#define BLYNK_TEMPLATE_ID "TMPL3Xer1uMPh"
#define BLYNK_TEMPLATE_NAME "RADAR"
#define BLYNK_AUTH_TOKEN "wV6ZqTL92aJOw8kkexGgv9DovyqsLVVx"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "OnePlus Nord CE 3 Lite 5G";
char pass[] = "Akanksh31";

const int trigPin = 12;
const int echoPin = 14;
const int servoPin = 13;

const int buzzerPin = 27;

const int greenLed = 25;
const int yellowLed = 26;
const int redLed = 33;

long duration;

int distance;

int notificationSent = 0;

Servo myServo;

void setup() {

	pinMode(trigPin, OUTPUT);

	pinMode(echoPin, INPUT);

	pinMode(buzzerPin, OUTPUT);

	pinMode(greenLed, OUTPUT);

	pinMode(yellowLed, OUTPUT);

	pinMode(redLed, OUTPUT);

	Serial.begin(115200);

	Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

	myServo.setPeriodHertz(50);

	myServo.attach(servoPin, 500, 2400);
}

void loop() {

	int i;

	Blynk.run();

	for(i = 0; i <= 180; i++) {

		Blynk.run();

		myServo.write(i);

		delay(30);

		distance = calculateDistance();

		checkIndicators(distance);

		sendData(i, distance);
	}

	for(i = 180; i >= 0; i--) {

		Blynk.run();

		myServo.write(i);

		delay(30);

		distance = calculateDistance();

		checkIndicators(distance);

		sendData(i, distance);
	}
}

int calculateDistance() {

	digitalWrite(trigPin, LOW);

	delayMicroseconds(2);

	digitalWrite(trigPin, HIGH);

	delayMicroseconds(10);

	digitalWrite(trigPin, LOW);

	duration = pulseIn(echoPin, HIGH, 30000);

	if(duration == 0) {

		return 400;
	}

	distance = duration * 0.034 / 2;

	return distance;
}

void checkIndicators(int dist) {

	if(dist >= 0 && dist <= 20) {

		digitalWrite(redLed, HIGH);

		digitalWrite(yellowLed, LOW);

		digitalWrite(greenLed, LOW);

		digitalWrite(buzzerPin, HIGH);

		if(notificationSent == 0) {

			Blynk.logEvent("obstacle_alert");

			notificationSent = 1;
		}
	}

	else if(dist > 20 && dist <= 40) {

		digitalWrite(redLed, LOW);

		digitalWrite(yellowLed, HIGH);

		digitalWrite(greenLed, LOW);

		digitalWrite(buzzerPin, LOW);

		notificationSent = 0;
	}

	else {

		digitalWrite(redLed, LOW);

		digitalWrite(yellowLed, LOW);

		digitalWrite(greenLed, HIGH);

		digitalWrite(buzzerPin, LOW);

		notificationSent = 0;
	}
}

void sendData(int angle, int dist) {

	Serial.print(angle);

	Serial.print(",");

	Serial.print(dist);

	Serial.print(".");
}
