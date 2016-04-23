#include <iostream>
#include <tinyxml.h>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	cout << "argc = " << argc << endl;
	if (argc < 2)
	{
		cout << "Usage: RPiGpioTriggerMain [configFilename]" << endl;
		return -1;
	}
	// Read config file
	string configFilename = argv[1];
	
	cout << "Done!" << endl;
}
