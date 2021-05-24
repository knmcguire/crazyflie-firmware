#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "app.h"

#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

#include "radiolink.h"
#include "configblock.h"
#include "stabilizer_types.h"


#define DEBUG_MODULE "OOT_ESTIMATOR"
#include "debug.h"

#include "param.h"
#include "semphr.h"
#include "system.h"
#include "static_mem.h"


// Semaphore to signal that we got data from the stabilzer loop to process
static SemaphoreHandle_t runTaskSemaphore;

// Mutex to protect data that is shared between the task and
// functions called by the stabilizer loop
static SemaphoreHandle_t dataMutex;
static StaticSemaphore_t dataMutexBuffer;
static state_t taskEstimatorState = {0}; // The estimator state produced by the task, copied to the stabilzer when needed.

static bool isInit = false;

void ootEstimatorTask(void* parameters);

void appInit()
{
    DEBUG_PRINT("Initalizing Error Kalman Filter\n");
    ootEstimatorTaskInit();
}

void estimatorOutOfTree(state_t *state, const uint32_t tick)
{
  // This function is called from the stabilizer loop. It is important that this call returns
  // as quickly as possible. The dataMutex must only be locked short periods by the task.
  xSemaphoreTake(dataMutex, portMAX_DELAY);

  // Copy the latest state, calculated by the task
  memcpy(state, &taskEstimatorState, sizeof(state_t));
  xSemaphoreGive(dataMutex);

  xSemaphoreGive(runTaskSemaphore);
}

void ootEstimatorTaskInit() {
  vSemaphoreCreateBinary(runTaskSemaphore);

  dataMutex = xSemaphoreCreateMutexStatic(&dataMutexBuffer);

  STATIC_MEM_TASK_CREATE(ootEstimatorTask, ootEstimatorTask, "OOT_ESTIMATOR", NULL, OOT_ESTIMATOR);

  isInit = true;
}

bool estimatorOutOfTreeTest(void)
{
  return isInit;
}

void estimatorOutOfTreeInit(void)
{

}

static void ootEstimatorTask(void* parameters) 
{
    systemWaitStart();

    while (true) {
    xSemaphoreTake(runTaskSemaphore, portMAX_DELAY);

    taskEstimatorState.position.x = 12;
    taskEstimatorState.position.y = 12;
    taskEstimatorState.position.z = 12;

    xSemaphoreGive(dataMutex);
    }


}

