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

struct Argument
{
	string name;
	int key;
};

void CopyToFile(unique_ptr<IInputDataStream>& input, unique_ptr<IOutputDataStream>& output)
{
	while (!input->IsEOF())
	{
		try
		{
			uint8_t ch = input->ReadByte();
			output->WriteByte(ch);
		}
		catch (const std::exception&)
		{
		}
	};
}

unique_ptr<IInputDataStream> DecorateInputStream(unique_ptr<IInputDataStream>&& input, vector<Argument> const& argsForInput)
{
	if (argsForInput.size() != 0)
	{
		for (auto it = argsForInput.crbegin(); it != argsForInput.crend(); it++)
		{
			if (it -> name == DECRYPT_OPTION)
			{
				input = make_unique<CDecryptInputStreamDecorator>(move(input), it->key);
			}
			else if (it->name == DECOMPRESS_OPTION)
			{
				input = make_unique<CDecompressInputStreamDecorator>(move(input));
			}
		}
	}
	return std::move(input);
}

unique_ptr<IOutputDataStream> DecorateOutputStream(unique_ptr<IOutputDataStream>&& output, vector<Argument> const& argsForOutput)
{
	if (argsForOutput.size() != 0)
	{
		for (size_t i = 0; i < argsForOutput.size(); i++)
		{
			if (argsForOutput[i].name == ENCRYPT_OPTION)
			{
				output = make_unique<CEncryptOutputStreamDecorator>(move(output), argsForOutput[i].key);
			}
			else if (argsForOutput[i].name == COMPRESS_OPTION)
			{
				output = make_unique<CCompressOutputStreamDecorator>(move(output));
			}
		}
	}
	return std::move(output);
}

void ÀrgumentHandling(vector<string> const& args)
{
	if (args.size() < 2)
	{
		throw logic_error("Not enough arguments");
	}

	unique_ptr<IInputDataStream> input = make_unique<CFileInputStream>(args[args.size() - 2]);
	unique_ptr<IOutputDataStream> output = make_unique<CFileOutputStream>(args[args.size() - 1]);

	std::vector<Argument> argsForOutput;
	std::vector<Argument> argsForIntput;
		
	for (size_t i = 1; i < args.size() - 2; i++)
	{
		if (args[i] == ENCRYPT_OPTION)	
		{
			argsForOutput.push_back({ args[i], atoi(args[++i].c_str()) });
		}
		else if (args[i] == DECRYPT_OPTION)
		{
			argsForIntput.push_back({ args[i], atoi(args[++i].c_str()) });
		}
		else if (args[i] == COMPRESS_OPTION)
		{
			argsForOutput.push_back({ args[i], 0 });
		}
		else if (args[i] == DECOMPRESS_OPTION)
		{
			argsForIntput.push_back({ args[i], 0 });
		}
		else
		{
			throw logic_error("Unhandled option");
		}
	}

	input = DecorateInputStream(std::move(input), argsForIntput);
	output = DecorateOutputStream(std::move(output), argsForOutput);

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