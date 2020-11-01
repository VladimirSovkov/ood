#pragma once
#include "IImage.h"
#include "CommandFunc.h"

class CImage : public IImage
{
public:
	CImage(CommandFunc commandFunc, const Path& path, int width, int height);
	~CImage();

	Path GetPath()const override;
	int GetWidth()const override;
	int GetHeight()const override;
	void Resize(int width, int height) override;

private: 
	bool IsImage(Path const& path);
	std::string GenerateTemporaryFileName();

	CommandFunc m_commandFunc;
	Path m_path;
	int m_width;
	int m_height;
};

