#pragma once
#include <ios>
#include <fstream>
#include <sstream>

class IInputDataStream
{
public:
	// Возвращает признак достижения конца данных потока
	// Выбрасывает исключение std::ios_base::failuer в случае ошибки
	virtual bool IsEOF()const = 0;

	// Считывает байт из потока. 
	// Выбрасывает исключение std::ios_base::failure в случае ошибки
	virtual uint8_t ReadByte() = 0;

	// Считывает из потока блок данных размером size байт, записывая его в память
	// по адресу dstBuffer
	// Возвращает количество реально прочитанных байт. Выбрасывает исключение в случае ошибки
	virtual std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) = 0;

	virtual ~IInputDataStream() = default;
};

class CFileInputStream : public IInputDataStream
{
public:
	CFileInputStream(const std::string& fileName) 
		: m_file(fileName, std::ios::binary)
	{
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("input file '" + fileName + "' not found");
		}
	}

	bool IsEOF() const override
	{
		return m_file.eof();
	}

	uint8_t ReadByte() override
	{
		char byte;
		m_file.get(byte);
		if (IsEOF())
		{
			throw std::ios_base::failure("Read when is eof");
		}
		return (uint8_t)byte;
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		m_file.read(static_cast<char*>(dstBuffer), size);
		if (m_file.bad())
			throw std::ios_base::failure("Error reading block from file");
		
		return m_file.gcount();
	}

private:
	std::ifstream m_file;
};

class CMemoryInputStream : public IInputDataStream
{
public:
	CMemoryInputStream(std::istringstream& stream)
		:m_stream(stream)
	{}
	
	bool IsEOF()const override
	{
		return m_stream.eof();
	}

	uint8_t ReadByte() override
	{
		if (IsEOF())
		{
			throw std::ios_base::failure("Read when is eof");
		}
		return static_cast<uint8_t>(m_stream.get());
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{

		m_stream.readsome(static_cast<char*>(dstBuffer), size);
		if (m_stream.bad())
		{
			throw std::ios_base::failure("error reading block from memory");
		}
		return m_stream.gcount();
			
	}

private:
	std::istream& m_stream;
};