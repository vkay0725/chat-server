# Chat Server

## Overview
This project is a simple chat server that allows multiple clients to connect and communicate with each other in real-time.

## Features
- Multi-client support
- Real-time messaging
- Message broadcasting

## Requirements
- `socket` library
- `threading` library

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/chat-server.git
    ```
2. Navigate to the project directory:
    ```sh
    cd chat-server
    ```

## Usage
### Running the Server and Client

1. Compile the server and client programs:
    ```sh
    g++ -o server server.cpp -lpthread
    g++ -o client client.cpp -lpthread
    ```

2. Start the server:
    ```sh
    ./server
    ```

3. In a new terminal, start the client:
    ```sh
    ./client
    ```

4. Repeat step 3 for additional clients.


