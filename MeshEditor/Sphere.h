#pragma once

#include "Command.h"
#include "STLParser.h"


class Sphere : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    void makeVertices();
    void setOrigin(std::vector<double> ori);
    Vec makeNormal3Vector(Vec A, Vec B, Vec C);
    Vec normalize3Vector(Vec vextor);
    float length3Vector(const Vec vector);
    float dotProduct(Vec A, Vec B);



    TriangleSoup triangles;
    Vec origin;
    double radius;
    unsigned int slices;
    unsigned int rings;
    const std::string name = "Sphere";
};


// MeshEditor.exe Sphere R = 10.0, origin = (4.5,3.4,2.1), slices = 3, rings = 4, filename = "D:\Programming\cpp\STL\sphere.stl"
