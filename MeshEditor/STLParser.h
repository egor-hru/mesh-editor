#ifndef STL_PARSER_H
#define STL_PARSER_H

#include <vector>
#include <string>

struct Vec { double x, y, z; };

struct Triangle
{
    Vec A, B, C;
    Vec normal;
};

using TriangleSoup = std::vector<Triangle>;

class STLParser
{
public:
    TriangleSoup read(const std::string& filename);
    void write(const TriangleSoup& triangleSoup, const std::string& filename);
private:
    // TODO
};

#endif // STL_PARSER_H
