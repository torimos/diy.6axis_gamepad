#include <Arduino.h>
#include <usb_hid.h>
#include "config.h"
#include "utils.h"

#include <gyro.h>
#include <magn.h>
#include <accel.h>

USBD_HandleTypeDef* pdev = NULL;
#pragma pack(push, 1)
typedef struct
{
    uint16_t buttons;
    uint16_t triggers;
    int16_t axis_data[4];
    int16_t mag_data[3];
    int16_t accel_data[3];
    int16_t gyro_data[3];
} joy_data_t;
typedef struct
{
    uint8_t id;
    joy_data_t data;
} usb_report_t;
typedef struct
{
    uint32_t header;
    uint16_t data_type;
    uint16_t data_size;
    joy_data_t data;
    uint32_t crc;
} uart_report_t;
#pragma pack(pop)

uart_report_t uart_report;
usb_report_t usb_report;

uint32_t last_usb_report_time = 0;
uint32_t last_uart_report_time = 0;
uint32_t last_update_time = 0;
uint32_t sleep_pressed_last_time = 0;
bool con_state = false;
uint32_t data_checksum = 0;
uint32_t last_usb_checksum = 0;
uint32_t last_uart_checksum = 0;

ITG3200 itg = ITG3200();
HMC5883L mag = HMC5883L();
ADXL345 acc = ADXL345();


stick_calib_t calib_left, calib_right;

void io_init()
{
    //set to low to on, floating to off
    pinMode(PWR_CTRL, OUTPUT);

    // leds
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    // leds test
    int led_delay = 300;
    digitalWrite(LED1, 1);delay(led_delay);
    digitalWrite(LED2, 1);delay(led_delay);
    digitalWrite(LED3, 1);delay(led_delay);
    digitalWrite(LED4, 1);delay(led_delay);
    //digitalWrite(LED1, 0);delay(led_delay >> 1);
    digitalWrite(LED2, 0);delay(led_delay >> 1);
    digitalWrite(LED3, 0);delay(led_delay >> 1);
    digitalWrite(LED4, 0);delay(led_delay >> 1);

#ifdef MOTORS_ENABLED
    // motors
    pinMode(PB8, OUTPUT);
    // motor test  
    digitalWrite(MOTOR1, 0);delay(100);digitalWrite(MOTOR1, 1);pinMode(MOTOR1, INPUT_FLOATING);delay(500);

    pinMode(PB9, OUTPUT);
    digitalWrite(MOTOR2, 0);delay(100);digitalWrite(MOTOR2, 1);pinMode(MOTOR2, INPUT_FLOATING);delay(200);
#endif

    // input
    pinMode(JADC_X_LEFT, INPUT_ANALOG);
    pinMode(JADC_Y_LEFT, INPUT_ANALOG);
    pinMode(JADC_X_RIGHT, INPUT_ANALOG);
    pinMode(JADC_Y_RIGHT, INPUT_ANALOG);
    pinMode(JSW_LEFT, INPUT_PULLUP);
    pinMode(JSW_RIGHT, INPUT_PULLUP);
    pinMode(MENU, INPUT_FLOATING);
    pinMode(SELECT, INPUT_PULLUP);
    pinMode(START, INPUT_PULLUP);
    pinMode(CHRG, INPUT);
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(LEFT_TUP, INPUT_PULLUP);
    pinMode(LEFT_TDOWN, INPUT_PULLUP);
    pinMode(RIGHT_TUP, INPUT_PULLUP);
    pinMode(RIGHT_TDOWN, INPUT_PULLUP);
    pinMode(TRIANGLE, INPUT_PULLUP);
    pinMode(CIRCLE, INPUT_PULLUP);
    pinMode(FORK, INPUT_PULLUP);
    pinMode(SQUARE, INPUT_PULLUP);
    pinMode(EXT1, OUTPUT);
    pinMode(EXT2, OUTPUT);
}

bool sensors_init()
{
    /* Initialise the sensor */
    if(!mag.begin())
    {
        Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
        return false;
    }
    if(!itg.begin())
    {
        Serial.println("Ooops, no ITG3205 detected ... Check your wiring!");
        return false;
    }
    if(!acc.begin())
    {
        Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
        return false;
    }
    return true;
}

void calib_init()
{
    calib_left.xmin = 3999;
    calib_left.xmax = 29919;
    calib_left.ymin = 2207;
    calib_left.ymax = 28991;
    calib_left.xoffs = -160;
    calib_left.yoffs = -352;

    calib_right.xmin = 3775;
    calib_right.xmax = 29119;
    calib_right.ymin = 1823;
    calib_right.ymax = 27967;
    calib_right.xoffs = -1536;
    calib_right.yoffs = -1088;
}

