#include "../../../catch2/catch.hpp"
#include "../stream/IOutputDataStream.h"
#include "../stream/IInputDataStream.h"
#include "../stream/OutputDataStreamDecorator.h"
#include "../stream/InputDataStreamDecorator.h"
#include "TestUtilities.h"

TEST_CASE("correct encryption and decryption of memory")
{
	WHEN("read byte")
	{
		std::ostringstream oss;
		{
			auto outStream = std::make_unique<CMemoryOutputStream>(oss);
			auto memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(outStream), 3);
			memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(memoryEncryptyion), 100500);
			memoryEncryptyion->WriteByte('a');
			memoryEncryptyion->WriteByte('b');
			memoryEncryptyion->WriteByte('c');
		}

		CHECK_FALSE("abc" == oss.str());

		std::istringstream iss(oss.str());
		auto inStream = std::make_unique<CMemoryInputStream>(iss);
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 3);
		memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(memoryDecryption), 100500);

		CHECK('a' == memoryDecryption->ReadByte());
		CHECK('b' == memoryDecryption->ReadByte());
		CHECK('c' == memoryDecryption->ReadByte());
	}

	WHEN("read byte")
	{
		std::ostringstream oss;
		{
			auto outStream = std::make_unique<CMemoryOutputStream>(oss);
			auto memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(outStream), 5);
			memoryEncryptyion->WriteBlock("@#0\n", 3);
		}

		CHECK_FALSE("@#0\n" == oss.str());

		std::istringstream iss(oss.str());
		auto inStream = std::make_unique<CMemoryInputStream>(iss);
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 5);
		std::vector<char> answer(4);
		std::vector<char> sample {'@', '#', '0', '\n'};
		memoryDecryption->ReadBlock(answer.data(), 4);
	}

	WHEN("check end of data ")
	{
		std::istringstream iss("");
		auto inStream = std::make_unique<CMemoryInputStream>(iss);
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 5);
		CHECK_FALSE(memoryDecryption->IsEOF());
		memoryDecryption->ReadByte();
		CHECK(memoryDecryption->IsEOF());
	}
}

TEST_CASE("correct file encryption and decryption")
{
	WHEN("read byte")
	{
		{
			auto outStream = std::make_unique<CFileOutputStream>("data/output.txt");
			auto memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(outStream), 3);
			memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(memoryEncryptyion), 100500);
			memoryEncryptyion->WriteByte('a');
			memoryEncryptyion->WriteByte('b');
			memoryEncryptyion->WriteByte('c');
		}

		CHECK_FALSE("abc" == GetDataFromFile("data/output.txt"));

		auto inStream = std::make_unique<CFileInputStream>("data/output.txt");
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 3);
		memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(memoryDecryption), 100500);


		CHECK('a' == memoryDecryption->ReadByte());
		CHECK('b' == memoryDecryption->ReadByte());
		CHECK('c' == memoryDecryption->ReadByte());
	}

	WHEN("read byte")
	{
		{
			auto outStream = std::make_unique<CFileOutputStream>("data/output.txt");
			auto memoryEncryptyion = std::make_unique<CEncryptOutputStreamDecorator>(std::move(outStream), 5);
				memoryEncryptyion->WriteBlock("@#0\n", 3);
		}

		auto inStream = std::make_unique<CFileInputStream>("data/output.txt");
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 5);
		std::vector<char> answer(4);
		std::vector<char> sample{ '@', '#', '0', '\n' };
		memoryDecryption->ReadBlock(answer.data(), 4);
	}

	WHEN("check end of data")
	{
		auto inStream = std::make_unique<CFileInputStream>("data/emptyFile");
		auto memoryDecryption = std::make_unique<CDecryptInputStreamDecorator>(std::move(inStream), 0);
		CHECK_FALSE(memoryDecryption->IsEOF());
		memoryDecryption->ReadByte();
		CHECK(memoryDecryption->IsEOF());
	}
}