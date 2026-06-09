#include "server.h"
#include "util.h"
#include "message.h"
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

void Listener::start_server() {
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Socket failed\n";
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    std::cerr << "Set socket option failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(7777);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    std::cerr << "Bind failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 1) < 0) {
    std::cerr << "Listen failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

void Listener::wait_on_client() {
  std::cout << "Waiting for connection on port 7777\n";

  client_fd = accept(server_fd, nullptr, nullptr);
  if (client_fd < 0) {
    std::cerr << "Accept failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "Client has connected\n";
}

void Listener::loop(Execute &cpu) {
  for(;;) {
    tanium_msg_header header;
    if (recv_n(client_fd, (void *) &header, sizeof(header)) != sizeof(header)) {
      std::cerr << "Failed to receive\n";
      exit(EXIT_FAILURE);
    }
    switch(header.type) {
      case TANIUM_MSG_ALLOCATE_MEMORY:
        std::cout << "TANIUM_MSG_ALLOCATE_MEMORY\n";
        handle_msg_type_allocate_memory();
        break;
      case TANIUM_MSG_FREE_MEMORY:
        std::cout << "TANIUM_MSG_FREE_MEMORY\n";
        handle_msg_type_free_memory();
        break;
      case TANIUM_MSG_UPDATE_DESCRIPTOR:
        std::cout << "TANIUM_MSG_UPDATE_DESCRIPTOR\n";
        handle_msg_type_update_descriptor(cpu);
        break;
      case TANIUM_MSG_COPY_BUFFER_SEND:
        std::cout << "TANIUM_MSG_COPY_BUFFER_SEND\n";
        handle_msg_type_copy_buffer_send(header.length - sizeof(tanium_msg_copy_buffer_send));
        break;
      case TANIUM_MSG_COPY_BUFFER_RECEIVE:
        std::cout << "TANIUM_MSG_COPY_BUFFER_RECEIVE\n";
        handle_msg_type_copy_buffer_receive();
        break;
    }
  }
}

void Listener::handle_msg_type_copy_buffer_send(uint32_t size) {
  tanium_msg_copy_buffer_send msg = {};
  if (recv_n(client_fd, (void *) &msg, sizeof(msg)) != sizeof(msg)) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }
  uint8_t *_handle = (uint8_t *) msg.handle + msg.offset;
  if (recv_n(client_fd, (void *) _handle, size) != size) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }

  struct {
    tanium_resp_header header;
  } resp = {
    .header = {TANIUM_RESP_NONE, 0}
  };
  if (send_n(client_fd, (void *) &resp, sizeof(resp)) != sizeof(resp)) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }
}

void Listener::handle_msg_type_copy_buffer_receive() {
  tanium_msg_copy_buffer_receive msg = {};
  if (recv_n(client_fd, &msg, sizeof(msg)) != sizeof(msg)) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }

  uint8_t *_handle = (uint8_t *) msg.handle + msg.offset;

  struct {
    tanium_resp_header header;
  } resp = {
    .header = {TANIUM_RESP_NONE, msg.size}
  };
  if (send_n(client_fd, (void *) &resp, sizeof(resp)) != sizeof(resp)) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }

  if (send_n(client_fd, (void *) _handle, msg.size) != msg.size) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }
}

void Listener::handle_msg_type_allocate_memory() {
  tanium_msg_allocate_memory msg = {};
  if (recv_n(client_fd, (void *) &msg, sizeof(msg)) != sizeof(msg)) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }
  void *handle = malloc(msg.size);
  struct {
    tanium_resp_header header;
    tanium_resp_allocate_memory data;
  } resp = {
    .header = {TANIUM_RESP_ALLOCATE_MEMORY, sizeof(tanium_resp_allocate_memory)},
    .data = {.handle = handle}
  };

  if (send_n(client_fd, (void *) &resp, sizeof(resp)) != sizeof(resp)) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }
}

void Listener::handle_msg_type_free_memory() {
  tanium_msg_free_memory msg = {};
  if (recv_n(client_fd, (void *) &msg, sizeof(msg)) != sizeof(msg)) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }
  free(msg.handle);
  struct {
    tanium_resp_header header;
  } resp = {
    .header = {TANIUM_RESP_NONE, 0},
  };

  if (send_n(client_fd, (void *) &resp, sizeof(resp)) != sizeof(resp)) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }

}

void Listener::handle_msg_type_update_descriptor(Execute &cpu) {
  tanium_msg_update_descriptor msg = {};
  if (recv_n(client_fd, (void *) &msg, sizeof(msg)) != sizeof(msg)) {
    std::cerr << "Failed to receive\n";
    exit(EXIT_FAILURE);
  }

  cpu.set_descriptor(msg.descriptor_index, msg.handle);
  std:: cout << "Descriptor Set: " << msg.handle << "\n";

  struct {
    tanium_resp_header header;
  } resp = {
    .header = {TANIUM_RESP_NONE, 0},
  };

  if (send_n(client_fd, (void *) &resp, sizeof(resp)) != sizeof(resp)) {
    std::cerr << "Failed to send\n";
    exit(EXIT_FAILURE);
  }

}

void Listener::cleanup() {
  close(client_fd);
  close(server_fd);
}
