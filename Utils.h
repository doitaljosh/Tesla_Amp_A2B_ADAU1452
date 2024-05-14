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

#endif