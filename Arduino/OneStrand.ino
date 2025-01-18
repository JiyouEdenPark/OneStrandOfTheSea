#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <Adafruit_NeoPixel.h>

// Create MPR121 object
// MPR121 객체 생성
Adafruit_MPR121 mpr121 = Adafruit_MPR121();

// NeoPixel settings
// NeoPixel 설정
#define NEOPIXEL_PIN 13       // NeoPixel data pin // 네오픽셀 데이터 핀
#define NUMPIXELS 26          // Number of NeoPixels // 네오픽셀 개수
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Function: Gradually change NeoPixel color
// 함수: NeoPixel 색상 서서히 변경
void fadeToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t steps = 50, uint16_t delayMs = 20) {
  uint32_t currentColor = pixels.getPixelColor(0);
  uint8_t currentR = (currentColor >> 16) & 0xFF;
  uint8_t currentG = (currentColor >> 8) & 0xFF;
  uint8_t currentB = currentColor & 0xFF;

  for (int i = 1; i <= steps; i++) {
    uint8_t newR = currentR + ((r - currentR) * i / steps);
    uint8_t newG = currentG + ((g - currentG) * i / steps);
    uint8_t newB = currentB + ((b - currentB) * i / steps);
    setNeoPixelColor(newR, newG, newB);
    delay(delayMs);
  }
}

// Function: Set NeoPixel color
// 함수: NeoPixel 색상 설정
void setNeoPixelColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void setup() {
  Serial.begin(9600);
  // Start MPR121 and NeoPixel test
  Serial.println("MPR121 및 NeoPixel 테스트 시작");

  // Initialize MPR121
  // MPR121 초기화
  if (!mpr121.begin(0x5A)) {
    // MPR121 initialization failed! Check connection
    Serial.println("MPR121 초기화 실패! 연결을 확인하세요.");
    while (1);
  }

  // MPR121 successfully initialized
  Serial.println("MPR121이 성공적으로 초기화되었습니다.");

  // Initialize NeoPixel
  // NeoPixel 초기화
  pixels.begin();
  setNeoPixelColor(0, 0, 0); // Turn off all LEDs // 모든 LED 끄기
}

void loop() {
  // Read MPR121 touch status
  // MPR121 터치 상태 읽기
  uint16_t touched = mpr121.touched();

  // Process when two pins are touched simultaneously
  // 두 핀이 동시에 터치된 경우 우선 처리
  if ((touched & (1 << 0)) && (touched & (1 << 9))) {
    // Pin 0 and 9 are touched simultaneously!
    Serial.println("0번과 9번 핀이 동시에 터치되었습니다!");
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
      setNeoPixelColor(brightness, brightness, brightness); // 흰색 디밍
      delay(20);
    }
    for (int brightness = 0; brightness <= 255; brightness += 5) {
      setNeoPixelColor(brightness, brightness, brightness); // 흰색 디밍
      delay(20);
    }
  }
  // Detect pin 0 touch
  // 0번 핀 터치 감지
  else if (touched & (1 << 0)) {
    // Pin 0 is touched!
    Serial.println("0번 핀이 터치되었습니다!");
    fadeToColor(207, 156, 255, 50, 10); // Gradually change to #cf9cff color // #cf9cff 색상으로 서서히 빛남
  }
  // Detect pin 9 touch
  // 9번 핀 터치 감지
  else if (touched & (1 << 9)) {
    // Pin 9 is touched!
    Serial.println("9번 핀이 터치되었습니다!");
    fadeToColor(166, 255, 255, 50, 10); // Gradually change to #a6ffff color // #a6ffff 색상으로 서서히 빛남
  }
  // Turn off LEDs when no pins are touched
  // 아무 핀도 터치되지 않은 경우 LED 끄기
  else {
    fadeToColor(0, 0, 0, 50, 10); // Turn off LEDs // LED 끄기
  }

  delay(100); // Loop delay // 반복 주기
}
