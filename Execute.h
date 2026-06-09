#ifndef EXECUTE_H
#define EXECUTE_H

#include <inttypes.h>

#define NUM_DESCRIPTORS 16

class Execute {
private:
  void *descriptors[NUM_DESCRIPTORS];
public:
  void set_descriptor(uint32_t index, void *handle);
};


#endif
