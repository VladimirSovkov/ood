#include "../../../catch2/catch.hpp"
#include "../stream/IOutputDataStream.h"
#include "../stream/OutputDataStreamDecorator.h"
#include "../stream/InputDataStreamDecorator.h"
#include "TestUtilities.h"
#include "../stream/AddDecorator.h"

TEST_CASE("compress data to a memory stream")
{
	WHEN("write byte")
	{
		std::ostringstream ss("", std::ios_base::ate);
		{
			auto stream = std::make_unique<CMemoryOutputStream>(ss);
			auto compressedStream = std::make_unique<CCompressOutputStreamDecorator>(std::move(stream));
			compressedStream->WriteByte('A');
			compressedStream->WriteByte('2');
			compressedStream->WriteByte('2');
			compressedStream->WriteByte('%');
			compressedStream->WriteByte('%');
		}
		std::string sample = "\1A\2" + std::string("2\2%");
		CHECK(sample == ss.str());
	}

	WHEN("write block")
	{
		std::ostringstream ss("", std::ios_base::ate);
		{
			auto stream = std::make_unique<CMemoryOutputStream>(ss);
			auto compressedStream = std::make_unique<CCompressOutputStreamDecorator>(std::move(stream));
			compressedStream->WriteBlock("v", 1);
			compressedStream->WriteBlock("ddddd", 3);
			compressedStream->WriteBlock("0000", 4);
			compressedStream->WriteBlock("$$$", 3);
		}
		std::string sample = "\1v\3d\4" + std::string("0\3$");
		CHECK(sample == ss.str());
	}
	
	WHEN("write block and write byte")
	{
		std::ostringstream ss("", std::ios_base::ate);
		{
			auto stream = std::make_unique<CMemoryOutputStream>(ss);
			auto compressedStream = std::make_unique<CCompressOutputStreamDecorator>(std::move(stream));
			compressedStream->WriteByte('A');
			compressedStream->WriteByte('b');
			compressedStream->WriteByte('b');
			compressedStream->WriteBlock("0000", 4);
			compressedStream->WriteBlock("$$$", 3);
		}
		std::string sample = "\1A\2b\4" + std::string("0\3$");
		CHECK(sample == ss.str());

	}
}

TEST_CASE("compress data into a file stream")
{
	WHEN("write byte")
	{
		{
			auto data—ompression =
				std::make_unique<CFileOutputStream>("data/output.dat")
				<< AddFunc<CCompressOutputStreamDecorator>();
			data—ompression->WriteByte('A');
			data—ompression->WriteByte('2');
			data—ompression->WriteByte('2');
			data—ompression->WriteByte('%');
			data—ompression->WriteByte('%');
		}
		std::string sample = "\1A\2" + std::string("2\2%");
		CHECK(sample == GetDataFromFile("data/output.dat"));
	}

	WHEN("write block")
	{
		{
			auto data—ompression =
				std::make_unique<CFileOutputStream>("data/output.dat")
				<< AddFunc<CCompressOutputStreamDecorator>();
			data—ompression->WriteBlock("v", 1);
			data—ompression->WriteBlock("ddddd", 3);
			data—ompression->WriteBlock("0000", 4);
			data—ompression->WriteBlock("$$$", 3);
		}
		std::string sample = "\1v\3d\4" + std::string("0\3$");
		CHECK(sample == GetDataFromFile("data/output.dat"));
	}

	WHEN("write block and write byte")
	{
		{
			auto data—ompression =
				std::make_unique<CFileOutputStream>("data/output.dat")
				<< AddFunc<CCompressOutputStreamDecorator>();
			data—ompression->WriteByte('A');
			data—ompression->WriteByte('b');
			data—ompression->WriteByte('b');
			data—ompression->WriteBlock("0000", 4);
			data—ompression->WriteBlock("$$$", 3);
		}

		std::string sample = "\1A\2b\4" + std::string("0\3$");
		CHECK(sample == GetDataFromFile("data/output.dat"));
	}
}

TEST_CASE("decompress data to a memory stream")
{
	SECTION("read byte")
	{
		WHEN("correct read")
		{
			std::istringstream ss("\1a\2b");
			auto decompressMemory =
				std::make_unique<CMemoryInputStream>(ss)
				<< AddFunc<CDecompressInputStreamDecorator>();
			CHECK('a' == decompressMemory->ReadByte());
			CHECK('b' == decompressMemory->ReadByte());
			CHECK('b' == decompressMemory->ReadByte());
		}

		WHEN("read after end of data")
		{
			std::istringstream ss("\1a");
			auto decompressMemory =
				std::make_unique<CMemoryInputStream>(ss)
				<< AddFunc<CDecompressInputStreamDecorator>();
			decompressMemory->ReadByte();
			CHECK('\0' == decompressMemory->ReadByte());
			CHECK_THROWS_AS(decompressMemory->ReadByte(), std::ios_base::failure);
		}
	}

	SECTION("read block")
	{
		WHEN("correct read")
		{
			std::istringstream ss("\1a\2b");
			std::vector<char> answer(3);
			auto decompressMemory =
				std::make_unique<CMemoryInputStream>(ss)
				<< AddFunc<CDecompressInputStreamDecorator>();
			decompressMemory->ReadBlock(answer.data(), 3);
			std::vector<char> sample{ 'a', 'b', 'b' };
			CHECK(sample == answer);
		}

		WHEN("block read size is larger than memory size")
		{
			std::istringstream ss("\2b");
			std::vector<char> answer(4);
			auto decompressMemory =
				std::make_unique<CMemoryInputStream>(ss)
				<< AddFunc<CDecompressInputStreamDecorator>();
			auto size = decompressMemory->ReadBlock(answer.data(), 5);
			std::vector<char> sample{ 'b', 'b', '\0', '\0' };
			CHECK(3 == size);
			CHECK(sample == answer);
		}
	}

	SECTION("end of data check")
	{
		std::istringstream ss("");
		auto decompressMemory =
			std::make_unique<CMemoryInputStream>(ss)
			<< AddFunc<CDecompressInputStreamDecorator>();
		WHEN("pointer has not reached the end")
		{
			CHECK_FALSE(decompressMemory->IsEOF());
		}

		WHEN("pointer reached the end")
		{
			decompressMemory->ReadByte();
			CHECK(decompressMemory->IsEOF());
		}
	}
}

