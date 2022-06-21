#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "networking.h"

struct message receive_message(const int from_socket_number) {
    struct message message;
    char message_text[MAX_MESSAGE_SIZE];
    long message_size = -1;

    struct sockaddr_in client_address;
    int client_address_size = sizeof(client_address);
    memset(&client_address, 0, client_address_size);

    while (message_size == -1) {
        message_size = recvfrom(
                from_socket_number,
                message_text,
                MAX_MESSAGE_SIZE,
                0,
                (struct sockaddr *) &client_address,
                &client_address_size
        );
    }
    message_text[message_size] = '\0';

    memcpy(message.text, message_text, strlen(message_text));
    message.origin = client_address;

    return message;
}

int bind_socket_to_address(const int socket_number, const struct sockaddr_in address) {
    if (bind(socket_number, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind");
        return 0;
    }
    return 1;
}

int new_socket_number() {
    int socket_number = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_number < 0) {
        perror("socket");
        exit(-1);
    }

    return socket_number;
}

int bind_socket_to_port(const int socket_number, const int port) {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    return bind_socket_to_address(socket_number, server_address);
}

void send_message(const char *message_text, const int from_socket_number, const struct sockaddr_in destination) {
    sendto(
            from_socket_number,
            (const char *) message_text,
            strlen(message_text),
            0,
            (const struct sockaddr *) &destination,
            sizeof(destination)
    );
}

struct sockaddr_in new_address(const char *ip, const int port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    return address;
}