void calib_apply(stick_calib_t* calib, int16_t *ax, int16_t *ay)
{
    int m2 = 32768 / 2;
    int cx = (m2 + calib->xoffs);
    int cy = (m2 + calib->yoffs);

    double dXmin = m2 / (double)(cx - calib->xmin);
    double dXmax = m2 / (double)(calib->xmax - cx);
    double dYmin = m2 / (double)(cy - calib->ymin);
    double dYmax = m2 / (double)(calib->ymax - cy);

    int x = *ax;
    int y = *ay;

    x -= cx;
    y -= cy;

    if (x < 0) x = (int)(x * dXmin);
    if (x >= 0) x = (int)(x * dXmax);


    if (y < 0) y = (int)(y * dYmin);
    if (y >= 0) y = (int)(y * dYmax);

    if (x >= m2) x = m2 - 1;
    if (x < -m2) x = -m2;

    if (y >= m2) y = m2 - 1;
    if (y < -m2) y = -m2;

    *ax = x;
    *ay = y;
}

void setup() {
    Serial.begin(921600);
    memset(&usb_report, 0, sizeof(usb_report));
    pdev = USB_Init(&USBD_HID_Class);
    io_init();
    digitalWrite(LED2, sensors_init());
    digitalWrite(EXT1, 1);// enable uart adapter
    digitalWrite(EXT2, 0);
    calib_init();
}

float scale(float v, float vmax, float max)
{
    return v * max/vmax;
}

void update()
{
    uint16_t buttons = 0;
    uint16_t triggers = 0;

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

    triggers |= (!digitalRead(LEFT_TDOWN))*0xFF;
    triggers |= (!digitalRead(RIGHT_TDOWN))*0xFF << 8;

    int x_l = analogRead(JADC_X_LEFT);
    int y_l = analogRead(JADC_Y_LEFT);
    int x_r = analogRead(JADC_X_RIGHT);
    int y_r = analogRead(JADC_Y_RIGHT);

    usb_report.data.buttons = buttons;
    usb_report.data.triggers = triggers;
    usb_report.data.axis_data[0] = scale(x_l, 1024.0, 32768.0);
    usb_report.data.axis_data[1] = scale(1024-y_l, 1024.0, 32768.0);
    usb_report.data.axis_data[2] = scale(x_r, 1024.0, 32768.0);
    usb_report.data.axis_data[3] = scale(1024-y_r, 1024.0, 32768.0);
    calib_apply(&calib_left, &usb_report.data.axis_data[0], &usb_report.data.axis_data[1]);
    calib_apply(&calib_right, &usb_report.data.axis_data[2], &usb_report.data.axis_data[3]);

    if (itg.isInitialized())
        itg.getRotation(&usb_report.data.gyro_data[0], &usb_report.data.gyro_data[1], &usb_report.data.gyro_data[2]);
    if (mag.isInitialized())
        mag.getScaled(&usb_report.data.mag_data[0], &usb_report.data.mag_data[1], &usb_report.data.mag_data[2]);
    if (acc.isInitialized())
        acc.getAcceleration(&usb_report.data.accel_data[0], &usb_report.data.accel_data[1], &usb_report.data.accel_data[2]);

    data_checksum = get_CRC32((uint8_t*)&usb_report.data, sizeof(joy_data_t));
    // Serial.printf("L=%4d:%4d  R=%4d:%4d  M=%4d:%4d:%4d  A=%4d:%4d:%4d  G=%4d:%4d:%4d  BTN=%02X%02X%02X  sleep_timeout=%d\n\r", 
    //     usb_report.data.axis_data[0], usb_report.data.axis_data[1], usb_report.data.axis_data[2], usb_report.data.axis_data[3], 
    //     usb_report.data.mag_data[0], usb_report.data.mag_data[1], usb_report.data.mag_data[2],
    //     usb_report.data.accel_data[0], usb_report.data.accel_data[1], usb_report.data.accel_data[2],
    //     usb_report.data.gyro_data[0], usb_report.data.gyro_data[1], usb_report.data.gyro_data[2],
    //     usb_report.data.button_data[2],  usb_report.data.button_data[1],  usb_report.data.button_data[0], 
    //     (millis()-sleep_pressed_last_time));
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
    con_state = pdev->dev_state == USBD_STATE_CONFIGURED;

    int shutdown_pressed = !digitalRead(SELECT);
    int chrg = !digitalRead(CHRG);
    digitalWrite(LED4, chrg);

    // int rxc = Serial.read();
    // if (rxc > 0)
    // {
    //      Serial.print(rxc);
    // }

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

     if ((time-last_uart_report_time) >= UART_REPORT_POOL_INTERVAL)
    {
        report_uart();
        last_uart_report_time = time;
    }


    if ((time-last_update_time) >= 5)
    {
        update();
        last_update_time = time;
    }
}

void HID_Device_ReceiveReport(uint8_t epnum, uint8_t *data)
{
    //Serial.printf("HID_DataOut ep: %02X. data: %02X %02X %02X %02X %02X %02X\n\r", epnum, data[0], data[1], data[2], data[3], data[4], data[5]);
    if (data[0] == REPORT_ID_PARAM)
    {
        digitalWrite(LED1, data[1]);
        digitalWrite(LED2, data[2]);
        digitalWrite(LED3, data[3]);
        digitalWrite(LED4, data[4]);
    }
    HID_Device_PrepareReceive(pdev, epnum);
}