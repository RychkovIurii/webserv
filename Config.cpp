/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:36:29 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/30 10:37:42 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config :: Config( void ) { }

Config :: ~Config( void ) { }

void Config :: addServer( const Server& server ) {
	_servers.push_back(server);
}
const std::vector<Server>& Config :: getServers( void ) const {
	return _servers;
}