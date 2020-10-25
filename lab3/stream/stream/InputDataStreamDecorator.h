#pragma once
#include "EncryptTable.h"
#include "IInputDataStream.h"

class CInputStreamDecorator : public IInputDataStream
{
public:
	virtual ~CInputStreamDecorator() = default;

	virtual bool IsEOF() const override
	{
		return m_stream->IsEOF();
	}
	virtual uint8_t ReadByte() override
	{
		return m_stream->ReadByte();
	}
	virtual std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		return m_stream->ReadBlock(dstBuffer, size);
	}

protected:
	CInputStreamDecorator(std::unique_ptr<IInputDataStream>&& stream)
		: m_stream(std::move(stream))
	{}

private:
	std::unique_ptr<IInputDataStream> m_stream;
};

class CDecompressInputStreamDecorator : public CInputStreamDecorator
{
public:
	CDecompressInputStreamDecorator(std::unique_ptr<IInputDataStream>&& stream)
		: CInputStreamDecorator(std::move(stream))
	{
	}

	bool IsEOF() const override
	{
		return (m_count == 0) && CInputStreamDecorator::IsEOF();
	}

	uint8_t ReadByte() override
	{
		if (m_count == 0)
		{
			m_count = CInputStreamDecorator::ReadByte();
			if (!CInputStreamDecorator::IsEOF())
			{
				m_symbol = CInputStreamDecorator::ReadByte();
			}
			else
			{
				m_count = 0;
				m_symbol = '\0';
			}
		}
		if (m_count != 0)
		{
			m_count--;
		}
		return (m_symbol);
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		uint8_t* buff = static_cast<uint8_t*>(dstBuffer);

		for (std::streamsize i = 0; i < size; ++i)
		{
			try
			{
				*(buff + i) = ReadByte();
			}
			catch (const std::exception&)
			{
				return i;
			}

			//if (IsEOF())
			//{
			//	return i;
			//}
			//*(buff + i) = ReadByte();
		}
		return size;
	}

private:
	uint8_t m_symbol = '\0';
	size_t m_count = 0;
};

class CDecryptInputStreamDecorator : public CInputStreamDecorator
{
public:
	CDecryptInputStreamDecorator(std::unique_ptr<IInputDataStream>&& stream, uint32_t key)
		:CInputStreamDecorator(std::move(stream)),
		m_encryptor(key)
	{
	}

	uint8_t ReadByte() override
	{
		return m_encryptor.Decrypt(CInputStreamDecorator::ReadByte());
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		std::streamsize newSize = CInputStreamDecorator::ReadBlock(dstBuffer, size);
		uint8_t* buff = static_cast<uint8_t*>(dstBuffer);

		for (auto i = 0; i < newSize; ++i)
		{
			uint8_t ch = *(buff + i);
			*(buff + i) = m_encryptor.Decrypt(ch);
		}
		return newSize;
	}

private:
	EncryptTable m_encryptor;
};