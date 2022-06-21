#ifndef CLIENT_NETWORKING_H
#define CLIENT_NETWORKING_H

#define MAX_MESSAGE_SIZE 512

#include <netinet/in.h>

struct message {
    char text[MAX_MESSAGE_SIZE];
    struct sockaddr_in origin;
};

struct message receive_message(int from_socket_number);

int new_socket_number();  // exits the program if none can be assigned
int bind_socket_to_port(int, int);  // returns 0 when the binding fails
struct sockaddr_in new_address(const char *, int);

int bind_socket_to_address(int, struct sockaddr_in);

void send_message(const char *message_text, int from_socket_number, struct sockaddr_in destination);

#endif
