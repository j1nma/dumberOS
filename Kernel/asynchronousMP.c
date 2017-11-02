#include "message.h"
#include "mutex.h"
#include "process.h"
#include "circular_buffer.h"
#include "queue.h"

void disableTaskSwitch() {
  down();
}

void enableTaskSwitch() {
  up();
}