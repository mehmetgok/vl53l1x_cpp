#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#ifdef __cplusplus
extern "C"
{
#endif

void i2c_setup(uint8_t bus, uint8_t addr);

void i2c_release();

int i2c_readRegisterMulti(uint16_t reg, size_t count, void *pdata);

int i2c_readRegisterByte(int reg, uint8_t *pdata);

int i2c_readRegisterWord(uint16_t reg, uint16_t *pdata);

int i2c_writeRegisterMulti(uint16_t reg, size_t count, void *pdata);

int i2c_writeRegisterByte(uint16_t reg, uint8_t value);

int i2c_writeRegisterWord(uint16_t reg, uint16_t value);

#ifdef __cplusplus
}
#endif