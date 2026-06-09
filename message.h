#ifndef MESSAGE_H
#define MESSAGE_H

#include <inttypes.h>

enum tanium_msg_type {
   TANIUM_MSG_ALLOCATE_MEMORY,
   TANIUM_MSG_FREE_MEMORY,
   TANIUM_MSG_UPDATE_DESCRIPTOR,
   TANIUM_MSG_COPY_BUFFER_SEND,
   TANIUM_MSG_COPY_BUFFER_RECEIVE,
};

struct tanium_msg_header {
   uint64_t type;
   uint64_t length;
};

struct tanium_msg_allocate_memory {
   uint64_t size;
};

struct tanium_msg_free_memory {
   void *handle;
};

struct tanium_msg_update_descriptor {
   uint32_t descriptor_index;
   void *handle;
};

struct tanium_msg_copy_buffer_send {
   void *handle;
   uint64_t offset;
};

struct tanium_msg_copy_buffer_receive {
   void *handle;
   uint64_t offset;
   uint64_t size;
};

enum tanium_resp_type {
   TANIUM_RESP_ALLOCATE_MEMORY,
   TANIUM_RESP_NONE,
};

struct tanium_resp_header {
   uint64_t type;
   uint64_t length;
};

struct tanium_resp_allocate_memory {
   void *handle;
};

#endif
