#ifndef SOCKET_H
#define SOCKET_H

void hostSocket(int port);
void closeSocket();
void connectToSocket(const char* host, int port);
void sendData(const char* data);
char *listenForData();

#endif // SOCKET_H