#include <../include/socket.h>
#include <stdio.h>

#define PORT 8069

int main() {
    hostSocket(PORT);
    while (1)
    {
        char *data = listenForData();
        if (data != NULL) {
            printf("Data received: %s\n", data);
        }

        // Allow user to type data
        char input[1024];
        printf("Enter data to send: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            sendData(input);
            printf("Data sent: %s\n", input);
        }
    }
}