TEST_CASE("decompress data into a file stream")
{
	SECTION("read byte")
	{
		WHEN("correct read")
		{
			{
				auto compressFile =
					std::make_unique<CFileOutputStream>("data/output.dat")
					<< AddFunc<CCompressOutputStreamDecorator>();
				compressFile->WriteBlock("vddd0000", 8);
			}

			auto decompressFile =
				std::make_unique<CFileInputStream>("data/output.dat")
				<< AddFunc<CDecompressInputStreamDecorator>();
			CHECK('v' == decompressFile->ReadByte());
			CHECK('d' == decompressFile->ReadByte());
			CHECK('d' == decompressFile->ReadByte());
		}

		WHEN("read after end of file")
		{
			auto decompressFile =
				std::make_unique<CFileInputStream>("data/emptyFile")
				<< AddFunc<CDecompressInputStreamDecorator>();
			CHECK('\0' == decompressFile->ReadByte());
			CHECK_THROWS_AS(decompressFile->ReadByte(), std::ios_base::failure);
		}
	}

	SECTION("read block")
	{
		WHEN("correct read")
		{
			{
				auto compressFile =
					std::make_unique<CFileOutputStream>("data/output.dat")
					<< AddFunc<CCompressOutputStreamDecorator>();
				compressFile->WriteBlock("vddd0000", 8);
			}

			auto decompressFile =
				std::make_unique<CFileInputStream>("data/output.dat")
				<< AddFunc<CDecompressInputStreamDecorator>();
			std::vector<char> answer(5);
			CHECK(5 == decompressFile->ReadBlock(answer.data(), 5));
			std::vector<char> sample{ 'v', 'd', 'd', 'd', '0' };
			CHECK(sample == answer);
		}

		WHEN("block read size is larger than memory size")
		{
			auto decompressFile =
				std::make_unique<CFileInputStream>("data/emptyFile")
				<< AddFunc<CDecompressInputStreamDecorator>();
			std::vector<char> answer(3);
			CHECK(1 == decompressFile->ReadBlock(answer.data(), 3));
			std::vector<char> sample{ '\0', '\0', '\0' };
			CHECK(sample == answer);
		}
	}

	SECTION("end of file check")
	{
		auto decompressFile =
			std::make_unique<CFileInputStream>("data/emptyFile")
			<< AddFunc<CDecompressInputStreamDecorator>();
		WHEN("the pointer has not reached the end of the file")
		{
			CHECK_FALSE(decompressFile->IsEOF());
		}

		WHEN("the pointer has reached the end of the file")
		{
			decompressFile->ReadByte();
			CHECK(decompressFile->IsEOF());
		}
	}
}

TEST_CASE("data compression and decompression")
{
	WHEN("memory stream")
	{
		std::ostringstream oss;
		{
			auto compressMemeory =
				std::make_unique< CMemoryOutputStream>(oss)
				<< AddFunc<CCompressOutputStreamDecorator>();
			compressMemeory->WriteBlock("vdddoooo", 8);
		}

		std::istringstream iss(oss.str());
		auto decompressMemory =
			std::make_unique<CMemoryInputStream>(iss)
			<< AddFunc<CDecompressInputStreamDecorator>();
		std::vector<char> answer (8);
		decompressMemory->ReadBlock(answer.data(), 8);
		std::vector<char> sample{'v', 'd', 'd', 'd', 'o', 'o', 'o', 'o'};
		CHECK(sample == answer);
	}

	WHEN("file stream")
	{
		std::vector<char> vect(8);
		{
			auto abc =
				std::make_unique<CFileOutputStream>("data/output.dat")
				<< AddFunc<CCompressOutputStreamDecorator>();
			abc->WriteBlock("daabb", 8);
		}

		auto decompressFile = std::make_unique<CFileInputStream>("data/output.dat")
			<< AddFunc<CDecompressInputStreamDecorator>();
		std::vector<char> answer(5);
		std::vector<char> sample{'d', 'a', 'a', 'b', 'b'};
		decompressFile->ReadBlock(answer.data(), 5);
		CHECK(sample == answer);
	}
}