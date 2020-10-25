#include "CShapeFactory.h"
#include <stdexcept>
#include <sstream>
#include <algorithm>

const std::string TRIANGLE_NAME = "triangle";
const std::string RECTANGLE_NAME = "rectangle";
const std::string ELLIPSE_NAME = "ellipse";
const std::string POLYGON_NAME = "polygon";

template<typename T>
T GetParam(std::istream& argsStream)
{
	T param;
	if (!(argsStream >> param))
	{
		throw std::logic_error("Not enough arguments");
	}
	return param;
}

Color GetColor(std::istream& argsStream)
{
	std::string param;
	if (!(argsStream >> param))
	{
		throw std::logic_error("Not enough arguments");
	}
	
	std::transform(param.begin(), param.end(), param.begin(), [](char ch) { return std::tolower(ch); });
	if (param == "green")
	{
		return Color::Green;
	}
	else if (param == "red")
	{
		return Color::Red;
	}
	else if (param == "blue")
	{
		return Color::Blue;
	}
	else if (param == "yellow")
	{
		return Color::Yellow;
	}
	else if (param == "pink")
	{
		return Color::Pink;
	}
	else if (param == "black")
	{
		return Color::Black;
	}

	throw std::invalid_argument("");
}

std::shared_ptr<CTriangle> MakeTriangle(std::istream& argsStream)
{
	Color color = GetColor(argsStream);
	Point vertex1 = { GetParam<double>(argsStream), GetParam<double>(argsStream) };
	Point vertex2 = { GetParam<double>(argsStream), GetParam<double>(argsStream) };
	Point vertex3 = { GetParam<double>(argsStream), GetParam<double>(argsStream) };

	return std::make_shared<CTriangle>(color, vertex1, vertex2, vertex3);
}

std::shared_ptr<CRectangle> MakeRectangle(std::istream& argsStream)
{
	Color color = GetColor(argsStream);
	Point leftTop = { GetParam<double>(argsStream), GetParam<double>(argsStream) };
	Point rightBottom = { GetParam<double>(argsStream), GetParam<double>(argsStream) };

	return std::make_shared<CRectangle>(color, leftTop, rightBottom);
}

std::shared_ptr<CRegularPolygon> MakePolygon(std::istream& argsStream)
{
	Color color = GetColor(argsStream);
	size_t vertexCount = GetParam<size_t>(argsStream);
	Point center = { GetParam<double>(argsStream), GetParam<double>(argsStream) };
	double radius = GetParam<double>(argsStream);

	return std::make_shared<CRegularPolygon>(color, vertexCount, center, radius);
}

std::shared_ptr<CEllipse> MakeEllipse(std::istream& argsStream)
{
	Color color = GetColor(argsStream);
	Point leftTop = { GetParam<double>(argsStream), GetParam<double>(argsStream) };
	double width = GetParam<double>(argsStream);
	double height = GetParam<double>(argsStream);

	return std::make_shared<CEllipse>(color, leftTop, width, height);
}

CShapeFactory::CShapeFactory()
{
	m_shapeCreationMap[TRIANGLE_NAME] = MakeTriangle;
	m_shapeCreationMap[RECTANGLE_NAME] = MakeRectangle;
	m_shapeCreationMap[ELLIPSE_NAME] = MakeEllipse;
	m_shapeCreationMap[POLYGON_NAME] = MakePolygon;
}

std::shared_ptr<CShape> CShapeFactory::CreateShape(const std::string& description)
{
	std::istringstream args(description);
	std::string shapeName;

	try
	{
		args >> shapeName;
		auto shapeIt = m_shapeCreationMap.find(shapeName);
		if (shapeIt == m_shapeCreationMap.end())
		{
			throw std::invalid_argument("Unhandled shape: " + shapeName + " given");
		}

		auto shape = shapeIt->second(args);
		if (!args.eof())
		{
			throw std::logic_error("Overabundance of arguments");
		}
		return shape;
	}
	catch (const std::exception& ex)
	{
		throw std::invalid_argument("Error while parsing " + shapeName + ". " + ex.what());
	}
}
