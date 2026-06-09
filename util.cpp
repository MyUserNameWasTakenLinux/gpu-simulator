#include "util.h"
#include "sys/socket.h"
#include "arpa/inet.h"

ssize_t
send_n(int fd, const void *buf, size_t len)
{
   size_t sent = 0;
   while (sent < len) {
      size_t n = send(fd, (uint8_t *) buf + sent, len - sent, 0);
      if (n <= 0)
         return -1;
      sent += n;
   }
   return sent;
}

ssize_t
recv_n(int fd, void *buf, size_t len)
{
   size_t recvd = 0;
   while (recvd < len) {
      ssize_t n = recv(fd, (uint8_t *) buf + recvd, len - recvd, 0);
      if (n <= 0)
         return -1;
      recvd += n;
   }
   return recvd;
}
