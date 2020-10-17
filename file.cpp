#include "file.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>


void insertPoints(const std::string &fileName,
                  Graph &graph)
{
    std::ifstream fin(fileName.c_str());

    if (fin.fail())
        throw FILE_NOT_FOUND;

    std::string strID, strPhi, strTheta;
    std::stringstream ss;
    std::size_t ID;
    double phi, theta;

    while (!std::getline(fin, strID, ',').eof())
    {
        std::getline(fin, strPhi, ',');
        std::getline(fin, strTheta);
        ss << strID, ss >> ID, ss.str(""), ss.clear();
        ss << strPhi, ss >> phi, ss.str(""), ss.clear();
        ss << strTheta, ss >> theta, ss.str(""), ss.clear();
        graph.insertPoint(ID, phi, theta);
    }

    fin.close();
}

void erasePoints(const std::string &fileName,
                 Graph &graph)
{
    std::ifstream fin(fileName.c_str());

    if (fin.fail())
        throw FILE_NOT_FOUND;

    std::string strID;
    std::stringstream ss;
    std::size_t ID;

    while (!std::getline(fin, strID).eof())
    {
        ss << strID, ss >> ID, ss.str(""), ss.clear();
        graph.erasePoint(ID);
    }

    fin.close();
}

void insertRoutes(const std::string &fileName,
                  Graph &graph)
{
    std::ifstream fin(fileName.c_str());

    if (fin.fail())
        throw FILE_NOT_FOUND;

    std::ofstream ferr("errors.txt");
    std::string strSrcID, strDstID, strCost;
    std::stringstream ss;
    std::size_t srcID, dstID;
    double cost;

    while (!std::getline(fin, strSrcID, ',').eof())
    {
        std::getline(fin, strDstID, ',');
        std::getline(fin, strCost);
        ss << strSrcID, ss >> srcID, ss.str(""), ss.clear();
        ss << strDstID, ss >> dstID, ss.str(""), ss.clear();
        ss << strCost, ss >> cost, ss.str(""), ss.clear();

        try
        {
            graph.insertRoute(srcID, dstID, cost);
        }
        catch (int e)
        {
            if (e == INVALID_ROUTE_1)
                ferr << "Src & Dst should be different: ("
                     << srcID << ")" << std::endl;
            else if (e == INVALID_ROUTE_2)
                ferr << "Route cost can't be less than distance: ("
                     << srcID << "," << dstID << ")" << std::endl;
        }
    }

    fin.close();
    ferr.close();
}

void eraseRoutes(const std::string &fileName,
                 Graph &graph)
{
    std::ifstream fin(fileName.c_str());

    if (fin.fail())
        throw FILE_NOT_FOUND;

    std::string strSrcID, strDstID;
    std::stringstream ss;
    std::size_t srcID, dstID;

    while (!std::getline(fin, strSrcID, ',').eof())
    {
        std::getline(fin, strDstID);
        ss << strSrcID, ss >> srcID, ss.str(""), ss.clear();
        ss << strDstID, ss >> dstID, ss.str(""), ss.clear();
        graph.eraseRoute(srcID, dstID);
    }

    fin.close();
}

void backup(const Graph &graph)
{
    std::vector<std::size_t> IDs;
    std::vector<Point> points;
    std::vector<Route> routes;
    std::vector<double> costs;
    graph.copy(IDs, points, routes, costs);

    {
        std::stringstream ss;
        ss << "points(" << graph.countPoints() << ").txt";
        std::string fileName;
        ss >> fileName;
        std::ofstream fout(fileName.c_str());

        for (std::size_t i = 0; i < IDs.size(); ++i)
        {
            fout << IDs[i] << ",";
            fout << std::acos(points[i].unitZ) << ",";
            fout << std::atan2(points[i].unitY,
                               points[i].unitX)
                 << std::endl;
        }

        fout.close();
    }

    {
        std::stringstream ss;
        ss << "routes(" << graph.countRoutes() << ").txt";
        std::string fileName;
        ss >> fileName;
        std::ofstream fout(fileName.c_str());

        for (std::size_t i = 0; i < routes.size(); ++i)
        {
            fout << routes[i].srcID << ",";
            fout << routes[i].dstID << ",";
            fout << costs[i] << std::endl;
        }

        fout.close();
    }
}

void savePath(const Graph &graph,
              const std::string &fileName)
{
    std::vector<std::size_t> path = graph.getPath();
    std::ofstream fout(fileName.c_str());

    for (std::size_t i = 0; i < path.size(); ++i)
        fout << path[i] << std::endl;

    fout << std::setprecision(15)
         << graph.getCosts() << std::endl;
    fout.close();
}