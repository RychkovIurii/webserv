/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:31:52 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/30 13:45:09 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "ConfigParser.hpp"

int	main(int ac, char** av)
{
	std::string config_file;

	if (ac == 1)
		config_file = "default.conf";
	else if (ac == 2)
		config_file = av[1];
	else
	{
		std::cout << "\n=========USAGE=========" << std::endl;
		std::cout << "  ./webserv            # Uses default.conf" << std::endl;
		std::cout << "  ./webserv config.conf" << std::endl;
		return(0);
	}

	try {
		ConfigParser parser;
		Config config;

		std::vector<Server> all_servers = parser.parseConfig(config_file);
		for (size_t i = 0; i < all_servers.size(); ++i) {
			config.addServer(all_servers[i]);
		}

		// Print the configuration
		const std::vector<Server>& servers = config.getServers();
		for (size_t i = 0; i < servers.size(); ++i) {
			std::cout << "Server " << i + 1 << ": "
					<< servers[i].getHost() << ":"
					<< servers[i].getPort() << std::endl;

			const std::vector<std::string>& names = servers[i].getServerNames();
			for (size_t j = 0; j < names.size(); ++j) {
				std::cout << "  server_name: " << names[j] << std::endl;
			}

			const std::vector<Location>& locations = servers[i].getLocations();
			for (size_t k = 0; k < locations.size(); ++k) {
				std::cout << "  location: " << locations[k].path
						<< " -> root: " << locations[k].root
						<< ", autoindex: ";
				if (locations[k].autoindex)
					std::cout << "on";
				else
					std::cout << "off";
				std::cout << std::endl;
			}
		}
	} catch (const ConfigParser::CustomError& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	} catch (const std::exception& e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
