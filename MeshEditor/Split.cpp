#include "Split.h"

#include <iostream>
#include <fstream>
#include <list>
#include <cmath>

const std::string& Split::getName() const
{
    return this->name;
}

int Split::execute(const std::map<std::string, std::string>& args)
{
    if (args.count("input") == 0 || args.count("direction") == 0 ||
        args.count("origin") == 0 || args.count("output1") == 0 ||
        args.count("output2") == 0)
    {
        return 3;
    }

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

    std::vector<double> direction;
    std::string directionText = args.at("direction");
    if (directionText[0] == '(')
        directionText.replace(0, 1, "");

    temp = "";
    for (std::string::iterator it = directionText.begin(); it != directionText.end(); ++it)
    {
        temp += *it;

        if (*it == ',' || *it == ')')
        {
            direction.push_back(std::stod(temp));
            temp = "";
        }
    }


    setOrigin(origin);
    setDirection(direction);
    normal = {
        this->origin.x + this->direction.x,
        this->origin.y + this->direction.y,
        this->origin.z + this->direction.z
    };

    if (length3Vector(normal) <= 0)
        return 1;

    STLParser stlFile;
    TriangleSoup figure = stlFile.read(args.at("input"));
    for (auto n : figure)
        addTriangle(n);

    triangulationEarClipping(figure1);


    std::ofstream output1File;
    output1File.open(args.at("output1"));
    if (!output1File.is_open())
    {
        output1File.close();
        return 2;
    }
    else
    {
        output1File.close();
        STLParser stl1;
        stl1.write(figure1, args.at("output1"));
    }

    std::ofstream output2File;
    output2File.open(args.at("output2"));
    if (!output2File.is_open())
    {
        output2File.close();
        return 2;
    }
    else
    {
        output2File.close();
        STLParser stl2;
        stl2.write(figure2, args.at("output2"));
    }

    return 0;
}

Vec Split::make3Vector(const Vec point1, const Vec point2)
{
    return {
        point2.x - point1.x,
        point2.y - point1.y,
        point2.z - point1.z,
    };
}

void Split::setOrigin(std::vector<double> ori)
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

void Split::setDirection(std::vector<double> dir)
{
    if (dir.size() == 3)
    {
        direction.x = dir[0];
        direction.y = dir[1];
        direction.z = dir[2];
    }
    else
    {
        throw "Vector has to have only 3 elements!";
    }
}

