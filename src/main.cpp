// Minimal ESP32 HUB75 example: header + seconds counter

#include <Arduino.h>
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

// Display object and timing state
static MatrixPanel_I2S_DMA* dma_display = nullptr;
static uint32_t lastTick = 0;
static uint32_t secondsElapsed = 0;

// Forward declarations
static void drawHeader();
static void drawCounter(uint32_t s);

void setup() {
	Serial.begin(115200);
	delay(100);
	Serial.println("MSChrono starting...");

	// Configure display (sizes and chain are provided via build flags)
	HUB75_I2S_CFG mxconfig(
		MATRIX_WIDTH,      // panel width
		MATRIX_HEIGHT,     // panel height
		CHAIN_LENGTH       // number of chained panels horizontally
	);

	// Explicit pin mapping from build flags (if defined)
#ifdef HUB75_R1
	mxconfig.gpio.r1 = HUB75_R1; mxconfig.gpio.g1 = HUB75_G1; mxconfig.gpio.b1 = HUB75_B1;
	mxconfig.gpio.r2 = HUB75_R2; mxconfig.gpio.g2 = HUB75_G2; mxconfig.gpio.b2 = HUB75_B2;
	mxconfig.gpio.a = HUB75_A; mxconfig.gpio.b = HUB75_B; mxconfig.gpio.c = HUB75_C; mxconfig.gpio.d = HUB75_D; mxconfig.gpio.e = HUB75_E;
	mxconfig.gpio.lat = HUB75_LAT; mxconfig.gpio.oe = HUB75_OE; mxconfig.gpio.clk = HUB75_CLK;
#endif

		dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	if (!dma_display->begin()) {
		Serial.println("RGB panel init failed");
		while (true) { delay(1000); }
	}

	dma_display->setBrightness8(160); // 0..255
	dma_display->clearScreen();

	// Quick self-test: color bars
	int w = MATRIX_WIDTH * CHAIN_LENGTH;
	int bar = max(1, w / 6);
	dma_display->fillRect(0 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(255, 0, 0));
	dma_display->fillRect(1 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(0, 255, 0));
	dma_display->fillRect(2 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(0, 0, 255));
	dma_display->fillRect(3 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(255, 255, 0));
	dma_display->fillRect(4 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(0, 255, 255));
	dma_display->fillRect(5 * bar, 0, bar, MATRIX_HEIGHT, dma_display->color565(255, 0, 255));
	delay(1000);

	dma_display->clearScreen();
	drawHeader();
	drawCounter(0);
	lastTick = millis();
}

void loop() {
	// 1Hz tick
	if (millis() - lastTick >= 1000) {
		lastTick += 1000;
		secondsElapsed++;
		drawHeader();
		drawCounter(secondsElapsed);
	}
}

static void drawHeader() {
	// Header band
	dma_display->fillRect(0, 0, MATRIX_WIDTH * CHAIN_LENGTH, 8, dma_display->color565(0, 0, 32));
	dma_display->setTextWrap(false);
	dma_display->setTextSize(1);
	dma_display->setCursor(1, 1);
	dma_display->setTextColor(dma_display->color565(255, 255, 255));
	dma_display->print("MSChrono");
}

static void drawCounter(uint32_t s) {
	// Clear area below header
	int totalW = MATRIX_WIDTH * CHAIN_LENGTH;
	dma_display->fillRect(0, 8, totalW, MATRIX_HEIGHT - 8, 0);

	char buf[16];
	snprintf(buf, sizeof(buf), "%lu", (unsigned long)s);

	dma_display->setTextWrap(false);
	dma_display->setTextSize(2);
	dma_display->setTextColor(dma_display->color565(255, 200, 0));

	// Try to center horizontally
	int16_t x1, y1; uint16_t w, h;
	dma_display->getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
	int x = max(0, (totalW - (int)w) / 2);
	int y = 8 + max(0, (MATRIX_HEIGHT - 8 - (int)h) / 2);
	dma_display->setCursor(x, y);
	dma_display->print(buf);
}