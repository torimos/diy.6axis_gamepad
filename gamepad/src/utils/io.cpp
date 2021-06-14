#include "main.h"

ITG3200 itg = ITG3200();
HMC5883L mag = HMC5883L();
ADXL345 acc = ADXL345();

bool sensors_init()
{
    /* Initialise the sensor */
    if(!mag.begin())
    {
        #if DEBUG_LVL > 3
        Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
        #endif
        return false;
    }
    if(!itg.begin())
    {
        #if DEBUG_LVL > 3
        Serial.println("Ooops, no ITG3205 detected ... Check your wiring!");
        #endif
        return false;
    }
    if(!acc.begin())
    {
        #if DEBUG_LVL > 3
        Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
        #endif
        return false;
    }
    return true;
}

void io_init()
{
    //set to low to on, floating to off
    pinMode(PWR_CTRL, OUTPUT);

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
    pinMode(EXT2, INPUT_PULLDOWN);
}