double Split::length3Vector(const Vec vector)
{
    return (float)sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

double Split::lengthFromPointToPlane(const Vec point)
{
    return (normal.x * point.x + normal.y * point.y + normal.z * point.z
        - (normal.x * origin.x + normal.y * origin.y + normal.z * origin.z)) /
        length3Vector(normal);
}

void Split::fig1OrFig2(bool condition, Triangle tr)
{
    if (condition)
        figure1.push_back(tr);
    else
        figure2.push_back(tr);
}

void show(Vec obj)
{
    std::cout << "X:" << obj.x << " Y:" << obj.y << " Z:" << obj.z << std::endl;
}

void Split::addTriangle(Triangle triangle)
{
    double lenA = lengthFromPointToPlane(triangle.A);
    double lenB = lengthFromPointToPlane(triangle.B);
    double lenC = lengthFromPointToPlane(triangle.C);

    if (lenA <= 0 && lenB <= 0 && lenC <= 0)
    {
        if (lenA == 0 && lenB == 0)
        {
            std::vector<Vec> line;
            line.push_back(triangle.A);
            line.push_back(triangle.B);
            linesOnPlane.push_back(line);
        }
        else if (lenA == 0 && lenC == 0)
        {
            std::vector<Vec> line;
            line.push_back(triangle.A);
            line.push_back(triangle.C);
            linesOnPlane.push_back(line);
        }
        else if (lenB == 0 && lenC == 0)
        {
            std::vector<Vec> line;
            line.push_back(triangle.B);
            line.push_back(triangle.C);
            linesOnPlane.push_back(line);
        }

        figure1.push_back(triangle);
        return;
    }
    else if (lenA >= 0 && lenB >= 0 && lenC >= 0)
    {
        figure2.push_back(triangle);
        return;
    }
    else if ((lenA > 0 && lenB > 0 && lenC < 0) || (lenA < 0 && lenB < 0 && lenC > 0))
    {
        //       A    line      -->              A
        //		/ \    /		--> 			/|\    
        //	   /   \  /			--> 		   / | \  
        //	  /     \/          --> 		  /  |2 \  
        //	 /	    /\			--> 		 / 1 | /3\
		//  /______/__\			--> 	    /____|/___\
		// B      /    C		--> 	   B           C

        Triangle tr;
        tr.normal = triangle.normal;
        //   1
        tr.A = triangle.A;
        tr.B = triangle.B;
        tr.C = findIntersectionPoint(triangle.B, triangle.C);
        fig1OrFig2(lenC > 0, tr);

        // 2
        tr.B = tr.C;
        tr.C = findIntersectionPoint(triangle.A, triangle.C);
        fig1OrFig2(lenC > 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(tr.B);
        line.push_back(tr.C);
        linesOnPlane.push_back(line);

        // 3
        tr.A = tr.C;
        tr.C = triangle.C;
        fig1OrFig2(lenC < 0, tr);
        return;
    }
    else if ((lenA > 0 && lenB < 0 && lenC > 0) || (lenA < 0 && lenB > 0 && lenC < 0))
    {
        //       A    line      -->              A
        //		/ \    /		--> 			/|\    
        //	   /   \  /			--> 		   / | \  
        //	  /     \/          --> 		  /  |2 \  
        //	 /	    /\			--> 		 / 1 | /3\
		//  /______/__\			--> 	    /____|/___\
		// C      /    B		--> 	   C           B
        Triangle tr;
        tr.normal = triangle.normal;


        //   1
        tr.A = triangle.A;
        tr.B = findIntersectionPoint(triangle.C, triangle.B);
        tr.C = triangle.C;
        fig1OrFig2(lenB > 0, tr);

        // 2
        tr.C = tr.B;
        tr.B = findIntersectionPoint(triangle.A, triangle.B);
        fig1OrFig2(lenB > 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(tr.B);
        line.push_back(tr.C);
        linesOnPlane.push_back(line);

        // 3
        tr.A = tr.C;
        tr.C = triangle.B;
        fig1OrFig2(lenB < 0, tr);
        return;
    }
    else if ((lenA > 0 && lenB < 0 && lenC < 0) || (lenA < 0 && lenB > 0 && lenC > 0))
    {
        //       B    line      -->              B
        //		/ \    /		--> 			/|\    
        //	   /   \  /			--> 		   / | \  
        //	  /     \/          --> 		  /  |2 \  
        //	 /	    /\			--> 		 / 1 | /3\
		//  /______/__\			--> 	    /____|/___\
		// C      /    A		--> 	   C           A


        Triangle tr;
        tr.normal = triangle.normal;

        //   1
        tr.A = triangle.B;
        tr.B = triangle.C;
        tr.C = findIntersectionPoint(triangle.C, triangle.A);
        fig1OrFig2(lenA > 0, tr);

        // 2
        tr.B = tr.C;
        tr.C = findIntersectionPoint(triangle.B, triangle.A);
        fig1OrFig2(lenA > 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(tr.B);
        line.push_back(tr.C);
        linesOnPlane.push_back(line);

        // 3
        tr.A = tr.C;
        tr.C = triangle.A;
        fig1OrFig2(lenA < 0, tr);
        return;
    }
    else if (lenA == 0 && lenB != 0 && lenC != 0)
    {
        //       A          
        //		/|\    		
        //	   / | \ 		
        //	  /  |  \       
        //	 / 1 | 2 \		
        //  /____|____\		
        // B     |     C	
        //      line
        Triangle tr;
        tr.normal = triangle.normal;

        // 1
        tr.A = triangle.A;
        tr.B = triangle.B;
        tr.C = findIntersectionPoint(triangle.B, triangle.C);
        fig1OrFig2(lenB < 0, tr);

        // 2
        tr.B = tr.C;
        tr.C = triangle.C;
        fig1OrFig2(lenB > 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(triangle.A);
        line.push_back(tr.B);
        linesOnPlane.push_back(line);
        return;
    }
    else if (lenA != 0 && lenB == 0 && lenC != 0)
    {
        //       B          
        //		/|\    		
        //	   / | \ 		
        //	  /  |  \       
        //	 / 1 | 2 \		
        //  /____|____\		
        // C     |     A	
        //      line
        Triangle tr;
        tr.normal = triangle.normal;

        // 1
        tr.A = triangle.B;
        tr.B = triangle.C;
        tr.C = findIntersectionPoint(triangle.C, triangle.A);
        fig1OrFig2(lenA > 0, tr);

        // 2
        tr.B = tr.C;
        tr.C = triangle.A;
        fig1OrFig2(lenA < 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(triangle.B);
        line.push_back(tr.B);
        linesOnPlane.push_back(line);
        return;
    }
    else if (lenA != 0 && lenB != 0 && lenC == 0)
    {
        //       C          
        //		/|\    		
        //	   / | \ 		
        //	  /  |  \       
        //	 / 1 | 2 \		
        //  /____|____\		
        // A     |     B	
        //      line
        Triangle tr;
        tr.normal = triangle.normal;

        // 1
        tr.A = triangle.C;
        tr.B = triangle.A;
        tr.C = findIntersectionPoint(triangle.A, triangle.B);
        fig1OrFig2(lenA < 0, tr);

        // 2
        tr.B = tr.C;
        tr.C = triangle.B;
        fig1OrFig2(lenA > 0, tr);

        // line
        std::vector<Vec> line;
        line.push_back(triangle.C);
        line.push_back(tr.B);
        linesOnPlane.push_back(line);
        return;
    }
}

Vec Split::findIntersectionPoint(Vec point1, Vec point2)
{
    Vec s = {
        point2.x - point1.x,
        point2.y - point1.y,
        point2.z - point1.z
    };

    double D = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
    double F = point1.x * normal.x + point1.y * normal.y + point1.z * normal.z;
    double K = s.x * normal.x + s.y * normal.y + s.z * normal.z;
    double M = (F + D) / K;

    Vec intersectionPoint = {
        point1.x - s.x * M,
        point1.y - s.y * M,
        point1.z - s.z * M,
    };

    return intersectionPoint;
}

void Split::triangulationEarClipping(TriangleSoup figure)
{
    std::list<Vec> orderedPointsOnPlane;
    orderedPointsOnPlane.push_back(linesOnPlane[0][0]);
    orderedPointsOnPlane.push_back(linesOnPlane[0][1]);
    std::cout << linesOnPlane[0][0].x << " " << linesOnPlane[0][0].y << " " << linesOnPlane[0][0].z << " " << std::endl;
    std::cout << linesOnPlane[0][1].x << " " << linesOnPlane[0][1].y << " " << linesOnPlane[0][1].z << " " << std::endl;

    linesOnPlane.erase(linesOnPlane.begin());
    for (size_t i = 0; i < linesOnPlane.size(); i++)
    {
        std::cout << "--[" << i << "]---" << std::endl;
        std::cout << linesOnPlane[i][0].x << " " << linesOnPlane[i][0].y <<
            " " << linesOnPlane[i][0].z << std::endl;
        std::cout << linesOnPlane[i][1].x << " " << linesOnPlane[i][1].y <<
            " " << linesOnPlane[i][1].z << std::endl;
    }

    std::cout << linesOnPlane.size() << std::endl;
    std::cout << orderedPointsOnPlane.back().x << " " << orderedPointsOnPlane.back().y << " " << orderedPointsOnPlane.back().z << std::endl;

    for (auto it = orderedPointsOnPlane.begin(); it != prev(orderedPointsOnPlane.end()); it++)
    {
        auto it2 = next(it);
        while (it2 != orderedPointsOnPlane.end())
        {
            if (it->x == it2->x && it->y == it2->y && it->z == it2->z)
                it2 = orderedPointsOnPlane.erase(it2);
            else
                ++it2;
        }
    }


    while (linesOnPlane.size() != 0)
    {
        for (size_t j = 0; j < linesOnPlane.size(); j++)
        {
            std::cout << "--------------" << std::endl;
            std::cout << orderedPointsOnPlane.back().x << " " << orderedPointsOnPlane.back().y << " " << orderedPointsOnPlane.back().z << std::endl;
            std::cout << linesOnPlane[j][0].x << " " << linesOnPlane[j][0].y << " " << linesOnPlane[j][0].z << " " << std::endl;
            std::cout << linesOnPlane[j][1].x << " " << linesOnPlane[j][1].y << " " << linesOnPlane[j][1].z << " " << std::endl;

            if (orderedPointsOnPlane.back().x == linesOnPlane[j][0].x && orderedPointsOnPlane.back().y == linesOnPlane[j][0].y && orderedPointsOnPlane.back().z == linesOnPlane[j][0].z)
            {
                std::cout << "+1+" << std::endl;

                orderedPointsOnPlane.push_back(linesOnPlane[j][1]);
                linesOnPlane.erase(linesOnPlane.begin() + j);
                break;
            }
            else if (orderedPointsOnPlane.back().x == linesOnPlane[j][1].x && orderedPointsOnPlane.back().y == linesOnPlane[j][1].y && orderedPointsOnPlane.back().z == linesOnPlane[j][1].z)
            {
                std::cout << "+0+" << std::endl;
                orderedPointsOnPlane.push_back(linesOnPlane[j][0]);
                linesOnPlane.erase(linesOnPlane.begin() + j);
                break;
            }
        }
    }

    do
    {
        Triangle triangle;
        triangle.normal = normal;
        triangle.A = orderedPointsOnPlane.front();

        auto it = orderedPointsOnPlane.begin();
        triangle.B = *(++it);
        triangle.C = *(++it);

        figure2.push_back(triangle);
        orderedPointsOnPlane.erase(--it);

        Vec temp = triangle.A;
        triangle.A = triangle.C;
        triangle.C = temp;
        triangle.normal.x = -normal.x;
        triangle.normal.y = -normal.y;
        triangle.normal.z = -normal.z;
        figure1.push_back(triangle);
    } while (orderedPointsOnPlane.size() != 2);

}
