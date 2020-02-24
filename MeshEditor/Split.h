#include "Command.h"
#include "STLParser.h"

#include <list>

class Split : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    Vec make3Vector(const Vec point1, const Vec point2);
    void setOrigin(std::vector<double> ori);
    void setDirection(std::vector<double> dir);
    double length3Vector(const Vec vector);
    void addTriangle(Triangle triangle);
    Vec findIntersectionPoint(Vec point1, Vec point2);
    void triangulationEarClipping(TriangleSoup figure);
    std::list<Vec> findPointsOnPlane(TriangleSoup figure);
    double lengthFromPointToPlane(const Vec point);

    inline void fig1OrFig2(bool condition, Triangle);

    TriangleSoup figure1, figure2;
    Vec origin, direction;
    Vec normal;
    std::vector<std::vector<Vec>> linesOnPlane;
    const std::string name = "Split";
};

//  MeshEditor.exe Split input = "D:\Programming\cpp\STL\block.stl", origin = (0,0,0), direction = (0,0,1), output1 = "D:\Programming\cpp\STL\A.stl", output2 = "D:\Programming\cpp\STL\B.stl"
//  MeshEditor.exe Split input = "D:\Programming\cpp\STL\sphere.stl", origin = (0,0,0), direction = (0,1,1), output1 = "D:\Programming\cpp\STL\A.stl", output2 = "D:\Programming\cpp\STL\B.stl"