//
// Created by kostarubtsov1990 on 04/12/17.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>


using namespace std;
#define MAX_CONNECTED_CLIENTS 10

Server::Server(int port): port(port), serverSocket(0) {
    cout << "Server" << endl;
}

void Server::start() {
    //Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        throw "Error opening socket";
    }
    //Asign a local address to the socket
    struct sockaddr_in serverAddress;
    bzero((void*)&serverAddress,sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error on binding";
    }
    listen(serverSocket,MAX_CONNECTED_CLIENTS);

    //Define the client socket's structures
    //Need to define another one for second player?
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    while (true) {
        cout << "Waiting for client connections...." << endl;
        char* massage = "Wait for opponent";

        //Accept a new client connection
        int firstPlayerClientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress, &clientAddressLen);

        if (firstPlayerClientSocket == -1)
            throw "Error on accept first client";

        cout << "first player connected" << endl;

        int n = write(firstPlayerClientSocket, &massage, sizeof(massage) + 1);

        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }

        //Accept a new client connection
        int secondPlayerClientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress, &clientAddressLen);

        if (secondPlayerClientSocket == -1)
            throw "Error on accept first client";


        cout << "first player connected" << endl;

        massage = "Wait for first player to take first move";

        n = write(secondPlayerClientSocket, &massage, sizeof(massage) + 1);

        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }


        handleOneTurn(firstPlayerClientSocket, secondPlayerClientSocket);

        //Close communication with the client
        close(firstPlayerClientSocket);
    }
}

void Server::handleOneTurn(int firstPlayerClientSocket, int secondPlayerClientSocket) {

}

void Server::stop() {}