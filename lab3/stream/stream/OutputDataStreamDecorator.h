#pragma once
#include "IOutputDataStream.h"
#include <vector>
#include "EncryptTable.h"

class COutputStreamDecorator : public IOutputDataStream
{
public:
	virtual ~COutputStreamDecorator() = default;

	virtual void WriteByte(uint8_t data) override
	{
		m_stream->WriteByte(data);
	}
	virtual void WriteBlock(const void* srcData, std::streamsize size) override
	{
		m_stream->WriteBlock(srcData, size);
	}

protected:
	COutputStreamDecorator(std::unique_ptr<IOutputDataStream>&& stream)
		: m_stream(std::move(stream))
	{}

private:
	std::unique_ptr<IOutputDataStream> m_stream;
};

class CCompressOutputStreamDecorator : public COutputStreamDecorator
{
public:
	CCompressOutputStreamDecorator(std::unique_ptr<IOutputDataStream>&& stream)
		: COutputStreamDecorator(std::move(stream))
	{
	}

	~CCompressOutputStreamDecorator()
	{
		FlushByte();
	}

	void WriteByte(uint8_t data) override
	{
		if (data != m_symbol || m_count >= UINT8_MAX)
		{
			FlushByte();
			m_symbol = data;
		}
		++m_count;
	}
	
	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		const uint8_t* buff = static_cast<const uint8_t*>(srcData);
		for (std::streamsize i = 0; i < size; ++i)
		{
			uint8_t ch = *(buff + i);
			WriteByte(ch);
		}
	}

private:

	void FlushByte()
	{
		if (m_count > 0)
		{
			COutputStreamDecorator::WriteByte(static_cast<uint8_t>(m_count));
			COutputStreamDecorator::WriteByte(m_symbol);
			m_count = 0;
		}
	}

	uint8_t m_symbol = '\0';
	size_t m_count = 0;
};

class CEncryptOutputStreamDecorator : public COutputStreamDecorator
{
public:
	CEncryptOutputStreamDecorator(std::unique_ptr<IOutputDataStream>&& stream, uint32_t key)
		: COutputStreamDecorator(std::move(stream)),
		m_encryptor(key)
	{
	}

	void WriteByte(uint8_t data) override
	{
		COutputStreamDecorator::WriteByte(m_encryptor.Encrypt(data));
	}
	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		const uint8_t* buff = static_cast<const uint8_t*>(srcData);
		std::vector<uint8_t> encodedBuff(static_cast<size_t>(size));

		for (size_t i = 0; i < encodedBuff.size(); ++i)
		{
			encodedBuff[i] = m_encryptor.Encrypt(*(buff + i));
		}

		COutputStreamDecorator::WriteBlock(encodedBuff.data(), size);
	}

private:
	EncryptTable m_encryptor;
};