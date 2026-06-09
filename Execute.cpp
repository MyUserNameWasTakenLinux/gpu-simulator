#include "Execute.h"
#include <cstdlib>
#include <iostream>

void Execute::set_descriptor(uint32_t descriptor_index, void *handle)
{
  if (descriptor_index > NUM_DESCRIPTORS - 1) {
    std::cerr << "Invalid descriptor index\n";
    exit(EXIT_FAILURE);
  }

  descriptors[descriptor_index] = handle;
}
