/*
 * UI_Status_fix
 * 
 * v1.0 Nov 2020
 * Dimitar Stoimenov 
 * 
 * For use with ESP-32 CAN-Bus board
 * http://skpang.co.uk/catalog/esp32-canbus-board-p-1586.html
 * 
 */
#include <ESP32CAN.h>
CAN_device_t CAN_cfg;
CAN_frame_t rx_frame;

void setup()
{
    Serial.begin(115200);
    Serial.println("UI status fix");
    CAN_cfg.speed=CAN_SPEED_500KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_25;
    CAN_cfg.rx_pin_id = GPIO_NUM_26;
    CAN_cfg.rx_queue = xQueueCreate(100,sizeof(CAN_frame_t));
    ESP32Can.CANInit();
}

void loop()
{ 
  if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE)
  {
    if(rx_frame.FIR.B.RTR==CAN_no_RTR && rx_frame.MsgID == 0x353)
    {
      // Resend message 353 with the same data as message 00C
      rx_frame.MsgID = 0x00C;
      ESP32Can.CANWriteFrame(&rx_frame);
    }
  }
  else
  {
    delay(10);    
  }
}
