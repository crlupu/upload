/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include "tock.h"
#include <temperature.h>
#include <led.h>

static int temperature = 0;
static int numbers[10][11] = {
  { 10, 0, 1, 5, 6, 10, 11, 15, 16, 20, 21 },
  { 5, 1, 6, 11, 16, 21 },
  { 8, 0, 1, 6, 10, 11, 15, 20, 21 },
  { 8, 0, 1, 6, 10, 11, 16, 20, 21 },
  { 8, 0, 1, 5, 6, 10, 11, 16, 21 },
  { 8, 0, 1, 5, 10, 11, 16, 20, 21 },
  { 9, 0, 1, 5, 10, 11, 15, 16, 20, 21 },
  { 6, 0, 1, 6, 11, 16, 21 },
  { 8, 0, 1, 5, 6, 15, 16, 20, 21 },
  { 9, 0, 1, 5, 6, 10, 11, 16, 20, 21 },
};

static void show_number(int number, int offset) {
  for (int i = 1; i <= numbers[number][0]; i++) {
    led_on(numbers[number][i] + offset);
  }
}

static void show_temperature() {
  for(int i = 0; i < 25; i++) {
    led_off(i);
  }
  if (temperature < 10) {
    show_number(temperature, 3);
  } else {
    show_number(temperature / 10, 0);
    show_number(temperature % 10, 3);    
  }
}

static void button_callback(int btn_num,
                            int val,
                            int arg2 __attribute__ ((unused)),
                            void *user_data __attribute__ ((unused)) ) {
  

  if (val == 1) {
    if (btn_num == 0 && temperature > 0) temperature--;
    else if (temperature < 99) temperature++;
    show_temperature();
  }
}

int main(void) {
  
  show_temperature();
  button_enable_interrupt(0);
  button_enable_interrupt(1);

  button_subscribe(button_callback, NULL);
  
  return 0;
}
