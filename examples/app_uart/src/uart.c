/**
 * ,---------,       ____  _ __
 * |  ,-^-,  |      / __ )(_) /_______________ _____  ___
 * | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * | / ,--´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *    +------`   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2019 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * hello_world.c - App layer application of a simple hello world debug print every
 *   2 seconds.
 */


#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "app.h"

#include "FreeRTOS.h"
#include "task.h"

#define DEBUG_MODULE "HELLOWORLD"
#include "debug.h"
#include "uart2.h"
#include "log.h"
#include "param.h"

#define ARRAY_SIZE 6

void appMain() {
  DEBUG_PRINT("Waiting for activation ...\n");
  //uint8_t it = 0;
  vTaskDelay(M2T(2000));
  uart2Init(9600);
  uint8_t start_byte = 0xAA;
  uint8_t end_byte = 0x55;
  uint8_t data[ARRAY_SIZE * sizeof(float) + 2]; // Extra space for start and end bytes
  float array[ARRAY_SIZE] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  logVarId_t idx = logGetVarId("stateEstimate", "x");
  logVarId_t idy = logGetVarId("stateEstimate", "y");
  logVarId_t idz = logGetVarId("stateEstimate", "z");
  logVarId_t idyaw = logGetVarId("stateEstimate", "yaw");

  logVarId_t idctrlpitch = logGetVarId("controller", "pitch");
  logVarId_t idctryawrate = logGetVarId("controller", "roll");



  while(1) {
   vTaskDelay(M2T(100));

    array[0]= logGetFloat(idx);
    array[1]= logGetFloat(idy);
    array[2]= logGetFloat(idz);
    array[3]= logGetFloat(idyaw);

    float ctrl_forward = logGetFloat(idctrlpitch);
    float ctrl_yaw = logGetFloat(idctryawrate);

    array[4] = ctrl_forward;
    array[5] = ctrl_yaw;




    data[0] = start_byte;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        memcpy(&data[i * sizeof(float) + 1], &array[i], sizeof(float)); // Offset by 1 for start byte
    }
    data[sizeof(data) - 1] = end_byte;
    for (int i = 0; i < sizeof(data); i++) {
        uart2Putchar(data[i]);
    }


    //DEBUG_PRINT("%d\n",it);
    //it++;
  }
}
