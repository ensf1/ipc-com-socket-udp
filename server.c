#include <stdio.h>
#include "string.h"
#include <sys/unistd.h>
#include <stdlib.h>
#include "networking.h"
#include "client.h"
#include "math.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

const int SERVER_PORT = 9010;

void *start_client(void *vargp){
    sleep(1);
    int *myid = (int *)vargp;
    printf("thread %d\t", *myid);
    client(SERVER_PORT,*myid);
    return NULL;
}

int main(int argc, char **argsv) {
    int socket_number;
    struct message message;
    double result = 0;
    char *end_of_number;
    long portions = strtol(argsv[1], &end_of_number, 10);
    long clients = strtol(argsv[2], &end_of_number, 10);
    long interaction_type = strtol(argsv[3], &end_of_number, 10);
    char long_string[64];
    char int_string[32];
    char interval[129] = "0-";
    long portion;
//    printf("%ld", portions);
//    printf("%ld", clients);
//    printf("%ld\n", interaction_type);
    printf("server pid %d\n", getpid());

    socket_number = new_socket_number();
    bind_socket_to_port(socket_number, SERVER_PORT);

    pthread_t thread_id[clients];
    for(int i=0;i<clients;i++){
        pthread_create(&thread_id[i], NULL, start_client, (void *)&thread_id[i]);
    }

    message = receive_message(socket_number);
    printf("%s\n", message.text);

    if (interaction_type == 1) {
        portion = (long) ceil(portions / clients);
//        printf("portion: %ld\n", portion);
        long end = 0;
        while (end + portion < portions) {
            end = end + portion;
            snprintf(long_string, 64, "%ld", end);
            strcat(interval, long_string);
            printf("interval sent: %s\n",interval);
            send_message(interval, socket_number, message.origin);
            snprintf(interval, 64, "%ld-", end);
//            printf("next interval: %s\n",interval);
            message = receive_message(socket_number);
            printf("%s\n", message.text);
            result += strtod(message.text, &end_of_number);
            printf("result while: %f\n",result);
        }
        snprintf(long_string, 64, "%ld", portions);
//        printf("long_string %s\n", long_string);
        strcat(interval, long_string);
//        printf("end: %ld:\n", end);
//        printf("portions: %ld:\n", portions);
//        printf("interval fim: %s\n", interval);
        send_message(interval, socket_number, message.origin);
        message = receive_message(socket_number);
        result += strtod(message.text, &end_of_number);
    }
    else if(interaction_type == 2){
        for(int i=0;i<portions;i++){
            snprintf(int_string, 32, "%d", i);
            send_message(int_string, socket_number, message.origin);
            message = receive_message(socket_number);
            printf("partial result %s\n", message.text);
            result += strtod(message.text, &end_of_number);
            printf("result while %f\t sum %f\n",result,strtod(message.text, &end_of_number));
        }
    }
    send_message("shutdown", socket_number, message.origin);

    printf("%.9f", result * 4);
    close(socket_number);




//    send_message("0-2", socket_number, message.origin);
//    message = receive_message(socket_number);
//    printf("%s\n", message.text);
//    result += strtod(message.text, &end_of_number);
//
//    send_message("2", socket_number, message.origin);
//    message = receive_message(socket_number);
//    printf("%s\n", message.text);
//    result += strtod(message.text, &end_of_number);
//
//    result += strtod(message.text, &end_of_number);


    return 0;
}
