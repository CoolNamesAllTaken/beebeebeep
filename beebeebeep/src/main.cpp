#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SPEAKER_PIN 3
#define BUTTON_PIN 2

#define LED_PIN 13
#define LED_COUNT 26

#define DEBOUNCE_INTERVAL_MS 200

#define BEEP_DURATION_MS 50
#define BEEP_SCORE_LENGTH 20

uint32_t next_read_time = millis();

uint32_t next_beep_note_time = millis();
uint16_t beep_score_index = 0;
uint8_t beep_score[BEEP_SCORE_LENGTH] = 
{
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0
};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");

  pinMode(SPEAKER_PIN, OUTPUT);
  TCCR2B = TCCR2B & (B11111000 | B00000101); // for PWM frequency of 245.10 Hz
  analogWrite(SPEAKER_PIN, 127);

  pinMode(BUTTON_PIN, INPUT);

  strip.begin();
}

void beep_start() {
  beep_score_index = 0;
  next_beep_note_time = millis() + BEEP_DURATION_MS;
}

void beep_on() {
  analogWrite(SPEAKER_PIN, 127);
}

void beep_off() {
  analogWrite(SPEAKER_PIN, 0);
}

void led_red() {
  for (size_t i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 0, 255, 0, 0);
  }
}

void led_green() {
  for (size_t i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 255, 0, 0, 0);
  }
}

void beep_play() {
  if (beep_score_index >= BEEP_SCORE_LENGTH) {
    beep_off(); // stop beeping if you fell off the end of the score
    led_green();
    return;
  }

  beep_score[beep_score_index] ? beep_on() : beep_off();

  if (millis() > next_beep_note_time) {
    next_beep_note_time = millis() + BEEP_DURATION_MS;
    beep_score_index++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() > next_read_time && !digitalRead(BUTTON_PIN)) {
    // Button pin has been depressed
    next_read_time = millis() + DEBOUNCE_INTERVAL_MS;
    Serial.println(next_read_time);
    beep_start();
    led_red();
  }

  beep_play();
  strip.show();
}