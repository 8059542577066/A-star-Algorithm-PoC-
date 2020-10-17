#include "graph.h"
#include <cmath>
#include <limits>
#include <algorithm>


Point::Point()
{
}

Point::Point(const Point &other)
{
    this->unitX = other.unitX;
    this->unitY = other.unitY;
    this->unitZ = other.unitZ;
}

Point::Point(double phi, double theta)
{
    this->unitX = std::sin(phi) * std::cos(theta);
    this->unitY = std::sin(phi) * std::sin(theta);
    this->unitZ = std::cos(phi);
}

double Point::operator*(const Point &other) const
{
    return this->unitX * other.unitX +
           this->unitY * other.unitY +
           this->unitZ * other.unitZ;
}

double Point::operator^(const Point &other) const
{
    return std::acos(*this * other);
}


Route::Route(std::size_t srcID, std::size_t dstID)
{
    this->srcID = srcID;
    this->dstID = dstID;
}

bool Route::operator<(const Route &other) const
{
    if (this->srcID != other.srcID)
        return this->srcID < other.srcID;

    if (this->dstID != other.dstID)
        return this->dstID < other.dstID;

    return false;
}

bool Route::OrderByDstID::operator()(const Route &a,
                                     const Route &b) const
{
    if (a.dstID != b.dstID)
        return a.dstID < b.dstID;

    if (a.srcID != b.srcID)
        return a.srcID < b.srcID;

    return false;
}


Node::Node(std::size_t currID)
{
    this->currID = currID;
    this->prevID = -1;
    this->found = false;
    this->costs = std::numeric_limits<double>::infinity();
}

bool Node::operator<(const Node &other) const
{
    return this->currID < other.currID;
}

bool Node::MinHeap::operator()(const Node *a,
                               const Node *b) const
{
    return a->score > b->score;
}


Points::Points()
{
}

Point Points::operator[](std::size_t ID) const
{
    return this->points.find(ID)->second;
}

void Points::insert(std::size_t ID,
                    double phi, double theta)
{
    this->points[ID] = Point(phi, theta);
}

void Points::erase(std::size_t ID)
{
    this->points.erase(ID);
}

void Points::clear()
{
    this->points.clear();
}

void Points::copy(std::vector<Node> &nodes) const
{
    nodes.reserve(this->points.size());
    std::map<std::size_t, Point>::const_iterator
        const_lower = this->points.begin(),
        const_upper = this->points.end(),
        const_iter;

    for (const_iter = const_lower;
         const_iter != const_upper; ++const_iter)
        nodes.push_back(Node(const_iter->first));
}

void Points::copy(std::vector<std::size_t> &IDs,
                  std::vector<Point> &points) const
{
    IDs.reserve(this->points.size());
    points.reserve(this->points.size());
    std::map<std::size_t, Point>::const_iterator
        const_lower = this->points.begin(),
        const_upper = this->points.end(),
        const_iter;

    for (const_iter = const_lower;
         const_iter != const_upper; ++const_iter)
    {
        IDs.push_back(const_iter->first);
        points.push_back(const_iter->second);
    }
}

bool Points::find(std::size_t ID) const
{
    return this->points.find(ID) != this->points.end();
}

std::size_t Points::size() const
{
    return this->points.size();
}


Routes::Routes()
{
}

double Routes::operator[](const Route &route) const
{
    return this->routesBySrcID.find(route)->second;
}

void Routes::insert(std::size_t srcID, std::size_t dstID,
                    double cost)
{
    Route route(srcID, dstID);
    this->routesBySrcID[route] = cost;
    this->routesByDstID[route] = cost;
}

void Routes::erase(std::size_t srcID, std::size_t dstID)
{
    Route route(srcID, dstID);
    this->routesBySrcID.erase(route);
    this->routesByDstID.erase(route);
}

