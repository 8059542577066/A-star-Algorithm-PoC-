#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>


#ifndef INVALID_ROUTE_1
#define INVALID_ROUTE_1 11
#endif

#ifndef INVALID_ROUTE_2
#define INVALID_ROUTE_2 12
#endif

#ifndef NEGATIVE_WEIGHT
#define NEGATIVE_WEIGHT 21
#endif

#ifndef INCREASED_WEIGHT
#define INCREASED_WEIGHT 22
#endif


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
    bool found;
    double costs;
    double angle;
    double score;

    Node(std::size_t);

    bool operator<(const Node &) const;
    struct MinHeap
    {
        bool operator()(const Node *,
                        const Node *) const;
    };
};


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
    void copy(std::vector<std::size_t> &,
              std::vector<Point> &) const;
    bool find(std::size_t) const;
    std::size_t size() const;
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
    void copy(std::vector<std::size_t> &,
              std::size_t) const;
    void copy(std::vector<Route> &,
              std::vector<double> &) const;
    std::size_t size() const;
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

    void markID(std::size_t);
    void update(std::size_t, std::size_t);
    bool check(std::size_t, std::size_t) const;
    bool isNew(std::size_t) const;
    std::size_t getPrevID(std::size_t) const;
    double getCosts(std::size_t) const;
    double getScore(std::size_t) const;

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
    void copy(std::vector<std::size_t> &,
              std::vector<Point> &,
              std::vector<Route> &,
              std::vector<double> &) const;
    void initialize(std::size_t, std::size_t);
    void findPath();
    std::size_t countPoints() const;
    std::size_t countRoutes() const;
    std::vector<std::size_t> getPath() const;
    double getCosts() const;
};


#endif