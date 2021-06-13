#include "main.h"

void eeprom_read_data(int pos, void* data, int len)
{
    int i = 0;
    while(i<len)
    {
        *((uint8_t*)((int)data+i)) = EEPROM.read(pos+i);
        i++;
    }
}

void eeprom_write_data(int pos, void* data, int len)
{
    int i = 0;
    while(i<len)
    {
        EEPROM.update(pos+i, *((uint8_t*)((int)data+i)));
        i++;
    }
}