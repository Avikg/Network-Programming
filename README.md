Simplified TCP Ping

This project implements a simplified version of the ping command using the Transmission Control Protocol (TCP) in the C programming language. It includes both a server and a client application.
Table of Contents

    Dependencies
    Compilation
    Usage
    Features
    Improvements

Dependencies

    GCC (GNU Compiler Collection) for compiling the C code.

Compilation

    Server: gcc server.c -o server -lpthread

    Client: gcc client.c -o client

Usage

    Server:
    Start the server by running: ./server

By default, the server listens on port 8080.

Client:
Run the client using: ./client [server_ip] [num_requests] [interval] [port]

        server_ip: IP address of the server (default is 127.0.0.1).
        num_requests: Number of ping requests to send (default is 4).
        interval: Interval in seconds between requests (default is 1).
        port: Port number to send the ping (default is 8080).

Features

    Server:
        Handles multiple incoming ping requests concurrently.
        Responds to each client's ping request with an acknowledgment containing the same payload.
        Logs each incoming ping request, including the source IP address, port, and RTT in a file named ping_log.txt.

    Client:
        Sends ping requests to the server and measures the round-trip time (RTT).
        Displays the acknowledgment from the server along with the RTT.
        Allows users to specify the server IP, number of ping requests, interval between requests, and port via command-line arguments.

Improvements

    Implemented a mutex lock to protect the logfile, ensuring that multiple ping handler processes do not overwrite the file concurrently.
    Enhanced error handling for better user feedback during connection failures or other issues.

