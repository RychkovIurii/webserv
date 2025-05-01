/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunCGI.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:34:45 by irychkov          #+#    #+#             */
/*   Updated: 2025/05/01 19:50:51 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RunCGI.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <iostream>

std::string runCGI(const std::string& script_path, const HttpRequest& request) {
	(void)request; // Suppress unused parameter warning temporarily
	int pipe_fd[2];
	if (pipe(pipe_fd) < 0) {
		perror("pipe");
		return "";
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return "";
	} else if (pid == 0) {
		// Child
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);

		char* argv[] = { const_cast<char*>(script_path.c_str()), NULL };
		char* envp[] = { NULL }; // You can add CGI-specific env variables here later

		execve(script_path.c_str(), argv, envp);
		exit(1); // If execve fails
	} else {
		// Parent
		close(pipe_fd[1]);
		char buffer[1024];
		std::stringstream output;
		int bytes;
		while ((bytes = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
			output.write(buffer, bytes);
		}
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
		return output.str();
	}
}
