#pragma once
#include <vector>
#include <numeric>
#include <random>

class EncryptTable
{
public:
	EncryptTable(uint32_t key)
		: encodeArr(UINT8_MAX + 1)
		, decodeArr(encodeArr.size())
	{
		std::iota(encodeArr.begin(), encodeArr.end(), '\0');
		std::shuffle(encodeArr.begin(), encodeArr.end(), std::mt19937(key));
		for (size_t i = 0; i < encodeArr.size(); ++i)
		{
			decodeArr[encodeArr[i]] = static_cast<uint8_t>(i);
		}
	}

	uint8_t Encrypt(uint8_t ch)
	{
		return encodeArr[ch];
	}

	uint8_t Decrypt(uint8_t ch)
	{
		return decodeArr[ch];
	}

private:
	std::vector<uint8_t> encodeArr;
	std::vector<uint8_t> decodeArr;
};