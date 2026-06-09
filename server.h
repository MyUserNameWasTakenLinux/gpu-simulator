#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "Execute.h"

class Listener {
public:
  int server_fd;
  int client_fd;
  void start_server(); 
  void wait_on_client();
  void loop(Execute &cpu);
  void cleanup();
private:
  void handle_msg_type_allocate_memory();
  void handle_msg_type_free_memory();
  void handle_msg_type_update_descriptor(Execute &cpu);
  void handle_msg_type_copy_buffer_send(uint32_t size);
  void handle_msg_type_copy_buffer_receive();
};


#endif
