#pragma once
#include <ios>
#include <fstream>
#include <sstream>


class IOutputDataStream
{
public:
	// Записывает в поток данных байт
	// Выбрасывает исключение std::ios_base::failure в случае ошибки
	virtual void WriteByte(uint8_t data) = 0;

	// Записывает в поток блок данных размером size байт, 
	// располагающийся по адресу srcData,
	// В случае ошибки выбрасывает исключение std::ios_base::failure
	virtual void WriteBlock(const void* srcData, std::streamsize size) = 0;

	virtual ~IOutputDataStream() = default;
};

class CFileOutputStream : public IOutputDataStream
{
public:
	CFileOutputStream(const std::string& fileName)
		:m_stream(fileName, std::ios::binary)
	{
		if (!m_stream.is_open())
		{
			throw std::ios_base::failure("Output file '" + fileName + "' not found");
		}
	}

	void WriteByte(uint8_t data) override
	{
		char byte = data;
		m_stream.write(&byte, 1);
		if (m_stream.fail() || m_stream.bad())
		{
			throw std::ios_base::failure("Error writing byte to file");
		}
		m_stream.flush();
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		const char* buff = static_cast<const char*>(srcData);
		m_stream.write(buff, size);
		if (m_stream.fail() || m_stream.bad())
		{
			throw std::ios_base::failure("Error in writing a block of bytes to a file");
		}
		m_stream.flush();
	}

private:
	std::ofstream m_stream;
};

class CMemoryOutputStream : public IOutputDataStream
{
public:
	CMemoryOutputStream(std::ostringstream& stream)
		: m_stream(stream)
	{}

	void WriteByte(uint8_t data) override
	{
		m_stream.put(data);
	}
	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		const char* buff = static_cast<const char*>(srcData);
		m_stream.write(buff, size);
		if (m_stream.fail() || m_stream.bad())
		{
			throw std::ios_base::failure("Error in writing a block of bytes to a memory");
		}
	}

private:
	std::ostream& m_stream;
};
