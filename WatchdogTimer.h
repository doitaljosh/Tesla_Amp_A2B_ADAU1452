#ifndef __WATCHDOG_TIMER__
#define __WATCHDOG_TIMER__

void setupWatchdog(int timeoutSec);
void petWatchdog(void);
void deleteWatchdog(void);

#endif