#include <netinet/in.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "networking.h"
#include "client.h"

const char *SERVER_IP = "127.0.0.1";

void send_result_message(double result, int from_socket_number, struct sockaddr_in destination);

double calculate_result(const char *message_text);

double pi_iteration_result(long);

double pi_interval_result(long start_inclusive, long end_exclusive);

int client(int SERVER_PORT, int id_thread) {
    struct sockaddr_in server_address;
    int socket_number;
    struct message message;

    socket_number = new_socket_number();
    server_address = new_address(SERVER_IP, SERVER_PORT);


    send_message("R", socket_number, server_address);
    message = receive_message(socket_number);
    printf("pid client: %d\ta msg que eu recebi: %s\n",id_thread,  message.text);
    while (0 != strncmp(message.text, "shutdown", 8)) {
        send_result_message(calculate_result(message.text), socket_number, server_address);
        message = receive_message(socket_number);
        printf("pid client: %d\ta msg que eu recebi: %s\n",id_thread,  message.text);
    }
    close(socket_number);
    return 0;
}


void send_result_message(const double result, const int from_socket_number, struct sockaddr_in destination) {
    char message_text[MAX_MESSAGE_SIZE];
    snprintf(message_text, MAX_MESSAGE_SIZE, "%f", result);
    send_message(message_text, from_socket_number, destination);
}

double calculate_result(const char *message_text) {
    double result;
    char *end_of_number;

    long number = strtol(message_text, &end_of_number, 10);
    if ('-' != end_of_number[0]) {
        result = pi_iteration_result(number);
    } else {
        result = pi_interval_result(number, strtol(end_of_number + 1, &end_of_number, 10));
    }

    return result;
}

double pi_interval_result(const long start_inclusive, const long end_exclusive) {
    double result = 0;
    for (long i = start_inclusive; i < end_exclusive; i++) {
        result += pi_iteration_result(i);
    }
    return result;
}

double pi_iteration_result(long number) {
    return (pow(-1, (double) number) / ((2 * (double) number) + 1));
}
