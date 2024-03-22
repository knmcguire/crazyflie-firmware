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


void appMain() {
  DEBUG_PRINT("Waiting for activation ...\n");
  uint8_t it = 0;
  vTaskDelay(M2T(2000));
  uart2Init(9600);
  uint8_t start_byte = 0xAA;
  uint8_t end_byte = 0x55;
  uint8_t data[4 * sizeof(float) + 2]; // Extra space for start and end bytes
  float array[4] = {1.0, 2.0, 3.0, 4.0};

  logVarId_t idpitch = logGetVarId("controller", "pitch");
  logVarId_t idYaw = logGetVarId("controller", "roll");


  while(1) {
   vTaskDelay(M2T(100));

    array[0]= logGetFloat(idYaw);
    array[1]= logGetFloat(idpitch);


    data[0] = start_byte;
    for (int i = 0; i < 4; i++) {
        memcpy(&data[i * sizeof(float) + 1], &array[i], sizeof(float)); // Offset by 1 for start byte
    }
    data[sizeof(data) - 1] = end_byte;
    for (int i = 0; i < sizeof(data); i++) {
        uart2Putchar(data[i]);
    }


    DEBUG_PRINT("%d\n",it);
    it++;
  }
}
