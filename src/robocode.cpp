/*
	Project: ROBOCODE

	File:	 robocode.cpp
	
	Description:
	Main programm file

*/

#include "robocode.h"

////////////////////////////////
// Main 
int main(int argc, char* argv[])
{
	using namespace Robocode;	

	try
	{
		// Allow alternative input file as 1st parameter
		FileName_t inputFile = argc > 1 ? argv[1] : g_defaultInputFile;
		
		std::ifstream input(inputFile);
		
		if (!input) throw CError(ERR_INPUT_FILE_NOT_EXIST);

		while (input)
		{
			CBattleField bf;
			
			if (!(input >> bf))
			{
				return 1;
			}
			else
			{
				std::cout << bf;
			}
		}
		
	}
	catch (CError e)
	{
		//output error message
		std::cerr << e.GetMessage() << " :-(" << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << ERR_UNHANDLED_ERROR << std::endl;
		return 1;
	}
	
	return 0;
}

