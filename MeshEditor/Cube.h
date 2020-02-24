#pragma once

#include "Command.h"
#include "STLParser.h"


class Cube : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    void makeVerteces(const std::map<std::string, std::string>& args);
    void setOrigin(std::vector<double> ori);
    Vec makeNormal3Vector(Vec point1, Vec point2, Vec point3);
    Vec normalize3Vector(Vec vextor);
    float length3Vector(const Vec vector);


    TriangleSoup vertices;
    Vec origin;
    double length;
    const std::string name = "Cube";
};

//  MeshEditor.exe Cube L = 10, origin = (4.2, 0.0, 1.0), filepath = D:\Programming\STL\block.stl"