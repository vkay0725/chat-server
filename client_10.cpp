#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <thread>

#define PORT 8080
#define BUFFER_SIZE 1024

std::ofstream chatLog("chat_history_client.txt", std::ios::app); 

void receive_messages(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cout << "Server disconnected. Exiting...\n";
            chatLog << "Server disconnected.\n";
            exit(0); // exit if server disconnects
        }
        std::cout << buffer << std::endl;
        chatLog << "Server: " << buffer << std::endl;
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed!\n";
        return -1;
    }

    std::cout << "Connected to the server. Please enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    send(client_socket, name.c_str(), name.length(), 0);
    chatLog << "Name: " << name << std::endl;

    std::cout << "Start chatting! Use 'target message' or 'all message'. Type 'exit' to disconnect.\n";

    std::thread receiver_thread(receive_messages, client_socket);

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        send(client_socket, message.c_str(), message.length(), 0);
        chatLog << "You: " << message << std::endl;
    }

    close(client_socket);
    receiver_thread.join();
    chatLog.close();
    return 0;
}
