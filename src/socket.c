#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <../include/socket.h>

#define BACKLOG 10

int server_fd, new_socket;

/*!
 * \brief Create socket
 * \details Maakt een socket aan op de gegeven poort
 * \param port De poort waarop de socket moet worden aangemaakt
 */
void hostSocket(int port) {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", port);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);
    printf("Connection accepted\n");

    
}
/*!
 * \brief Close socket
 * \details Sluit de socket
 */
void closeSocket() {
    close(new_socket);
    close(server_fd);
    printf("Socket closed\n");
}

/*!
 * \brief Verbind met socket
 * \details Verbind met de socket op het gegeven ip en poort
 * \param ip Het ip-adres waarmee verbonden moet worden
 * \param port De poort waarmee verbonden moet worden
 */

void connectToSocket(const char *host, int port) {
    struct sockaddr_in serv_addr;

    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, gethostbyname(host), &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to (%s) %s:%d\n", host, gethostbyname(host), port);
}

/*!
 * \brief Senddata
 * \details Stuurt data naar de socket
 * \param message De data die moet worden verstuurd
 */
void sendData (const char *message) {
    write(new_socket, message, strlen(message));
    printf("Data sent: %s\n", message);
}

/*!
 * \brief Listen for data
 * \details Luistert naar inkomende data op de socket
 */

char *listenForData() {
    static char buffer[1024] = {0};
    int valread;

    valread = read(new_socket, buffer, sizeof(buffer) - 1);
    if (valread > 0) {
        buffer[valread] = '\0';
        return buffer;
    }

    if (valread < 0) {
        perror("read");
    }

    return NULL;
}