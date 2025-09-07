//new_i2c.h

#include <cstdlib>
#include <cstdint>

namespace i2c {
	class device {
		uint8_t addr;
		uint32_t busfd;

	public:
		device(uint8_t a, const char *path);
		bool read(uint8_t *buffer, size_t len);
		bool write(const uint8_t *buffer, size_t len);
	};
};
