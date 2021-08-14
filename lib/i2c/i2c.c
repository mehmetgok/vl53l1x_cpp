#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "i2c.h"

#define false 0
#define true 1

int fd;

void i2c_setup(uint8_t bus, uint8_t addr)
{
	char filename[50];
	sprintf(filename, "/dev/i2c-%d", bus);

	fd = open(filename, O_RDWR);

	if (fd < 0) {
		exit(-1);
	}

	int rc = ioctl(fd, I2C_SLAVE, addr);
	if (rc < 0) {
		exit(-2);
	}
}

void i2c_release()
{
	close(fd);
}

int i2c_readRegisterMulti(uint16_t reg, size_t count, void *pdata)
{
	reg = htons(reg); // little endian to big endian

	// write register number
	if (write(fd, &reg, sizeof(reg)) < 0) {
		return false;
	}

	return read(fd, pdata, count);
}

int i2c_readRegisterByte(int reg, uint8_t *pdata)
{
	return i2c_readRegisterMulti(reg, 1, pdata) > 0;
}

int i2c_readRegisterWord(uint16_t reg, uint16_t *pdata)
{
	if (i2c_readRegisterMulti(reg, 2, pdata) < 0) {
		return false;
	}
	*pdata = ntohs(*pdata); // big endian to little endian
	return true;
}

int i2c_writeRegisterMulti(uint16_t reg, size_t count, void *pdata)
{
	uint8_t buf[count + sizeof(reg)];

	reg = htons(reg); // little endian to big endian

	memcpy(buf, &reg, sizeof(reg));
	memcpy(buf + sizeof(reg), pdata, count);

	return write(fd, buf, count + sizeof(reg)) > 0;
}

int i2c_writeRegisterByte(uint16_t reg, uint8_t value)
{
	return i2c_writeRegisterMulti(reg, 1, &value);
}


int i2c_writeRegisterWord(uint16_t reg, uint16_t value)
{
	value = htons(value); // little endian to big endian
	return i2c_writeRegisterMulti(reg, 2, &value);
}
