#include "common.h"

HardwareSerial STM32uart(1);
uint8_t rx_buf[0x40];
uint8_t frame_buf[0x80];
int frame_buf_len = 0;
report_t *_report;

int16_t get_expected_frame_size(uint16_t frame_type, bool flag = true)
{
  switch(frame_type)
  {
    case 0x1: return sizeof(joy_data_t) + sizeof(sens_data_t);
  }
  return -1;
}

int uart_read()
{
  int rx_len =  STM32uart.read(rx_buf, sizeof(rx_buf));
  STM32uart.flush();
  if (rx_len <= 0) 
    return rx_len;
  else
  {
    if ((frame_buf_len + rx_len) > sizeof(frame_buf))
    {
      // data overflow
      frame_buf_len = 0;
    }
    memcpy(frame_buf+frame_buf_len, rx_buf, rx_len);
    frame_buf_len += rx_len;
    return rx_len;
  }
  return 0;
}

int parse_frame()
{
    if (frame_buf_len > 0)
    {
        int frame_start_offset = 0;
        bool header_found = false;
        while(frame_start_offset < (frame_buf_len-4))
        {
            uint32_t* header = (uint32_t*)&frame_buf[frame_start_offset];
            if (*header ==  0x413242aa)
            {
                header_found = true;
                break;
            }
            frame_start_offset++;
        }
        if (header_found)
        {
            // align frame start with 0 start index in buffer
            memmove(frame_buf, &frame_buf[frame_start_offset], frame_buf_len-frame_start_offset);
            frame_buf_len = frame_buf_len-frame_start_offset;
            if ((sizeof(frame_buf) - frame_buf_len) >= 1)
                memset(frame_buf+frame_buf_len, 0x55, sizeof(frame_buf) - frame_buf_len);
            if (frame_buf_len > 6)
            {
                uint16_t* pftype = (uint16_t*)&frame_buf[4];
                uint16_t* pfsize = (uint16_t*)&frame_buf[6];
                uint16_t frame_type = *pftype;
                int16_t frame_size = (int16_t)*pfsize;
                int16_t expected_frame_size = get_expected_frame_size(frame_type);
                if (frame_size == expected_frame_size && frame_size > 0 && frame_size <= frame_buf_len)
                {
                    uint32_t expected_crc32 = get_CRC32((uint8_t*)&frame_buf[8], frame_size);
                    uint32_t actual_crc32 = *((uint32_t*)(&frame_buf[8]+frame_size));
                    bool crc_valid = expected_crc32 == actual_crc32;
                    if (crc_valid)
                    {
                        _report->crc = actual_crc32;
                        memcpy(&_report->data, frame_buf+8, frame_size);
                        frame_buf_len = 0;
                        #if DEBUG_LVL >= 3
                        printf("%s[%ld]: frame[%x] size: %d, checksum[%d]: %08x\n\r", __func__, millis(), frame_type, frame_size, crc_valid, actual_crc32);
                        #endif
                        return 1;
                    }
                    #if DEBUG_LVL >= 3
                    printf("%s[%ld]: frame[%x] size: %d, checksum[%d]: %08x\n\r", __func__, millis(), frame_type, frame_size, crc_valid, actual_crc32);
                    #endif
                    return -3;
                }
                #if DEBUG_LVL >= 3
                printf("%s[%ld]: frame[%x] size: %d, expected size: %d\n\r", __func__, millis(), frame_type, frame_size, expected_frame_size);
                #endif
                return -2;
            }
        }
        #if DEBUG_LVL >= 3
        printf("%s[%ld]: frame found: %d, offs: %d, size: %d\n\r", __func__, millis(), header_found, frame_start_offset, frame_buf_len);
        #endif
        return -1;
    }
    return 0;
}

void stm32_uart_init(report_t *preport)
{
    _report = preport;
    STM32uart.begin(921600, SERIAL_8N1, 27, 14);
}

int stm32_uart_process()
{
    int r = parse_frame();
    uart_read();
    return r;
}