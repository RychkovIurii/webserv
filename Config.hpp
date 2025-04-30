/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:34:50 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/30 11:01:09 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include <vector>

class Config {

	private:
		std::vector<Server> _servers;

	public:
		Config( void);
		~Config( void );
		Config( const Config& ) = default;
		Config& operator=( const Config& ) = default;

		void addServer( const Server& server );
		const std::vector<Server>& getServers( void ) const;
};