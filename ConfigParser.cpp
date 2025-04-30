/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:20:59 by irychkov          #+#    #+#             */
/*   Updated: 2025/04/30 12:22:52 by irychkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser :: ConfigParser( void ) { }

ConfigParser :: ~ConfigParser( void ) { }

// --- CustomError ---
ConfigParser::CustomError::CustomError( const std::string& msg ) {
	_msg = msg;
}

const char* ConfigParser::CustomError::what() const throw() {
	return (_msg.c_str());
}
