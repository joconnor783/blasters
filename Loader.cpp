/*
 * Loader.cpp
 *
 *  Created on: Jun 8, 2019
 *      Author: jim
 */

#include "Loader.h"

#include <iostream>

const std::string kLogfilePath = "logFileName";

Loader::Loader(std::string configPath):
	config_(configPath),
	xmlFile_(config_.c_str())
{


    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile_.data());

    xml_node<> *pRoot = doc.first_node();
    // With the xml example above this is the <document/> node

    std::cout << "parsing xml\n";

    for (xml_node<> *pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
    {
    	if (pNode == nullptr)
    	{
    		break;
    	}

    	std::cout << pNode->name() << std::endl;

    	std::string name(pNode->name());

    	if (name == kLogfilePath)
    	{
    		std::cout << "found first node with value" << pNode->value() << std::endl;
    	}

    }
}

Loader::~Loader() {
	// TODO Auto-generated destructor stub
}

