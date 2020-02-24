#include "Cube.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int Cube::execute(const std::map<std::string, std::string>& args)
{
    if (args.count("L") == 0 || args.count("filepath") == 0 ||
        args.count("origin") == 0)
    {
        return 3;
    }

    length = std::stod(args.at("L"));
    if (length <= 0)
        return 1;

    std::vector<double> origin;

    std::string originText = args.at("origin");
    if (originText[0] == '(')
        originText.replace(0, 1, "");

    std::string temp = "";
    for (std::string::iterator it = originText.begin(); it != originText.end(); ++it)
    {
        temp += *it;

        if (*it == ',' || *it == ')')
        {
            origin.push_back(std::stod(temp));
            temp = "";
        }
    }

    setOrigin(origin);
    makeVerteces(args);

    std::ofstream file;
    file.open(args.at("filepath"));
    if (!file.is_open())
    {
        std::cout << "+++++++++" << std::endl;

        file.close();
        return 2;
    }
    else
    {
        file.close();
        STLParser stl;
        stl.write(vertices, args.at("filepath"));
    }

    return 0;
}

void Cube::makeVerteces(const std::map<std::string, std::string>& args)
{
    const double halfLength = length / 2;

    Triangle triangle;

    //front facet
        // first triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //	top facet
    //	first triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //	back facet
    //	first triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //	bottom facet
    //	first triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //	right facet
    //	first triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x + halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x + halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x + halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //	left facet
    //	first triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y + halfLength;
    triangle.A.z = origin.z + halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y + halfLength;
    triangle.B.z = origin.z - halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y - halfLength;
    triangle.C.z = origin.z - halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);

    //second triangle
    triangle.A.x = origin.x - halfLength;
    triangle.A.y = origin.y - halfLength;
    triangle.A.z = origin.z - halfLength;

    triangle.B.x = origin.x - halfLength;
    triangle.B.y = origin.y - halfLength;
    triangle.B.z = origin.z + halfLength;

    triangle.C.x = origin.x - halfLength;
    triangle.C.y = origin.y + halfLength;
    triangle.C.z = origin.z + halfLength;

    triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
    triangle.normal = normalize3Vector(triangle.normal);
    vertices.push_back(triangle);
}

Vec Cube::makeNormal3Vector(Vec point1, Vec point2, Vec point3)
{
    Vec AB = {
        point2.x - point1.x,
        point2.y - point1.y,
        point2.z - point1.z
    };

    Vec AC = {
    point3.x - point1.x,
    point3.y - point1.y,
    point3.z - point1.z
    };

    Vec normal = {
        (AB.y * AC.z) - (AB.z * AC.y),
        (AB.z * AC.x) - (AB.x * AC.z),
        (AB.x * AC.y) - (AB.y * AC.x)
    };

    return normal;
}

Vec Cube::normalize3Vector(Vec vector)
{
    float locLength = length3Vector(vector);
    float inv_length = (1 / locLength);

    vector.x *= inv_length;
    vector.y *= inv_length;
    vector.z *= inv_length;

    return vector;
}

float Cube::length3Vector(const Vec vector)
{
    return (float)sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

void Cube::setOrigin(std::vector<double> ori)
{
    if (ori.size() == 3)
    {
        origin.x = ori[0];
        origin.y = ori[1];
        origin.z = ori[2];
    }
    else
    {
        throw "Vector has to have only 3 elements!";
    }
}

const std::string& Cube::getName() const
{
    return this->name;
}