void Routes::erase(std::size_t ID)
{
    std::map<Route, double>::iterator
        lowerSrc = this->routesBySrcID.lower_bound(Route(ID, 0)),
        upperSrc = this->routesBySrcID.upper_bound(Route(ID, -1)),
        iterSrc = lowerSrc;

    while (iterSrc != upperSrc)
    {
        this->routesByDstID.erase(iterSrc->first);
        this->routesBySrcID.erase(iterSrc++);
    }

    std::map<Route, double, Route::OrderByDstID>::iterator
        lowerDst = this->routesByDstID.lower_bound(Route(0, ID)),
        upperDst = this->routesByDstID.upper_bound(Route(-1, ID)),
        iterDst = lowerDst;

    while (iterDst != upperDst)
    {
        this->routesBySrcID.erase(iterDst->first);
        this->routesByDstID.erase(iterDst++);
    }
}

void Routes::clear()
{
    this->routesBySrcID.clear();
    this->routesByDstID.clear();
}

void Routes::copy(std::vector<std::size_t> &dstIDs,
                  std::size_t srcID) const
{
    dstIDs.clear();
    std::map<Route, double>::const_iterator
        const_lower = this->routesBySrcID.lower_bound(Route(srcID, 0)),
        const_upper = this->routesBySrcID.upper_bound(Route(srcID, -1)),
        const_iter;

    for (const_iter = const_lower;
         const_iter != const_upper; ++const_iter)
        dstIDs.push_back(const_iter->first.dstID);
}

void Routes::copy(std::vector<Route> &routes,
                  std::vector<double> &costs) const
{
    routes.reserve(this->routesBySrcID.size());
    costs.reserve(this->routesBySrcID.size());
    std::map<Route, double>::const_iterator
        const_lower = this->routesBySrcID.begin(),
        const_upper = this->routesBySrcID.end(),
        const_iter;

    for (const_iter = const_lower;
         const_iter != const_upper; ++const_iter)
    {
        routes.push_back(const_iter->first);
        costs.push_back(const_iter->second);
    }
}

std::size_t Routes::size() const
{
    return this->routesBySrcID.size();
}


Nodes::Nodes()
{
}

Nodes::Nodes(const Points &points,
             std::size_t srcID, std::size_t dstID)
{
    points.copy(this->nodes);
    this->nodes[this->find(srcID)].costs = 0;
    Point dstPoint = points[dstID];

    for (std::size_t i = 0; i < this->nodes.size(); ++i)
        this->nodes[i].angle = points[this->nodes[i].currID] ^ dstPoint;
}

Node &Nodes::operator[](std::size_t index)
{
    return this->nodes[index];
}

Node Nodes::operator[](std::size_t index) const
{
    return this->nodes[index];
}

void Nodes::clear()
{
    this->nodes.clear();
}

std::size_t Nodes::size() const
{
    return this->nodes.size();
}

std::size_t Nodes::find(std::size_t ID) const
{
    return std::lower_bound(this->nodes.begin(),
                            this->nodes.end(), Node(ID)) -
           this->nodes.begin();
}


void Graph::markID(std::size_t ID)
{
    this->nodes[this->nodes.find(ID)].found = true;
}

void Graph::update(std::size_t currID, std::size_t nextID)
{
    double costs = this->getCosts(currID) +
                   this->routes[Route(currID, nextID)];
    Node &nextNode = this->nodes[this->nodes.find(nextID)];

    if (costs < nextNode.costs)
    {
        nextNode.prevID = currID;
        nextNode.costs = costs;
        nextNode.score = costs + this->weight * nextNode.angle;
    }
}

bool Graph::check(std::size_t srcID, std::size_t dstID) const
{
    if (srcID == dstID)
        throw INVALID_ROUTE_1;

    Point srcPoint = this->points[srcID],
          dstPoint = this->points[dstID];

    if (this->routes[Route(srcID, dstID)] <
        this->weight * (srcPoint ^ dstPoint))
        throw INVALID_ROUTE_2;

    return true;
}

bool Graph::isNew(std::size_t ID) const
{
    if (ID == this->dstID)
        return false;

    if (this->nodes[this->nodes.find(ID)].found)
        return false;

    return true;
}

std::size_t Graph::getPrevID(std::size_t ID) const
{
    return this->nodes[this->nodes.find(ID)].prevID;
}

double Graph::getCosts(std::size_t ID) const
{
    return this->nodes[this->nodes.find(ID)].costs;
}

