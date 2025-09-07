//am2320.cpp

/*!
 * @file Adafruit_AM2320.h
 *
 * This is a library for the AM2320 Temperature & Humidity Unified Sensor
 * breakout board
 * ----> https://www.adafruit.com/products/xxxx
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried for Adafruit Industries.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

//the original license because I used some of the code
//from the am2320 library for the arduino

#include "am2320.h"

#include <ctime>
void delay(uint32_t ms) {
	clock_t start_time = clock();
	clock_t end_time = start_time + (ms * CLOCKS_PER_SEC/1000);
	while (clock() < end_time);
}

using namespace Adafruit;

AM2320::AM2320(const char *path) {
	this->path = path;
	device = i2c::device(AM2320_I2C_PORT, path);
}

float AM2320::temperature_C() {
	return (read_register32( (uint8_t)Register::TEMP_H ) >> 16) / 10.0;
}

float AM2320::temperature_F() {
	return (temperature() * 1.8f) + 32;
}

float AM2320::humidity() {
	return (read_register32( (uint8_t)Register::HUM_H ) >> 16) / 10.0;
}

uint16_t AM2320::crc16(uint8_t *ptr, uint8_t len) {
	uint16_t crc = 0xFFFF;
	uint8_t i;
	
	while (len--) {
		crc ^= *ptr++;
		
		for (i = 0; i < 8; i++) {
			if (crc & 0x01) {
				crc >>= 1;
				crc ^= 0xA001;
			} else {
				crc >>= 1;
			}
		}
	}

	return crc;
}

uint32_t AM2320::read_register32(uint8_t reg) {
	uint8_t buffer[8] = {0,0,0,0,0,0,0,0};
	bool written = false;
	
	//wake
	for (int i = 0; i < 3; i++) {
		written = device.write(buffer, 1);
		if (written) { break; }
		delay(100);
	}

	if (!written) { return -1; }

	delay(10);

	buffer[0] = AM2320_CMD_READREG;
	buffer[1] = reg;
	buffer[2] = 4; // 4 bytes

	for (int i = 0; i < 3; i++) {
		written = device.write(buffer, 3);
		if (written) { break; }
		delay(5);
	}

	if (!written) { return -1; }

	delay(2);

	device.read(buffer, 8);

	if (buffer[0] != 0x03) { return -1; }
	if (buffer[1] != 0x04) { return -1; }

	uint16_t crc1 = buffer[7];
	uint16_t crc2 = crc16(buffer, 6);
	
	crc1 <<= 8;
	crc1 |= buffer[6];

	if (crc1 != crc2) { return -1; }

	return uint32_t(buffer[2]) << 24 | uint32_t(buffer[3]) << 16 | uint32_t(buffer[4]) << 8 | uint32_t(buffer[5]);
}