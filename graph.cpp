#include "graph.h"
#include <limits>
#include <cmath>
#include <algorithm>


typedef std::pair<VectorE::iterator, VectorE::iterator> range_t;

const double WEIGHT = 6371;
const double INFINITE_DOUBLE = std::numeric_limits<double>::infinity();


Vertex::Vertex(index_t number)
{
    this->number = number;
}

Vertex::Vertex(index_t number, double phi, double theta)
{
    this->number = number;
    this->unitX = std::sin(phi) * std::cos(theta);
    this->unitY = std::sin(phi) * std::sin(theta);
    this->unitZ = std::cos(phi);
}

bool Vertex::operator<(const Vertex &other) const
{
    return this->number < other.number;
}

double Vertex::operator*(const Vertex &other) const
{
    return this->unitX * other.unitX +
           this->unitY * other.unitY +
           this->unitZ * other.unitZ;
}

double Vertex::heuristic(const Vertex &other) const
{
    return WEIGHT * std::acos(*this * other);
}

index_t Vertex::getNumber() const
{
    return this->number;
}

index_t Vertex::find(const VectorV &lookup) const
{
    return std::lower_bound(lookup.begin(), lookup.end(), *this) -
           lookup.begin();
}


Edge::Edge(index_t srcVertexID)
{
    this->srcVertexID = srcVertexID;
}

Edge::Edge(index_t srcVertexID, index_t dstVertexID)
{
    this->srcVertexID = srcVertexID;
    this->dstVertexID = dstVertexID;
}

Edge::Edge(index_t srcNumber, index_t dstNumber, double cost,
           const VectorV &vertices)
{
    this->srcVertexID = Vertex(srcNumber).find(vertices);
    this->dstVertexID = Vertex(dstNumber).find(vertices);
    this->cost = cost;
}

bool Edge::operator<(const Edge &other) const
{
    if (this->srcVertexID != other.srcVertexID)
        return this->srcVertexID < other.srcVertexID;

    if (this->dstVertexID != other.dstVertexID)
        return this->dstVertexID < other.dstVertexID;

    return false;
}

bool Edge::SrcVertexOnly::operator()(const Edge &e1,
                                     const Edge &e2) const
{
    return e1.srcVertexID < e2.srcVertexID;
}

index_t Edge::getDstVertexID() const
{
    return this->dstVertexID;
}

double Edge::getCost() const
{
    return this->cost;
}

index_t Edge::find(const VectorE &lookup) const
{
    return std::lower_bound(lookup.begin(), lookup.end(), *this) -
           lookup.begin();
}


Node::Node(index_t vertexID)
{
    this->vertexID = vertexID;
}

Node::Node(index_t vertexID, index_t dstVertexID, double costs,
           const VectorV &vertices)
{
    this->vertexID = vertexID;
    this->prevVertexID = (index_t)-1;
    this->costs = costs;
    this->dist = vertices[vertexID].heuristic(vertices[dstVertexID]);
}

bool Node::operator<(const Node &other) const
{
    return this->vertexID < other.vertexID;
}

bool Node::MinScore::operator()(const Node *a,
                                const Node *b) const
{
    return a->score > b->score;
}

index_t Node::getVertexID() const
{
    return this->vertexID;
}

index_t Node::getPrevVertexID() const
{
    return this->prevVertexID;
}

index_t Node::find(const VectorN &lookup) const
{
    return std::lower_bound(lookup.begin(), lookup.end(), *this) -
           lookup.begin();
}

void Node::update(index_t nextVertexID,
                  const VectorE &edges, VectorN &nodes)
{
    Edge edge(this->vertexID, nextVertexID);
    double costs = this->costs + edges[edge.find(edges)].getCost();

    if (costs < nodes[nextVertexID].costs)
    {
        nodes[nextVertexID].prevVertexID = this->find(nodes);
        nodes[nextVertexID].costs = costs;
        nodes[nextVertexID].score = nodes[nextVertexID].costs +
                                    nodes[nextVertexID].dist;
    }
}


