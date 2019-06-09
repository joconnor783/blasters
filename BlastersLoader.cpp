/*
 * BlastersLoader.cpp
 *
 *  Created on: Jun 8, 2019
 *      Author: jim
 */

#include "BlastersLoader.h"

#include <iostream>

const std::string kLogfilePath = "logFileName";

BlastersLoader::BlastersLoader(std::string configPath):
			Loader(configPath),
			logfilePath_("")
{

    xml_node<> *pRoot = doc_.first_node();
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
    		logfilePath_ = pNode->value();
    	}

    }

}

std::string BlastersLoader::getLogfilePath()
{
	return logfilePath_;
}

BlastersLoader::~BlastersLoader() {
	// TODO Auto-generated destructor stub
}

