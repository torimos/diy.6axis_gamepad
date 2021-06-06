#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#define I2C_BAUDRATE 100000


class I2Cdev {
public:
	static void initialize();
	static void deinitialize();

	static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
	static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
	static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
	static int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data);
	static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
	static int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

	static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
	static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
	static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
	static bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
	static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
	static bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);
	
private:
	static bool readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length);
	static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length);
};

#ifdef __cplusplus
}
#endif

#endif