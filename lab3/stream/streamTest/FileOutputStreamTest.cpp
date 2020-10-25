#include "../../../catch2/catch.hpp"
#include "../stream/IOutputDataStream.h"
#include "TestUtilities.h"

TEST_CASE("object creation CFileOutputStream")
{
	WHEN("object correct creation")
	{
		CFileOutputStream fileOutputtream("data/output.bin");
	}

	WHEN("creation with non-existent file")
	{
		CHECK_THROWS_AS(CFileOutputStream("A://A"), std::ios_base::failure);
	}
}

TEST_CASE("WriteByte in an object CFileOutputStream")
{
	CFileOutputStream fileOutputStream("data/output.bin");
	fileOutputStream.WriteByte('a');
	CHECK("a" == GetDataFromFile("data/output.bin"));
}

TEST_CASE("writes a block of data to a file")
{
	CFileOutputStream fileOutputStream("data/output.bin");
	std::vector<char> buff{ 's', 't', 'r' };
	fileOutputStream.WriteBlock(buff.data(), 3);
	CHECK("str" == GetDataFromFile("data/output.bin"));
}