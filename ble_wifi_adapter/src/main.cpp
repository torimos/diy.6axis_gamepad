#include "common.h"

BleHid ble(DEVICE_NAME, (uint8_t*)REPORT_MAP, sizeof(REPORT_MAP));
report_t report;
uint32_t last_report_time = 0;
uint32_t last_report_crc = 0;

#if DEBUG_LVL >= 1
uint32_t last_timer_time = 0;
int err_cnt = 0;
int success_cnt = 0;
int ble_reports_cnt = 0;
#endif

void setup() {
  Serial.begin(115200);
  stm32_uart_init(&report);
  pinMode(EXT1, INPUT);
  pinMode(EXT2, OUTPUT);
  ble.begin();
}

void loop() {
  uint32_t time = millis();
  bool bleCon = ble.isConnected();
  int enabled = digitalRead(EXT1);
  if (enabled)
  {
    #if DEBUG_LVL >= 1
    int r = stm32_uart_process();
    if (r==1) success_cnt++;
    if (r<0) err_cnt++;
    #else 
    stm32_uart_process();
    #endif
    if ((time-last_report_time) >= REPORT_POOL_INTERVAL)
    {
      digitalWrite(EXT2, bleCon);
      #if DEBUG_LVL >= 2
      Serial.printf("BTN=%08X  L=%4d:%4d  R=%4d:%4d  M=%4d:%4d:%4d  A=%4d:%4d:%4d  G=%4d:%4d:%4d\n\r",
        report.data.buttons,
        report.data.axis_data[0], report.data.axis_data[1], report.data.axis_data[2], report.data.axis_data[3], 
        report.sens.mag_data[0], report.sens.mag_data[1], report.sens.mag_data[2],
        report.sens.accel_data[0], report.sens.accel_data[1], report.sens.accel_data[2],
        report.sens.gyro_data[0], report.sens.gyro_data[1], report.sens.gyro_data[2]);
      #endif
      if (last_report_crc != report.crc)
      {
        if (bleCon) {
          ble.notify(0, (uint8_t*)&report.data, sizeof(joy_data_t));
          ble.notify(1, (uint8_t*)&report.sens, sizeof(sens_data_t));
          #if DEBUG_LVL >= 1
          ble_reports_cnt++;
          #endif
        }
        last_report_crc = report.crc;
        last_report_time = time;
      }
    }
    
    #if DEBUG_LVL >= 1
    if ((time-last_timer_time) >= 1000)
    {
      Serial.printf("[%d]: RPS: %d BRS: %d\n\r", time, success_cnt, ble_reports_cnt);
      success_cnt = 0;
      err_cnt = 0;
      ble_reports_cnt = 0;
      last_timer_time = time;
    }
    #endif
  }
  else
  {
    delay(1000);
    esp_restart();
  }
}