double Graph::getScore(std::size_t ID) const
{
    return this->nodes[this->nodes.find(ID)].score;
}

Graph::Graph(double weight)
{
    this->ready = false;
    this->done = false;
    this->weight = weight;
}

void Graph::updateWeight(double weight)
{
    if (weight < 0)
        throw NEGATIVE_WEIGHT;

    if (weight > this->weight)
    {
        this->weight = weight;
        this->routes.clear();

        throw INCREASED_WEIGHT;
    }

    this->weight = weight;
}

void Graph::insertPoint(std::size_t ID,
                        double phi, double theta)
{
    this->points.insert(ID, phi, theta);
}

void Graph::erasePoint(std::size_t ID)
{
    this->points.erase(ID);
    this->routes.erase(ID);
    this->ready = false;
    this->done = false;
}

void Graph::clearPoints()
{
    this->points.clear();
    this->routes.clear();
    this->ready = false;
    this->done = false;
}

void Graph::insertRoute(std::size_t srcID, std::size_t dstID,
                        double cost)
{
    if (this->points.find(srcID) && this->points.find(dstID) &&
        this->check(srcID, dstID))
    {
        this->routes.insert(srcID, dstID, cost);
        this->ready = false;
        this->done = false;
    }
}

void Graph::eraseRoute(std::size_t srcID, std::size_t dstID)
{
    this->routes.erase(srcID, dstID);
    this->ready = false;
    this->done = false;
}

void Graph::clearRoutes()
{
    this->routes.clear();
    this->ready = false;
    this->done = false;
}

void Graph::copy(std::vector<std::size_t> &IDs,
                 std::vector<Point> &points,
                 std::vector<Route> &routes,
                 std::vector<double> &costs) const
{
    this->points.copy(IDs, points);
    this->routes.copy(routes, costs);
}

void Graph::initialize(std::size_t srcID, std::size_t dstID)
{
    if (this->points.find(srcID) && this->points.find(dstID))
    {
        this->dstID = dstID;
        this->nodes = Nodes(this->points, srcID, dstID);
        this->scoreHeap.clear();
        this->scoreHeap.push_back(
            &this->nodes[this->nodes.find(srcID)]);
        this->ready = true;
    }
    else
    {
        this->ready = false;
        this->done = false;
    }
}

void Graph::findPath()
{
    if (!this->ready)
        return;

    std::vector<std::size_t> dstIDs;

    while (!this->scoreHeap.empty())
    {
        std::size_t currID = this->scoreHeap.front()->currID;
        std::pop_heap(this->scoreHeap.begin(),
                      this->scoreHeap.end(), Node::MinHeap());
        this->scoreHeap.pop_back();

        if (this->getScore(currID) >= this->getCosts(this->dstID))
            break;

        this->markID(currID);
        this->routes.copy(dstIDs, currID);

        for (std::size_t i = 0; i < dstIDs.size(); ++i)
        {
            this->update(currID, dstIDs[i]);

            if (this->isNew(dstIDs[i]))
            {
                this->scoreHeap.push_back(
                    &this->nodes[this->nodes.find(dstIDs[i])]);
                this->markID(dstIDs[i]);
            }
        }

        std::make_heap(this->scoreHeap.begin(),
                       this->scoreHeap.end(), Node::MinHeap());
    }

    this->done = true;
}

std::size_t Graph::countPoints() const
{
    return this->points.size();
}

std::size_t Graph::countRoutes() const
{
    return this->routes.size();
}

std::vector<std::size_t> Graph::getPath() const
{
    std::vector<std::size_t> path;

    if (this->done)
    {
        path.push_back(dstID);
        std::size_t currID = this->dstID,
                    prevID = this->getPrevID(currID);

        while (prevID != (std::size_t)-1)
        {
            currID = prevID;
            path.push_back(currID);
            prevID = this->getPrevID(currID);
        }

        std::reverse(path.begin(), path.end());
    }

    return path;
}

double Graph::getCosts() const
{
    if (this->done)
        return this->getCosts(this->dstID);
    else
        return -1;
}