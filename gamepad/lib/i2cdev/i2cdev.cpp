#include "i2cdev.h"
#include <Wire.h>

/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
	bool response = readBytes(devAddr, regAddr, data, 1);
	*data &= (1 << bitNum);
	return response;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
	bool response = readBytes(devAddr, regAddr, data, 1);
	if (response) {
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		*data &= mask;
		*data >>= (bitStart - length + 1);
	}
	return response;
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data) {
	bool response = readBytes(devAddr, regAddr, data, 1);
	return response;
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
 */
int8_t I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
	bool response = readBytes(devAddr, regAddr, data, length);
	return response;
}

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @return Status of read operation (true = success)
 */
int8_t I2Cdev::readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data) {
	bool response = readBytes(devAddr, regAddr, (uint8_t*)data, 2);
	data[0] = (((uint8_t*)data)[0] << 8) | ((uint8_t*)data)[1];
	return response;
}

/** Read multiple words from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of words to read
 * @param data Buffer to store read data in
 * @return Number of words read (-1 indicates failure)
 */
int8_t I2Cdev::readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
	bool response = readBytes(devAddr, regAddr, (uint8_t*)data, length * 2);
	for (int i = 0; i < length; i++) {
		data[i] = (((uint8_t*)data)[i * 2] << 8) | ((uint8_t*)data)[i * 2 + 1];
	}
	return response;
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
	//first reading registery value
	uint8_t b;
	bool response = readBytes(devAddr, regAddr, &b, 1);
	if (response) {
		b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
		response = writeBytes(devAddr, regAddr, &b, 1);
	}
	return response;
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
	//first reading registery value
	uint8_t b;
	bool response = readBytes(devAddr, regAddr, &b, 1);
	if (response) {
		uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
		data <<= (bitStart - length + 1); // shift data into correct position
		data &= mask; // zero all non-important bits in data
		b &= ~(mask); // zero all important bits in existing byte
		b |= data; // combine data with existing byte
		response = writeBytes(devAddr, regAddr, &b, 1);
	}
	return response;
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
	bool response = writeBytes(devAddr, regAddr, &data, 1);
	return response;
}

bool I2Cdev::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data) {
	bool response = writeBytes(devAddr, regAddr, data, 1 + length);
	return response;
}

bool I2Cdev::writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data) {
	uint8_t buff[2];
	buff[0] = (uint8_t)(data >> 8); //MSByte
	buff[1] = (uint8_t)(data >> 0); //LSByte
	bool response = writeBytes(devAddr, regAddr, buff, 2);
	return response;
}

bool I2Cdev::writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data) {
	uint8_t buff[length];
	for (int i = 0; i < length; i++) {
		buff[0 + 2*i] = (uint8_t)(data[i] >> 8); //MSByte
		buff[1 + 2*i] = (uint8_t)(data[i] >> 0); //LSByte
	}
	bool response = writeBytes(devAddr, regAddr, buff, length);
	return response;
}

// Platform specific
static bool _initialized = false;
void I2Cdev::initialize() {
	if (!_initialized)
	{
		Wire.begin();
		Wire.setClock(I2C_BAUDRATE>>2);
		_initialized = true;
	}
}

void I2Cdev::deinitialize() {
	if (_initialized)
	{
    	Wire.end();
		_initialized = false;
	}
}

bool I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length) {
    Wire.beginTransmission(devAddr);
    Wire.write(regAddr);
    Wire.endTransmission();
    Wire.requestFrom(devAddr, (uint8_t)length);
    // Wait around until enough data is available
    while (Wire.available() < (int)length)
        ;
    return Wire.readBytes(data, length) == length;
}

bool I2Cdev::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t* data, size_t length) { 
    Wire.beginTransmission(devAddr);
    Wire.write((uint8_t)regAddr);
    Wire.write((uint8_t*)data, (size_t)length);
    return Wire.endTransmission() == 0;
}
