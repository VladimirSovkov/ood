#include <iostream>
#include <vector>
#include "IInputDataStream.h"
#include "IOutputDataStream.h"
#include "InputDataStreamDecorator.h"
#include "OutputDataStreamDecorator.h"

using namespace std;

const string ENCRYPT_OPTION = "--encrypt";
const string DECRYPT_OPTION = "--decrypt";
const string COMPRESS_OPTION = "--compress";
const string DECOMPRESS_OPTION = "--decompress";

void CopyToFile(unique_ptr<IInputDataStream>& input, unique_ptr<IOutputDataStream>& output)
{
	while (!input->IsEOF())
	{
		uint8_t ch = input->ReadByte();
		output->WriteByte(ch);
	};
}

void ÀrgumentHandling(vector<string> const& args)
{
	if (args.size() < 2)
	{
		throw logic_error("Not enough arguments");
	}


	unique_ptr<IInputDataStream> input = make_unique<CFileInputStream>(args[args.size() - 2]);
	unique_ptr<IOutputDataStream> output = make_unique<CFileOutputStream>(args[args.size() - 1]);

	for (auto i = 1; i < args.size() - 2; i++)
	{
		if (args[i] == ENCRYPT_OPTION)	
		{
			uint32_t key = atoi(args[++i].c_str());
			output = make_unique<CEncryptOutputStreamDecorator>(move(output), key);
		}
		else if (args[i] == DECRYPT_OPTION)
		{
			uint32_t key = atoi(args[++i].c_str());
			input = make_unique<CDecryptInputStreamDecorator>(move(input), key);
		}
		else if (args[i] == COMPRESS_OPTION)
		{
			output = make_unique<CCompressOutputStreamDecorator>(move(output));
		}
		else if (args[i] == DECOMPRESS_OPTION)
		{
			input = make_unique<CDecompressInputStreamDecorator>(move(input));
		}
		else
		{
			throw logic_error("Unhandled option");
		}
	}

	CopyToFile(input, output);
}

int main(int argc, char* argv[])
{
	vector<string> args(argv, argv + argc);
	try
	{
		ÀrgumentHandling(args);
	}
	catch (const std::exception& exception)
	{
		cout << exception.what() << endl;
	}
}