#include "main.h"

USBD_HandleTypeDef* pdev = NULL;

uart_report_t uart_report;
usb_report_t usb_report;
usb_params_report_t usb_params_report;

uint32_t last_usb_report_time = 0;
uint32_t last_uart_report_time = 0;
uint32_t last_update_time = 0;
uint32_t sleep_pressed_last_time = 0;
uint32_t charging_blink_last_time = 0;
bool usb_con_state = false;
bool adapter_con_state = false;
uint32_t data_checksum = 0;
uint32_t last_usb_checksum = 0;
uint32_t last_uart_checksum = 0;


uint8_t params_buf[64];
uint8_t params_buf_len = 0;

void calib_apply(stick_calib_t* calib, int16_t *ax, int16_t *ay)
{
    int16_t scaleFrom = AXIS_MAX_VALUE;
    int16_t scaleTo = calib->scale_to;
    int16_t m2 = (short)(scaleFrom / 2);
    int16_t mTo2 = (short)(scaleTo / 2);
    int16_t cx = (m2 + calib->xoffs);
    int16_t cy = (m2 + calib->yoffs);

    double scaleFactor = (scaleTo / (double)scaleFrom);
    double dXmin = m2 / (double)(cx - calib->xmin);
    double dXmax = m2 / (double)(calib->xmax - cx);
    double dYmin = m2 / (double)(cy - calib->ymin);
    double dYmax = m2 / (double)(calib->ymax - cy);

    int16_t x = *ax;
    int16_t y = *ay;

    x -= cx;
    y -= cy;

    if (x < 0) x = (int)(x * dXmin);
    if (x >= 0) x = (int)(x * dXmax);

    if (y < 0) y = (int)(y * dYmin);
    if (y >= 0) y = (int)(y * dYmax);

    x = (short)(x * scaleFactor);
    y = (short)(y * scaleFactor);

    if (x >= mTo2) x = mTo2 - 1;
    if (x < -mTo2) x = -mTo2;

    if (y >= mTo2) y = mTo2 - 1;
    if (y < -mTo2) y = -mTo2;

    *ax = x;
    *ay = y;
}

void setup() 
{
    memset(&usb_report, 0, sizeof(usb_report));
    memset(&usb_params_report, 0, sizeof(usb_params_report));
    
    io_init();
    pdev = USB_Init(&USBD_HID_Class);
    
    leds_init();
    #if DEBUG_LVL >= 1
    Serial.begin(115200);
    #endif
    bool settings_loaded = settings_init();
    if (!settings_loaded)
    {
        leds_anim(B1010, B0101, 250, 2000);
        leds_set(0);
    }


    bool sinit = sensors_init();
    if (!sinit) {
        leds_anim(B1110, B0000, 250, 1000);
        leds_set(0);
    }
    

    digitalWrite(EXT1, settings.uart_adapter_enabled);
    if (settings.uart_adapter_enabled)
        Serial.begin(921600);
}

float scale(float v, float vmax, float max)
{
    return v * max/vmax;
}

void update()
{
    uint32_t buttons = 0;

    buttons |= !digitalRead(FORK) << 0;
    buttons |= !digitalRead(CIRCLE) << 1;
    buttons |= !digitalRead(SQUARE) << 2;
    buttons |= !digitalRead(TRIANGLE) << 3;
    buttons |= !digitalRead(LEFT_TUP) << 4;
    buttons |= !digitalRead(RIGHT_TUP) << 5;
    buttons |= !digitalRead(SELECT) << 6;
    buttons |= !digitalRead(START) << 7;
    buttons |= !digitalRead(UP) << 8;
    buttons |= !digitalRead(LEFT) << 9;
    buttons |= !digitalRead(DOWN) << 10;
    buttons |= !digitalRead(RIGHT) << 11;
    buttons |= !digitalRead(JSW_LEFT) << 12;
    buttons |= !digitalRead(JSW_RIGHT) << 13;
    buttons |= !digitalRead(MENU) << 14;
    buttons |= !digitalRead(LEFT_TDOWN) << 15;
    buttons |= !digitalRead(RIGHT_TDOWN) << 16;

    int x_l = analogRead(JADC_X_LEFT);
    int y_l = AXIS_MAX_VALUE - analogRead(JADC_Y_LEFT);
    int x_r = analogRead(JADC_X_RIGHT);
    int y_r = AXIS_MAX_VALUE - analogRead(JADC_Y_RIGHT);

    usb_report.data.buttons = buttons;
    usb_report.data.axis_data[0] = x_l;
    usb_report.data.axis_data[1] = y_l;
    usb_report.data.axis_data[2] = x_r;
    usb_report.data.axis_data[3] = y_r;

    if (itg.isInitialized())
        itg.getRotation(&usb_report.data.gyro_data[0], &usb_report.data.gyro_data[1], &usb_report.data.gyro_data[2]);
    if (mag.isInitialized())
        mag.getScaled(&usb_report.data.mag_data[0], &usb_report.data.mag_data[1], &usb_report.data.mag_data[2]);
    if (acc.isInitialized())
        acc.getAcceleration(&usb_report.data.accel_data[0], &usb_report.data.accel_data[1], &usb_report.data.accel_data[2]);
    
    if (settings.calib_enabled)
    {
        calib_apply(&settings.calib_left, &usb_report.data.axis_data[0], &usb_report.data.axis_data[1]);
        calib_apply(&settings.calib_right, &usb_report.data.axis_data[2], &usb_report.data.axis_data[3]);
    }

    data_checksum = get_CRC32((uint8_t*)&usb_report.data, sizeof(joy_data_t));
    #if DEBUG_LVL >= 1
    Serial.printf("L=%4d:%4d  R=%4d:%4d  M=%4d:%4d:%4d  A=%4d:%4d:%4d  G=%4d:%4d:%4d  BTN=%08X  sleep_timeout=%d\n\r", 
        usb_report.data.axis_data[0], usb_report.data.axis_data[1], usb_report.data.axis_data[2], usb_report.data.axis_data[3], 
        usb_report.data.mag_data[0], usb_report.data.mag_data[1], usb_report.data.mag_data[2],
        usb_report.data.accel_data[0], usb_report.data.accel_data[1], usb_report.data.accel_data[2],
        usb_report.data.gyro_data[0], usb_report.data.gyro_data[1], usb_report.data.gyro_data[2],
        usb_report.data.buttons, 
        (millis()-sleep_pressed_last_time));
    #endif
}