Path::Path(index_t srcNumber, index_t dstNumber,
           const VectorV &vertices, const VectorE &edges)
{
    index_t srcVertexID = Vertex(srcNumber).find(vertices);
    this->dstVertexID = Vertex(dstNumber).find(vertices);
    this->vertices = vertices;
    this->edges = edges;
    std::sort(this->vertices.begin(), this->vertices.end());
    std::sort(this->edges.begin(), this->edges.end());
    this->nodes.reserve(vertices.size());

    for (index_t i = 0; i < this->vertices.size(); ++i)
        if (i == srcVertexID)
            this->nodes.push_back(
                Node(srcVertexID, this->dstVertexID, 0, vertices));
        else
            this->nodes.push_back(
                Node(i, this->dstVertexID, INFINITE_DOUBLE, vertices));

    this->heapNodePtrs.push_back(&this->nodes[srcVertexID]);
}

void Path::findPath()
{
    while (!this->heapNodePtrs.empty())
    {
        index_t top = this->heapNodePtrs.front() - &this->nodes.front();
        std::pop_heap(this->heapNodePtrs.begin(),
                      this->heapNodePtrs.end(), Node::MinScore());
        this->heapNodePtrs.pop_back();
        this->visitedVertexIDs.insert(top);
        range_t range = std::equal_range(this->edges.begin(),
                                         this->edges.end(),
                                         Edge(top), Edge::SrcVertexOnly());
        index_t lower = range.first - this->edges.begin(),
                upper = range.second - this->edges.begin();

        for (index_t i = lower; i < upper; ++i)
        {
            index_t dstVertexID = this->edges[i].getDstVertexID();
            this->nodes[top].update(dstVertexID, this->edges, this->nodes);

            if (dstVertexID != this->dstVertexID &&
                this->visitedVertexIDs.find(dstVertexID) ==
                    this->visitedVertexIDs.end())
                this->heapNodePtrs.push_back(&this->nodes[dstVertexID]);
        }

        std::make_heap(this->heapNodePtrs.begin(),
                       this->heapNodePtrs.end(), Node::MinScore());
    }
}

void Path::reset(index_t srcNumber, index_t dstNumber)
{
    index_t srcVertexID = Vertex(srcNumber).find(this->vertices);
    this->dstVertexID = Vertex(dstNumber).find(this->vertices);

    for (index_t i = 0; i < this->vertices.size(); ++i)
        if (i == srcVertexID)
            this->nodes[i] =
                Node(srcVertexID, this->dstVertexID, 0, vertices);
        else
            this->nodes[i] =
                Node(i, this->dstVertexID, INFINITE_DOUBLE, vertices);

    this->heapNodePtrs.clear();
    this->heapNodePtrs.push_back(&this->nodes[srcVertexID]);
    this->visitedVertexIDs.clear();
}

VectorI Path::getPath() const
{
    VectorI path;
    index_t vertexID = this->dstVertexID, prevVertexID;
    path.push_back(this->vertices[vertexID].getNumber());
    prevVertexID = this->nodes[Node(vertexID).find(this->nodes)]
                       .getPrevVertexID();

    while (prevVertexID != (index_t)-1)
    {
        vertexID = prevVertexID;
        path.push_back(this->vertices[vertexID].getNumber());
        prevVertexID = this->nodes[Node(vertexID).find(this->nodes)]
                           .getPrevVertexID();
    }

    std::reverse(path.begin(), path.end());

    return path;
}

double Path::getCosts() const
{
    double costs = 0;
    index_t vertexID = this->dstVertexID,
            prevVertexID = this->nodes[Node(vertexID).find(this->nodes)]
                               .getPrevVertexID();

    while (prevVertexID != (index_t)-1)
    {
        Edge edge(prevVertexID, vertexID);
        costs += this->edges[edge.find(this->edges)].getCost();
        vertexID = prevVertexID;
        prevVertexID = this->nodes[Node(vertexID).find(this->nodes)]
                           .getPrevVertexID();
    }

    return costs;
}