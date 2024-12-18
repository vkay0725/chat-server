#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>

#define PORT 8080
#define BUFFER_SIZE 1024

struct Client {
    int socket;
    std::string name;
};

std::vector<Client> clients;
std::mutex clients_mutex;
std::ofstream chatLog("chat_history_server.txt", std::ios::app); 

void notify_clients(const std::string &sender, const std::string &message, const std::string &target) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto &client : clients) {
        if (target == "all" || client.name == target) {
            std::string full_message = "from " + sender + ": " + message;
            send(client.socket, full_message.c_str(), full_message.length(), 0);
            if (target == "all") {
                chatLog << "To all: " << full_message << std::endl;
            } 
            else {
                chatLog << "To " << client.name << ": " << full_message << std::endl;
            }
        }
    }
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    std::string name;

    int name_bytes = recv(client_socket, buffer, BUFFER_SIZE-1, 0);
    buffer[name_bytes] = '\0';
    name = buffer;

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back({client_socket, name});
    }

    std::cout << "Client connected: " << name << std::endl;
    chatLog << "Client connected: " << name << std::endl;

    while (true) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cout << "Client disconnected: " << name << std::endl;
            chatLog << "Client disconnected: " << name << std::endl;
            break;
        }

        std::string message(buffer);
        std::cout << name << " sent: " << message << std::endl;
        chatLog << "From " << name << ": " << message << std::endl;

        size_t pos = message.find(' '); // message format: "target message"
        if (pos != std::string::npos) {
            std::string target = message.substr(0, pos);
            std::string actual_message = message.substr(pos + 1);
            notify_clients(name, actual_message, target);
        } else {
            std::string error_msg = "Invalid format. Use 'target message' or 'all message'.";
            send(client_socket, error_msg.c_str(), error_msg.length(), 0);
        }
    }

    close(client_socket);
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),[client_socket](const Client &c) { return c.socket == client_socket; }),clients.end());
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed\n";
        return -1;
    }

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        return -1;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Listen failed\n";
        return -1;
    }

    std::cout << "Server running, waiting for connections...\n";

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::thread(handle_client, client_socket).detach();
        //std::thread(handle_client, client_socket).join(); 
    }

    close(server_socket);
    chatLog.close();
    return 0;
}
