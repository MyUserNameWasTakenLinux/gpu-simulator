#include <iostream>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "server.h"

int main(void) {
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  Execute cpu;
  
  Listener s;
  s.start_server();
  s.wait_on_client();
  s.loop(cpu);
  s.cleanup();

  return 0;
}
