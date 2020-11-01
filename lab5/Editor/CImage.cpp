#include "CImage.h"
#include "ICommand.h"
#include "CResizeImageCommand.h"
#include <random>

const std::string JPG = ".jpg";
const std::string PNG = ".png";
const std::string SVG = ".svg";
const std::string DIRECTORY_PATH = "image";
const int MAX_IMAGE_SIZE = 10000;
const int MIN_IMAGE_SIZE = 1;

CImage::CImage(CommandFunc commandFunc, const Path& path, int width, int height)
    :m_commandFunc(commandFunc)
{
    if (!std::filesystem::exists(path) || !IsImage(path))
    {
        throw std::invalid_argument("invalid image path");
    }

    if (width < MIN_IMAGE_SIZE || height < MIN_IMAGE_SIZE || height > MAX_IMAGE_SIZE || width > MAX_IMAGE_SIZE)
    {
        throw std::invalid_argument("invalid expression sizes");
    }

    m_width = width;
    m_height = height;

    std::string tempPath =  DIRECTORY_PATH + "/" + GenerateTemporaryFileName() + path.extension().string();
    if (!std::filesystem::is_directory(DIRECTORY_PATH))
    {
        std::filesystem::create_directory(DIRECTORY_PATH);
    }
    std::filesystem::copy_file(path, tempPath);
    m_path = tempPath;
}

CImage::~CImage()
{
    std::filesystem::remove(m_path);
}

Path CImage::GetPath() const
{
    return m_path;
}

int CImage::GetWidth() const
{
    return m_width;
}

int CImage::GetHeight() const
{
    return m_height;
}

void CImage::Resize(int width, int height)
{
    std::unique_ptr<ICommand> commandPtr = std::make_unique<CResizeImageCommand>(m_width, m_height, width, height);
    m_commandFunc(std::move(commandPtr));
}

const size_t TMP_FILENAME_SIZE = 10;

std::string CImage::GenerateTemporaryFileName()
{
    std::mt19937 generator{ std::random_device{}() };
    std::uniform_int_distribution<int> distribution{ 'a', 'z' };

    std::string resultName(TMP_FILENAME_SIZE, '\0');
    for (auto& ch : resultName)
    {
        ch = static_cast<char>(distribution(generator));
    }

    return resultName;
}

bool CImage::IsImage(Path const& path)
{
    auto extension = path.extension();
    if (extension == JPG || extension == PNG || extension == SVG)
    {
        return true;
    }
    return false;
}