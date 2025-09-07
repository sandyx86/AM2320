//am2320.h

#ifndef ADAFRUIT_AM2320
#define ADAFRUIT_AM2320

#include "i2c.h"

#define AM2320_SENSOR_VERSION 1
#define AM2320_CMD_READREG 0x03
#define AM2320_REG_TEMP_H 0x02
#define AM2320_REG_HUM_H 0x00
#define AM2320_I2C_PORT 0x5c

namespace Adafruit {
	
enum class Register {
	HUM_H = 0x00, HUM_L,
	TEMP_H, TEMP_L,
	
	MODEL_H = 0x08, MODEL_L,
	VERSION, ID
};

class AM2320 {
	const char *path;
	i2c::device device = i2c::device(AM2320_I2C_PORT, path);

	uint16_t crc16(uint8_t *buffer, uint8_t bytes);
	uint32_t read_register32(uint8_t reg);

public:
	AM2320(const char *path);
	
	float temperature();
	float humidity();
};


};

#endif