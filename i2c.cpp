//new_i2c.cpp
#include "i2c.h"

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

namespace sys {
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <unistd.h>
};

using namespace i2c;

device::device(uint8_t a, const char *path) {
	addr = a;
	busfd = sys::open(path, O_RDWR);

	if (this->busfd == -1) {
			//fprintf(stderr, "open failed: %s\nmaybe try again as root\n", path);
			return;
	}

	sys::ioctl(busfd, I2C_SLAVE, addr);
}

bool device::read(uint8_t *buffer, size_t len) {
	int ret = sys::read(this->busfd, buffer, len);
	return ret == len;
}

bool device::write(const uint8_t *buffer, size_t len) {
	int ret = sys::write(this->busfd, buffer, len);
	return ret == len;
}
