#include "stm32l4xx.h"

void checksum_reset(void);
void checksum_update_value(uint8_t byte);
void checksum_update(uint8_t *array, uint8_t length);
uint32_t checksum_get_value(void);
