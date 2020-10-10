#include "../../../catch2/catch.hpp"
#include "../stream/IOutputDataStream.h"

TEST_CASE("object creation CMemoryOutputStream")
{
	std::ostringstream stream("string");
	CMemoryOutputStream fileOutputtream(stream);
}

TEST_CASE("WriteByte in an object CMemoryOutputStream")
{
	std::ostringstream stream;
	CMemoryOutputStream fileOutputtream(stream);
	fileOutputtream.WriteByte('a');
	CHECK("a" == stream.str());
}

TEST_CASE("WriteBlock in a object CMemoryOutputStream")
{
	std::ostringstream stream;
	CMemoryOutputStream fileOutputtream(stream);
	std::vector<char> buff{'s', 't', 'r'};
	fileOutputtream.WriteBlock(buff.data(), 2);
	CHECK("st" == stream.str());

	fileOutputtream.WriteBlock(buff.data(), 5);
	CHECK("ststrýý" == stream.str());
}