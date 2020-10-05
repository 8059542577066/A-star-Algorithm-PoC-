#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <set>


struct Point
{
    double unitX;
    double unitY;
    double unitZ;

    Point();
    Point(const Point &);
    Point(double, double);

    double operator*(const Point &) const;
    double operator^(const Point &) const;
};


struct Route
{
    std::size_t srcID;
    std::size_t dstID;

    Route(std::size_t, std::size_t);

    bool operator<(const Route &) const;
    struct OrderByDstID
    {
        bool operator()(const Route &,
                        const Route &) const;
    };
};


struct Node
{
    std::size_t currID;
    std::size_t prevID;
    double costs;
    double dist;
    double score;

    Node(std::size_t);

    bool operator<(const Node &) const;
    struct MinHeap
    {
        bool operator()(const Node *,
                        const Node *) const;
    };
};


class Graph;

class Points
{
    std::map<std::size_t, Point> points;

public:
    Points();

    Point operator[](std::size_t) const;

    void insert(std::size_t,
                double, double);
    void erase(std::size_t);
    void clear();
    void copy(std::vector<Node> &) const;
    bool find(std::size_t) const;
    std::size_t size() const;

    friend void backup(const Graph &);
};


class Routes
{
    std::map<Route, double> routesBySrcID;
    std::map<Route, double,
             Route::OrderByDstID>
        routesByDstID;

public:
    Routes();

    double operator[](const Route &) const;

    void insert(std::size_t, std::size_t,
                double);
    void erase(std::size_t, std::size_t);
    void erase(std::size_t);
    void clear();
    void copyBySrc(std::vector<std::size_t> &,
                   std::size_t) const;
    std::size_t size() const;

    friend void backup(const Graph &);
};


class Nodes
{
    std::vector<Node> nodes;

public:
    Nodes();
    Nodes(const Points &,
          std::size_t, std::size_t);

    Node &operator[](std::size_t);
    Node operator[](std::size_t) const;

    void clear();
    std::size_t size() const;
    std::size_t find(std::size_t) const;
};


class Graph
{
    bool ready;
    bool done;
    double weight;
    std::size_t dstID;
    Points points;
    Routes routes;
    Nodes nodes;
    std::vector<const Node *> scoreHeap;
    std::set<std::size_t> discovered;

    void update(std::size_t, std::size_t);
    bool isNew(std::size_t) const;
    std::size_t getPrevID(std::size_t) const;
    double getCosts(std::size_t) const;

public:
    Graph(double);

    void updateWeight(double);
    void insertPoint(std::size_t,
                     double, double);
    void erasePoint(std::size_t);
    void clearPoints();
    void insertRoute(std::size_t, std::size_t,
                     double);
    void eraseRoute(std::size_t, std::size_t);
    void clearRoutes();
    void initialize(std::size_t, std::size_t);
    void findPath();
    std::size_t countPoints() const;
    std::size_t countRoutes() const;
    std::vector<std::size_t> getPath() const;
    double getCosts() const;

    friend void backup(const Graph &);
};


#endif