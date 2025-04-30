/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:17:41 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/30 16:38:38 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketManager.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>

// Constructor: sets up sockets for each server defined in the config
SocketManager::SocketManager(const std::vector<Server>& servers) {
	setupSockets(servers);
}

// Destructor: closes all open file descriptors
SocketManager::~SocketManager() {
	for (size_t i = 0; i < _poll_fds.size(); ++i)
		close(_poll_fds[i].fd);
}

// Custom exception for socket errors
SocketManager::SocketError::SocketError(const std::string& msg) {
	_msg = msg;
}

const char* SocketManager::SocketError::what() const throw() {
	return (_msg.c_str());
}

// Set up sockets for each server (host:port)
void SocketManager::setupSockets(const std::vector<Server>& servers) {
	for (size_t i = 0; i < servers.size(); ++i) {
		int fd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
		if (fd < 0)
			throw SocketError("socket() failed: " + std::string(strerror(errno)));

		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) { // Make socket non-blocking
			close(fd);
			throw SocketError("fcntl() failed: " + std::string(strerror(errno)));
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(servers[i].getPort()); // Convert port to network byte order

		// Convert hostname to IP address
		if (servers[i].getHost() == "localhost")
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		else
			addr.sin_addr.s_addr = inet_addr(servers[i].getHost().c_str());

		if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { // Bind socket to IP:port
			close(fd);
			throw SocketError("bind() failed on " + servers[i].getHost() + ":" + std::to_string(servers[i].getPort()) + ": " + strerror(errno));
		}

		if (listen(fd, SOMAXCONN) < 0) { // Start listening for incoming connections
			close(fd);
			throw SocketError("listen() failed: " + std::string(strerror(errno)));
		}

		// Register fd in poll list
		_poll_fds.push_back((pollfd){ fd, POLLIN, 0 });
		_listen_map[fd] = servers[i]; // Map fd to its corresponding server

		std::cout << "Listening on " << servers[i].getHost() << ":" << servers[i].getPort() << std::endl;
	}
}

// Main server loop using poll
void SocketManager::run() {
	while (true) {
		int n = poll(&_poll_fds[0], _poll_fds.size(), -1); // Block until at least one fd is ready
		if (n < 0)
			throw SocketError("poll() failed: " + std::string(strerror(errno)));

		for (size_t i = 0; i < _poll_fds.size(); ++i) {
			if (_poll_fds[i].revents & POLLIN) { // Ready to read (incoming data or connection)
				if (_listen_map.count(_poll_fds[i].fd))
					handleNewConnection(_poll_fds[i].fd); // Accept a new client
				else
					handleClientData(_poll_fds[i].fd, i); // Handle data from existing client
			}
		}
	}
}

// Accept new client and add to poll list
void SocketManager::handleNewConnection(int listen_fd) {
	int client_fd = accept(listen_fd, NULL, NULL);
	if (client_fd < 0)
		return;

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(client_fd);
		return;
	}

	_poll_fds.push_back((pollfd){ client_fd, POLLIN, 0 });
	std::cout << "Accepted client on fd: " << client_fd << std::endl;
}

// Read data from client, send fixed response, then close
void SocketManager::handleClientData(int client_fd, size_t index) {
	char buffer[1024];
	int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0) {
		close(client_fd);
		_poll_fds.erase(_poll_fds.begin() + index);
		return;
	}

	// Static response for now
	std::string body = "<h1>Hello from Webserv!</h1>";
	std::stringstream response_stream;

	response_stream << "HTTP/1.1 200 OK\r\n";
	response_stream << "Content-Type: text/html\r\n";
	response_stream << "Content-Length: " << body.size() << "\r\n";
	response_stream << "Connection: close\r\n";
	response_stream << "\r\n";
	response_stream << body;

	// Convert stringstream to string
	std::string response = response_stream.str();

	send(client_fd, response.c_str(), response.size(), 0);
	close(client_fd);
	_poll_fds.erase(_poll_fds.begin() + index);
}
