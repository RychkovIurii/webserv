/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:42:50 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/29 13:59:11 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int	main(int ac, char **av)
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
	return(0);
}
