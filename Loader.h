/*
 * Loader.h
 *
 *  Created on: Jun 8, 2019
 *      Author: jim
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <string>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

class Loader {
public:
	Loader(std::string configPath);
	virtual ~Loader();

protected:
	std::string config_;
	rapidxml::file<> xmlFile_;
	rapidxml::xml_document<> doc_;
};

#endif /* LOADER_H_ */
