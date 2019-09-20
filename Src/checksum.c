#include "checksum.h"

const uint32_t MAGIC_VALUE = 0x990C9AB5;
uint32_t value = 0xFFFFFFFF;

void checksum_reset(void) { 
	value = 0xFFFFFFFF;
}

void checksum_update_value(uint8_t byte) {
		uint32_t temp = byte * MAGIC_VALUE;
		value = value + temp;
		uint32_t temp2 = value >> 1;
    value = value ^ temp2;
}

void checksum_update(uint8_t *array, uint8_t length) {
	for(uint8_t i = 0; i < length; i++) {
		checksum_update_value(array[i]);
	}
}

uint32_t checksum_get_value(void) {
	return value;
}
