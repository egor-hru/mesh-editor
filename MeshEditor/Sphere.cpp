#include "Sphere.h"

#include <iostream>
#include <fstream>

const std::string& Sphere::getName() const
{
    return this->name;
}

int Sphere::execute(const std::map<std::string, std::string>& args)
{
    if (args.count("R") == 0 || args.count("filename") == 0 ||
        args.count("origin") == 0 || args.count("slices") == 0 ||
        args.count("rings") == 0)
    {
        return 4;
    }

    radius = std::stod(args.at("R"));
    if (radius <= 0)
        return 1;

    slices = std::stoul(args.at("slices"));
    rings = std::stoul(args.at("rings"));
    if (slices <= 2 || rings <= 1)
        return 2;

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
    makeVertices();

    std::ofstream file;
    file.open(args.at("filename"));
    if (!file.is_open())
    {
        file.close();
        return 3;
    }
    else
    {
        file.close();
        STLParser stl;
        stl.write(triangles, args.at("filename"));
    }

    return 0;
}

void Sphere::setOrigin(std::vector<double> ori)
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

void Sphere::makeVertices()
{
    std::vector <Vec> vertices;
    const float PI = 3.1415926f;

    Vec vertex;
    float xy;

    float sectorStep = 2 * PI / rings;
    float stackStep = PI / slices;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= slices; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        vertex.z = radius * sinf(stackAngle) + origin.z;              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= rings; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            vertex.x = xy * cosf(sectorAngle) + origin.x;    // r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle) + origin.y;    // r * cos(u) * sin(v)
            vertices.push_back(vertex);
        }
    }


    std::vector<int> indices;
    int k1, k2;
    for (int i = 0; i < slices; ++i)
    {
        k1 = i * (rings + 1);     // beginning of current stack
        k2 = k1 + rings + 1;      // beginning of next stack

        for (int j = 0; j < rings; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (slices - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    std::cout << "Triangle amount: " << indices.size() / 3 << std::endl;
    std::cout << "Vectices amount: " << vertices.size() << std::endl;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Triangle triangle;
        triangle.A.x = vertices[indices[i]].x;
        triangle.A.y = vertices[indices[i]].y;
        triangle.A.z = vertices[indices[i]].z;

        triangle.B.x = vertices[indices[i + 1]].x;
        triangle.B.y = vertices[indices[i + 1]].y;
        triangle.B.z = vertices[indices[i + 1]].z;

        triangle.C.x = vertices[indices[i + 2]].x;
        triangle.C.y = vertices[indices[i + 2]].y;
        triangle.C.z = vertices[indices[i + 2]].z;

        triangle.normal = makeNormal3Vector(triangle.A, triangle.B, triangle.C);
        triangle.normal = normalize3Vector(triangle.normal);

        triangles.push_back(triangle);
    }
}

Vec Sphere::makeNormal3Vector(Vec A, Vec B, Vec C)
{
    Vec AB = {
        B.x - A.x,
        B.y - A.y,
        B.z - A.z
    };

    Vec AC = {
        C.x - A.x,
        C.y - A.y,
        C.z - A.z
    };

    Vec normal = {
        (AB.y * AC.z) - (AB.z * AC.y),
        (AB.z * AC.x) - (AB.x * AC.z),
        (AB.x * AC.y) - (AB.y * AC.x)
    };


    Vec AOr = {
        origin.x - A.x,
        origin.y - A.y,
        origin.z - A.z
    };

    if (dotProduct(normal, AOr) < 0)
    {
        normal.x *= -1;
        normal.y *= -1;
        normal.z *= -1;
    }

    return normal;
}

Vec Sphere::normalize3Vector(Vec vector)
{
    float locLength = length3Vector(vector);
    float inv_length = (1 / locLength);

    vector.x *= inv_length;
    vector.y *= inv_length;
    vector.z *= inv_length;

    return vector;
}

float Sphere::length3Vector(const Vec vector)
{
    return (float)sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

float Sphere::dotProduct(Vec A, Vec B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}