#include <string>
#include <fstream>
#include <sstream>

inline std::string GetDataFromFile(std::string const& name)
{
	std::ifstream fstream(name);
	if (!fstream.is_open())
	{
		throw std::ios_base::failure("error in getting data from file \"" + name + "\".");
	}
	std::stringstream ss;
	ss << fstream.rdbuf();
	return 	ss.str();
}



