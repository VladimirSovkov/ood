#include "../../../catch2/catch.hpp"
#include "../stream/IOutputDataStream.h"
#include "TestUtilities.h"

TEST_CASE("object creation CFileOutputStream")
{
	WHEN("object correct creation")
	{
		CFileOutputStream fileOutputtream("data/output.dat");
	}

	WHEN("creation with non-existent file")
	{
		CHECK_THROWS_AS(CFileOutputStream("A://A"), std::ios_base::failure);
	}
}

TEST_CASE("WriteByte in an object CFileOutputStream")
{
	CFileOutputStream fileOutputStream("data/output.dat");
	fileOutputStream.WriteByte('a');
	CHECK("a" == GetDataFromFile("data/output.dat"));
}

TEST_CASE("writes a block of data to a file")
{
	CFileOutputStream fileOutputStream("data/output.dat");
	std::vector<char> buff{ 's', 't', 'r' };
	fileOutputStream.WriteBlock(buff.data(), 3);
	CHECK("str" == GetDataFromFile("data/output.dat"));
}