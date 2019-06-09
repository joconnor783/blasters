/*
 * BlastersLoader.h
 *
 *  Created on: Jun 9, 2019
 *      Author: jim
 */

#ifndef BLASTERSLOADER_H_
#define BLASTERSOADER_H_

#include "Loader.h"

class BlastersLoader: public Loader {
public:
	BlastersLoader(std::string configPath);
	virtual ~BlastersLoader();

	std::string getLogfilePath();

private:
	std::string logfilePath_;
};

#endif /* BLASTERSLOADER_H_ */
