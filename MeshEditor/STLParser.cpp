#include "STLParser.h"

#include <iostream>
#include <fstream>
using namespace std;

TriangleSoup STLParser::read(const std::string& filename)
{

    ifstream fileSTL;
    fileSTL.exceptions(ifstream::badbit | ifstream::failbit);

    try
    {
        fileSTL.open(filename);
    }
    catch (const std::exception & ex)
    {
        cout << ex.what() << endl;
        cout << "File wasn't opened" << endl;
    }


    TriangleSoup model;
    string str;

    while (!fileSTL.eof())
    {
        Triangle triangle;
        str = "";
        fileSTL >> str;

        if (str == "facet")
        {
            fileSTL >> str;
            fileSTL >> str;
            triangle.normal.x = std::stod(str);

            fileSTL >> str;
            triangle.normal.y = std::stod(str);

            fileSTL >> str;
            triangle.normal.z = std::stod(str);

            string tmp;
            fileSTL >> str;
            tmp = str;
            fileSTL >> str;
            tmp += " " + str;
            if (tmp == "outer loop")
            {
                fileSTL >> str;
                if (str == "vertex")
                {
                    fileSTL >> str;
                    triangle.A.x = std::stod(str);

                    fileSTL >> str;
                    triangle.A.y = std::stod(str);

                    fileSTL >> str;
                    triangle.A.z = std::stod(str);
                }

                fileSTL >> str;
                if (str == "vertex")
                {
                    fileSTL >> str;
                    triangle.B.x = std::stod(str);

                    fileSTL >> str;
                    triangle.B.y = std::stod(str);

                    fileSTL >> str;
                    triangle.B.z = std::stod(str);
                }

                fileSTL >> str;
                if (str == "vertex")
                {
                    fileSTL >> str;
                    triangle.C.x = std::stod(str);

                    fileSTL >> str;
                    triangle.C.y = std::stod(str);

                    fileSTL >> str;
                    triangle.C.z = std::stod(str);
                }

                fileSTL >> str;
                if (str == "endloop")
                {
                    fileSTL >> str;
                    if (str == "endfacet")
                    {
                        model.push_back(triangle);
                    }
                }
            }
        }
    }
    fileSTL.close();

    return model;
}

void STLParser::write(const TriangleSoup& triangleSoup, const std::string& filename)
{
    std::ofstream file;
    file.open(filename);

    std::string temp;
    unsigned int start = filename.find(".stl");
    for (size_t i = 0; i < start; i++)
    {
        if (filename.at(start - i) != '\\' && filename.at(start - i) != '/')
            temp = filename.substr(start - i, i);
        else
            break;
    }

    file << "solid " << temp << '\n';

    for (size_t i = 0; i < triangleSoup.size(); i++)
    {
        file << "\tfacet " << "normal " << triangleSoup[i].normal.x << " " <<
            triangleSoup[i].normal.y << " " << triangleSoup[i].normal.z << '\n';

        file << "\t\touter loop\n";

        file << "\t\t\tvertex " << triangleSoup[i].A.x << " " <<
            triangleSoup[i].A.y << " " << triangleSoup[i].A.z << '\n';
        file << "\t\t\tvertex " << triangleSoup[i].B.x << " " <<
            triangleSoup[i].B.y << " " << triangleSoup[i].B.z << '\n';
        file << "\t\t\tvertex " << triangleSoup[i].C.x << " " <<
            triangleSoup[i].C.y << " " << triangleSoup[i].C.z << '\n';
        file << "\t\tendloop\n";
        file << "\tendfacet\n";

    }
    file << "endsolid " << temp;
    file.close();
}