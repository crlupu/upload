/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "tock.h"
#include "network.h"
#include <temperature.h>
#include <timer.h>
#include <led.h>
#include <buzzer.h>

static tock_timer_t timer;
static bool temperature               = false;
static int temperature_reading        = 0;
static int temperature_refference     = 0;

static void timer_fired(__attribute__ ((unused)) int arg0,
                        __attribute__ ((unused)) int arg1,
                        __attribute__ ((unused)) int arg2,
                        __attribute__ ((unused)) void* ud) {
  if (temperature)    temperature_read_sync(&temperature_reading);
  if (temperature)    printf("Temperature:                 : %d deg C\n", temperature_reading/100);
  if (temperature_refference != temperature_reading / 100) {
    char temperature_converted[10];
    sprintf(temperature_converted, "%d", temperature_reading / 100);
    network_post("http://localhost:8000/temperature/", temperature_converted);
    tone_sync (5000, 5000);
  }
  timer_in(10000, timer_fired, NULL, &timer);
}

static void button_callback(int btn_num,
                            int val,
                            int arg2 __attribute__ ((unused)),
                            void *user_data __attribute__ ((unused)) ) {
  
  if (val == 0) {
    if (btn_num == 0 && temperature_refference > 0) temperature_refference--;
    else if (temperature_refference < 50) temperature_refference++;
    printf("Refference temperature set to: %d deg C\n", temperature_refference);
  }
}

int main(void) {

button_enable_interrupt(0);
button_enable_interrupt(1);

button_subscribe(button_callback, NULL);

  temperature    = driver_exists(DRIVER_NUM_TEMPERATURE);
  if (temperature)    printf("[Sensors]   Sampling Temperature sensor.\n");
  timer_in(1000, timer_fired, NULL, &timer);

  return 0;
}
