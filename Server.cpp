//
// Created by kostarubtsov1990 on 04/12/17.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string.h>


using namespace std;
#define MAX_CONNECTED_CLIENTS 10
#define BUF_SIZE 1024

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



    while (true) {
        cout << "Waiting for client connections...." << endl;

        char* message;

        int firstPlayerClientSocket = connectPlayer(firstPlayer);
        int secondPlayerClientSocket = connectPlayer(secondPlayer);

        message = "START";

        int n = write(firstPlayerClientSocket, message, strlen(message) + 1);

        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }

        handleGame(firstPlayerClientSocket, secondPlayerClientSocket);

        //Close communication with the client
        close(firstPlayerClientSocket);
        close(secondPlayerClientSocket);
    }
}

void Server::handleGame(int firstPlayerClientSocket, int secondPlayerClientSocket) {

    gameStatus status;

    while (status != finished) {
        status = handleDirection(firstPlayerClientSocket, secondPlayerClientSocket);

        if (status == finished)
            break;

        status = handleDirection(secondPlayerClientSocket, firstPlayerClientSocket);
    }
}

gameStatus Server::handleDirection(int from, int to) {

    char clientQueryBuffer [BUF_SIZE];

    int n = read(from, clientQueryBuffer, sizeof(clientQueryBuffer));

    if (n == -1) {
        cout << "Error reading arg1" << endl;
        return finished;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return finished;
    }

    n = write(to, clientQueryBuffer, sizeof(clientQueryBuffer));

    if (n == -1) {
        cout << "Error reading arg1" << endl;
        return finished;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return finished;
    }
    if (strcmp(clientQueryBuffer, "END") == 0) {
        return finished;
    }
}

int Server::connectPlayer(player player) {
    char* massage;

    if (player == firstPlayer)
        massage = "wait_for_opponent";
    else
        massage = "Wait_for_first_move";

    //Define the client socket's structures
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);


    //Accept a new client connection
    int playerClientSocket = accept(serverSocket,(struct sockaddr*)&clientAddress, &clientAddressLen);

    if (playerClientSocket == -1)
        throw "Error on accept first client";

    if (player == firstPlayer)
        cout << "first player connected" << endl;
    else
        cout << "second player connected" << endl;

    int n = write(playerClientSocket, massage, strlen(massage) + 1);

    if (n == -1) {
        cout << "Error writing to socket" << endl;
        return -1;
    }
    return playerClientSocket;
}



void Server::stop() {}