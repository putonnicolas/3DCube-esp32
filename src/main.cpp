#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Wire.h>

#define TFT_CS 5
#define TFT_DC 4
#define TFT_RST 14
#define TFT_SCLK 18
#define TFT_MOSI 23

#define background ST7735_BLACK
#define cote 70

// ESP-32
#define ENCODER_CLK 25
#define ENCODER_DT 26
#define BUTTON_PIN 27

void drawCube(int vertices[8][2], uint16_t color);

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const int vertices[8][3] = {
  {-1, -1, 1},
  {1, -1, 1},
  {1, 1, 1},
  {-1, 1, 1},
  {-1, -1, -1},
  {1, -1, -1},
  {1, 1, -1},
  {-1, 1, -1}
};

int rotatedVertices[8][2];
int zOffset = -4;
float angle = 0.0;
int cubeSize = 100;

uint8_t lastClk = HIGH;
String lastDirection = "";

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  tft.initR(INITR_BLACKTAB);

  Serial.print(F("Initialized !"));
  tft.fillScreen(background);
}

void loop() {
  int newClk = digitalRead(ENCODER_CLK);
  unsigned long currentTime = millis();
  cubeSize = 100;

  if (newClk != lastClk) {
    lastClk = newClk;
    int dtValue = digitalRead(ENCODER_DT);

    if (newClk == LOW && dtValue == HIGH) {
      angle += 0.2;
      lastDirection = "Clockwise";
      drawCube(rotatedVertices, background);
    }

    if (newClk == LOW && dtValue == LOW) {
      angle -= 0.2;
      lastDirection = "Counterwise";
      drawCube(rotatedVertices, background);
    }
  }

  for (int i = 0; i < 8; i++) {
    rotatedVertices[i][0] = round(64 + ((vertices[i][0] * cos(angle) - vertices[i][2] * sin(angle)) / (vertices[i][0] * sin(angle) + vertices[i][2] * cos(angle) + zOffset)) * cubeSize);
    rotatedVertices[i][1] = round(80 + (vertices[i][1] / (vertices[i][0] * sin(angle) + vertices[i][2] * cos(angle) + zOffset)) * cubeSize);
  }

  drawCube(rotatedVertices, ST7735_WHITE);
}

void drawCube(int vertices[8][2], uint16_t color) {
  // Cotés premier cube
  tft.drawLine(vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], color);
  tft.drawLine(vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1], color);
  tft.drawLine(vertices[2][0], vertices[2][1], vertices[3][0], vertices[3][1], color);
  tft.drawLine(vertices[3][0], vertices[3][1], vertices[0][0], vertices[0][1], color);
  // Cotés second cube
  tft.drawLine(vertices[4][0], vertices[4][1], vertices[5][0], vertices[5][1], color);
  tft.drawLine(vertices[5][0], vertices[5][1], vertices[6][0], vertices[6][1], color);
  tft.drawLine(vertices[6][0], vertices[6][1], vertices[7][0], vertices[7][1], color);
  tft.drawLine(vertices[7][0], vertices[7][1], vertices[4][0], vertices[4][1], color);
  // Intersections
  tft.drawLine(vertices[0][0], vertices[0][1], vertices[4][0], vertices[4][1], color);
  tft.drawLine(vertices[1][0], vertices[1][1], vertices[5][0], vertices[5][1], color);
  tft.drawLine(vertices[2][0], vertices[2][1], vertices[6][0], vertices[6][1], color);
  tft.drawLine(vertices[3][0], vertices[3][1], vertices[7][0], vertices[7][1], color);
}
