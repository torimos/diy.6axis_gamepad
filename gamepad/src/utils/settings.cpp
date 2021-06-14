#include "main.h"

settings_t settings;

bool settings_init()
{
    bool res = true;
    uint32_t crc = 0;
    eeprom_read_data(0, &crc, sizeof(uint32_t));
    eeprom_read_data(4, &settings, sizeof(settings_t));
    uint32_t dataCrc = get_CRC32((uint8_t*)&settings, sizeof(settings_t));
    if (crc > 0 && dataCrc != crc)
    {
        settings.calib_enabled = false;
        settings.uart_adapter_enabled = false;
        res = false;
    }
    #if DEBUG_LVL >= 1
    Serial.printf("res:%d crc:%08x calib_enabled:%d scale_enabled:%d uart_adapter_enabled:%d\n\r",res, crc, settings.calib_enabled, settings.scale_enabled, settings.uart_adapter_enabled);
    if (res) {
        Serial.printf("scale_from:%d scale_to:%d\n\r", settings.scale_from, settings.scale_to);
        Serial.printf("left x[%d:%d:%d] y[%d:%d:%d]\n\r", settings.calib_left.xmin, settings.calib_left.xmax, settings.calib_left.xoffs, settings.calib_left.ymin, settings.calib_left.ymax, settings.calib_left.yoffs);
        Serial.printf("right x[%d:%d:%d] y[%d:%d:%d]\n\r", settings.calib_right.xmin, settings.calib_right.xmax, settings.calib_right.xoffs, settings.calib_right.ymin, settings.calib_right.ymax, settings.calib_right.yoffs);
    }
    #endif
    return res;
}