#ifndef __UTILS__
#define __UTILS__

typedef struct {
  int msgId;
  char* msg;
} mappedMsg_t;

enum results {
  RESULT_SUCCESS = 1,
  RESULT_FAILURE = -1
};

typedef unsigned int ADI_REG_TYPE;

enum States {
  STATE_DSP_INIT_DONE = 0,
  STATE_DISCOVERY_NOT_DONE = 1,
  STATE_DISCOVERY_DONE = 2,
  STATE_AMP_INIT_DONE = 3,
  STATE_MIC_INIT_DONE = 4,
  STATE_SLAVE_INIT_DONE = 5,
  STATE_A2B_LINE_FAULT_BEFORE_DSCVRY = 6,
  STATE_A2B_LINE_FAULT_POST_DSCVRY = 7,
  STATE_DSP_NEEDS_REINIT = 8,
  STATE_A2B_NEEDS_REDISCOVERY = 9
};

int currentState = STATE_DSP_NEEDS_REINIT;

#endif