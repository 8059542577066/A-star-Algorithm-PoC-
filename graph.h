#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>


class Vertex;
class Edge;
class Node;

typedef std::size_t index_t;
typedef std::vector<Vertex> VectorV;
typedef std::vector<Edge> VectorE;
typedef std::vector<Node> VectorN;
typedef std::vector<Node *> VectorNP;
typedef std::multiset<index_t> MulSetI;
typedef std::vector<index_t> VectorI;


class Vertex
{
private:
    index_t number;
    double unitX;
    double unitY;
    double unitZ;

public:
    Vertex(index_t);
    Vertex(index_t, double, double);

    bool operator<(const Vertex &) const;
    double operator*(const Vertex &) const;

    double heuristic(const Vertex &) const;
    index_t getNumber() const;
    index_t find(const VectorV &) const;
};


class Edge
{
private:
    index_t srcVertexID;
    index_t dstVertexID;
    double cost;

public:
    Edge(index_t);
    Edge(index_t, index_t);
    Edge(index_t, index_t, double,
         const VectorV &);

    bool operator<(const Edge &) const;
    struct SrcVertexOnly
    {
        bool operator()(const Edge &,
                        const Edge &) const;
    };

    index_t getDstVertexID() const;
    double getCost() const;
    index_t find(const VectorE &) const;
};


class Node
{
private:
    index_t vertexID;
    index_t prevVertexID;
    double costs;
    double dist;
    double score;

public:
    Node(index_t);
    Node(index_t, index_t, double,
         const VectorV &);

    bool operator<(const Node &) const;
    struct MinScore
    {
        bool operator()(const Node *,
                        const Node *) const;
    };

    index_t getVertexID() const;
    index_t getPrevVertexID() const;
    index_t find(const VectorN &) const;

    void update(index_t,
                const VectorE &, VectorN &);
};


class Path
{
private:
    index_t dstVertexID;
    VectorV vertices;
    VectorE edges;
    VectorN nodes;
    VectorNP heapNodePtrs;
    MulSetI visitedVertexIDs;

public:
    Path(index_t, index_t,
         const VectorV &, const VectorE &);

    void findPath();
    void reset(index_t, index_t);
    VectorI getPath() const;
    double getCosts() const;
};


#endif