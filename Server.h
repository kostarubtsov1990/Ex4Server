//
// Created by kostarubtsov1990 on 04/12/17.
//

#ifndef EX4SERVER_SERVER_H
#define EX4SERVER_SERVER_H


class Server {
public:

    Server(int port);
    void start();
    void stop();

private:
    int port;
    int serverSocket; // the socket's file descriptor

    void handleOneTurn(int firstPlayerClientSocket, int secondPlayerClientSocket);
};


#endif //EX4SERVER_SERVER_H
