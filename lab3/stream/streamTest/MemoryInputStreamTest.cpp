#include "../../../catch2/catch.hpp"
#include "../stream/IInputDataStream.h"


TEST_CASE("object creation CMemoryInputStream")
{
	WHEN("object correct creation")
	{
		std::istringstream stream;
		CMemoryInputStream memoryInputStream(stream);
	}
}

TEST_CASE("read byte from file into CMemoryInputStream")
{
	WHEN("correct byte reading")
	{
		std::istringstream stream("string");
		CMemoryInputStream memoryInputStream(stream);
		auto byte = memoryInputStream.ReadByte();
		CHECK('s' == byte);

		byte = memoryInputStream.ReadByte();
		CHECK('t' == byte);
	}

	WHEN("reading a byte from an empty file")
	{
		std::istringstream stream("");
		CMemoryInputStream memoryInputStream(stream);
		memoryInputStream.ReadByte();
		CHECK_THROWS_AS(memoryInputStream.ReadByte(), std::ios_base::failure);
	}
}

TEST_CASE("check for eof stream in CMemorynputStream")
{
	std::istringstream stream("");
	CMemoryInputStream memoryInputStream(stream);
	WHEN("pointer at the end")
	{
		memoryInputStream.ReadByte();
		CHECK(memoryInputStream.IsEOF());
	}

	WHEN("pointer not at the end ")
	{
		CHECK_FALSE(memoryInputStream.IsEOF());
	}
}

TEST_CASE("read a block in an object CMemoryInputStream")
{
	WHEN("correct block reading")
	{
		std::istringstream stream("str");
		CMemoryInputStream memoryInputStream(stream);
		char buff[5];
		auto size = memoryInputStream.ReadBlock(buff, 5);
		CHECK(3 == size);
	}

	WHEN("transferred size exceeds file size ")
	{
		std::istringstream stream("");
		CMemoryInputStream memoryInputStream(stream);
		char buff[2]{};
		auto size = memoryInputStream.ReadBlock(buff, 5);
		CHECK(0 == size);
		CHECK('\0' == buff[0]);
		CHECK('\0' == buff[1]);
	}

	WHEN("the pointer is at the end of the file")
	{
		std::istringstream stream("");
		CMemoryInputStream memoryInputStream(stream);
		memoryInputStream.ReadByte();
		char buff[2]{};
		auto size = memoryInputStream.ReadBlock(buff, 5);
		CHECK(0 == size);
		CHECK('\0' == buff[0]);
		CHECK('\0' == buff[1]);
	}
}

TEST_CASE("combining byte and block data read")
{
	std::istringstream stream("abcd");
	CMemoryInputStream memory(stream);
	CHECK('a' == memory.ReadByte());

	std::vector<char> answer(2);
	std::vector<char> sample{ 'b', 'c' };
	memory.ReadBlock(answer.data(), 2);
	CHECK(sample == answer);

	CHECK('d' == memory.ReadByte());
}