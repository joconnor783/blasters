#include <Blasters.h>


int main(int argc, char **argv)
{

	Blasters blasters;

	if (blasters.init())
	{
		blasters.run();
	}

}
