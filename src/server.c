#include <../include/socket.h>
#include <stdio.h>

#define PORT 8069

int main() {
    hostSocket(PORT);
    while (1)
    {
        char input = scanf("%c", &input);
        sendData(&input);
        printf("Data sent: %c\n", input);

        char *data = listenForData();
        if (data != NULL) {
            printf("Data received: %s\n", data);
        }
    }
    
    return 0;
}