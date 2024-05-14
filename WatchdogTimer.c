#include "WatchdogTimer.h"
#include "Settings.h"

#ifdef PLATFORM_ESP32
#include <esp_task_wdt.h>
#endif

#ifdef PLATFORM_STM32
#include <IWatchdog.h>

uint32_t calculateReloadValue(float timeoutSec, uint32_t prescaler, uint32_t lsiFreq) {
    // Calculate the reload value based on the formula provided
    float reloadValue = (timeoutSec * lsiFreq / prescaler) - 1;

    // Make sure the reload value does not exceed the maximum allowed value (0xFFF or 4095 for STM32)
    if (reloadValue > 0xFFF) {
        reloadValue = 0xFFF;
    }

    // Ensure the reload value does not go below the minimum value that can be handled
    if (reloadValue < 0) {
        reloadValue = 0;
    }

    return static_cast<uint32_t>(reloadValue);
}
#endif

void setupWatchdog(int timeoutSec = 10) {

#ifdef PLATFORM_ESP32
  esp_task_wdt_init(timeoutSec, true);
  esp_task_wdt_add(NULL);
#endif

#ifdef PLATFORM_STM32
  uint32_t reloadValue = calculateReloadValue(timeoutSec, IWDG_PRESCALER_256, LSI_FREQ);
  IWatchdog.begin(IWDG_PRESCALER_256, reloadValue);
#endif

}

void petWatchdog(void) {

#ifdef PLATFORM_ESP32
  esp_task_wdt_reset();
#endif

#ifdef PLATFORM_STM32
  IWatchdog.reload();
#endif

}

void deleteWatchdog(void) {

#ifdef PLATFORM_ESP32
  esp_task_wdt_delete(NULL);
#endif

}