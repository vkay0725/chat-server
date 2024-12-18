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
    g++ -o server server.c -lpthread
    g++ -o client client.c -lpthread
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

## Contributing
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a new Pull Request.

## Contact
For any questions or suggestions, please open an issue or contact the project maintainer.
