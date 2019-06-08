/*
 * Loader.cpp
 *
 *  Created on: Jun 8, 2019
 *      Author: jim
 */

#include "Loader.h"

Loader::Loader(std::string configPath):
	config_(configPath),
	xmlFile_(config_.c_str())
{
    doc_.parse<0>(xmlFile_.data());
}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