void report_usb()
{
    if (last_usb_checksum != data_checksum)
    {
        usb_report.id = REPORT_ID_JOY;
        HID_Device_SendReport(pdev, (uint8_t*)&usb_report, sizeof(usb_report));
        last_usb_checksum = data_checksum;
    }
}

void report_uart()
{
    if (last_uart_checksum != data_checksum)
    {
        uart_report.header = 0x413242aa; //0xAA'B2A'
        uart_report.data_type = 0x1; //
        uart_report.data_size = sizeof(joy_data_t);
        memcpy(&uart_report.data, &usb_report.data, uart_report.data_size);
        uart_report.crc = data_checksum;
        Serial.write((uint8_t*)&uart_report, sizeof(uart_report_t));
        Serial.flush();
        last_uart_checksum = data_checksum;
    }
}

void loop() {
    uint32_t time = millis();
    usb_con_state = pdev->dev_state == USBD_STATE_CONFIGURED;
    adapter_con_state = digitalRead(EXT2);

    int shutdown_pressed = !digitalRead(SELECT);
    int chrg = !digitalRead(CHRG);
    digitalWrite(LED1, usb_con_state);
    digitalWrite(LED2, adapter_con_state);

    int timeout = (time-charging_blink_last_time);
    if (chrg)
    {
        if (timeout >= CHAGING_BLINK_TIMEOUT)
        {
            digitalToggle(LED4);
            charging_blink_last_time = time;
        }
    }
    else
         digitalWrite(LED4, 0);

    if (shutdown_pressed) {
        int timeout = (time-sleep_pressed_last_time);
        if (timeout >= SLEEP_TIMEOUT)
        {
            pinMode(PWR_CTRL, INPUT_FLOATING);
        }
    }
    else {
        sleep_pressed_last_time = time;
    }
    
    if ((time-last_usb_report_time) >= USB_REPORT_POOL_INTERVAL)
    {
        report_usb();
        last_usb_report_time = time;
    }

    if (((time-last_uart_report_time) >= UART_REPORT_POOL_INTERVAL) && settings.uart_adapter_enabled)
    {
        report_uart();
        last_uart_report_time = time;
    }

    if ((time-last_update_time) >= 5)
    {
        update();
        last_update_time = time;
    }

    if (params_buf_len)
    {
        #if DEBUG_LVL >= 1
        Serial.printf("PARAMS [%d]:\n\r", params_buf_len);
        for (int i=0;i<params_buf_len;i++)
        {
            Serial.printf("%02X ", params_buf[i]);
        }
        Serial.println();
        #endif
        uint32_t crc = get_CRC32(params_buf, params_buf_len);
        eeprom_write_data(0, &crc, sizeof(uint32_t));
        eeprom_write_data(4, params_buf, params_buf_len);
        params_buf_len = 0;
        NVIC_SystemReset();
    }
}

void HID_Device_ReceiveReport(uint8_t epnum, uint8_t *data)
{
    #if DEBUG_LVL >= 3
    Serial.printf("%02X %02X\n\r", data[0], data[1]);
    #endif
    if (data[0] == REPORT_ID_PARAM)
    {
        int len = data[1];
        if (len == sizeof(settings_t)) {
            memcpy(params_buf, data+2, len);
            params_buf_len = len;
        }
    }
    HID_Device_PrepareReceive(pdev, epnum);
}

bool HID_Device_GetReport(uint8_t id)
{
    #if DEBUG_LVL >= 3
    Serial.printf("HID_Device_GetReport id: %02X\n\r", id);
    #endif
    switch(id)
    {
        case REPORT_ID_JOY:
        {
            usb_report.id = REPORT_ID_JOY;
            HID_Device_SendCtrlReport(pdev, (uint8_t*)&usb_report, sizeof(usb_report));
        }
        break;
        case REPORT_ID_PARAM:
        {
            usb_params_report.id = REPORT_ID_PARAM;
            memcpy(&usb_params_report.data, &settings, sizeof(settings_t));
            HID_Device_SendCtrlReport(pdev, (uint8_t*)&usb_params_report, sizeof(usb_params_report));
        }
        break;
        default:
            return false;
    }
    return true;
}