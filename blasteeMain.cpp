#include <Blastee.h>

int main(int argc, char **argv)
{
	Blastee blastee;

	if (blastee.init())
	{
		blastee.run();
	}

	return 0;
}

