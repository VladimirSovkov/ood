#include "../../../catch2/catch.hpp"
#include "../stream/IInputDataStream.h"

TEST_CASE("object creation CFileInputStream")
{
	WHEN("object correct creation")
	{
		CFileInputStream fileInputStream("data/input.bin");
	}

	WHEN("creation with non-existent file")
	{
		CHECK_THROWS_AS(CFileInputStream("data/input1.txt"), std::ios_base::failure);
	}
}

TEST_CASE("read byte from file into CFileInputStream")
{
	WHEN("correct byte reading")
	{
		CFileInputStream fileInputStream("data/input.bin");
		auto byte = fileInputStream.ReadByte();
		CHECK('v' == byte);

		byte = fileInputStream.ReadByte();
		CHECK('d' == byte);
	}

	WHEN("reading a byte from an empty file")
	{
		CFileInputStream fileInputStream("data/emptyFile");
		CHECK_THROWS_AS(fileInputStream.ReadByte(), std::ios_base::failure);
	}
}

TEST_CASE("check for end of file in CFileInputStream")
{
	CFileInputStream fileInputStream("data/emptyFile");
	WHEN("pointer at the end of the file")
	{
		CHECK_THROWS(fileInputStream.ReadByte());
		CHECK(fileInputStream.IsEOF());
	}

	WHEN("pointer not at the end of the file")
	{
		CHECK_FALSE(fileInputStream.IsEOF());
	}
}

TEST_CASE("read a block in an object CFileInputStream")
{
	WHEN("correct block reading")
	{
		CFileInputStream fileInputStream("data/input.bin");
		std::vector<char> buff(5);
		auto size = fileInputStream.ReadBlock(buff.data(), 5);
		std::vector<char> answer{ 'v', 'd', 'd', 'd', 'o' };
		CHECK(5 == size);
		CHECK(answer == buff);
		size = fileInputStream.ReadBlock(buff.data(), 5);
		answer = { 'o', 'o', 'o', '\r', '\n' };
		CHECK(answer == buff);
	}

	WHEN("transferred size exceeds file size")
	{
		CFileInputStream fileInputStream("data/emptyFile");
		std::vector<char> buff(5);
		auto size = fileInputStream.ReadBlock(buff.data(), 5);
		std::vector<char> sample(5);
		CHECK(0 == size);
		CHECK(sample == buff);
	}

	WHEN("the pointer is at the end of the file")
	{
		CFileInputStream fileInputStream("data/emptyFile");
		std::vector<char> buff(5);
		auto size = fileInputStream.ReadBlock(buff.data(), 5);
		std::vector<char> sample(5);
		CHECK(0 == size);
		CHECK(sample == buff);
	}
}



TEST_CASE("combining byte and block data read for file")
{
	CFileInputStream memory("data/input.bin");
	CHECK('v' == memory.ReadByte());
	std::vector<char> answer(2);
	std::vector<char> sample{ 'd', 'd' };
	memory.ReadBlock(answer.data(), 2);
	CHECK(sample == answer);
	CHECK('d' == memory.ReadByte());
}