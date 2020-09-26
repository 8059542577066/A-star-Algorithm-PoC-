#include "graph.h"
#include <iostream>
#include <cstdlib>


int main()
{
    index_t size = 18;
    VectorV vertices;

    for (index_t i = 0; i < size; ++i)
        vertices.push_back(Vertex(i, 0, 0));

    VectorE edges;

    // Row 0
    edges.push_back(Edge(0, 2, 2, vertices));
    edges.push_back(Edge(0, 7, 4, vertices));
    edges.push_back(Edge(0, 14, 1, vertices));

    // Row 1
    edges.push_back(Edge(1, 2, 9, vertices));
    edges.push_back(Edge(1, 4, 2, vertices));
    edges.push_back(Edge(1, 5, 8, vertices));

    // Row 2
    edges.push_back(Edge(2, 0, 2, vertices));
    edges.push_back(Edge(2, 1, 9, vertices));
    edges.push_back(Edge(2, 3, 8, vertices));
    edges.push_back(Edge(2, 6, 5, vertices));

    // Row 3
    edges.push_back(Edge(3, 2, 8, vertices));
    edges.push_back(Edge(3, 6, 4, vertices));

    // Row 4
    edges.push_back(Edge(4, 1, 2, vertices));
    edges.push_back(Edge(4, 5, 7, vertices));
    edges.push_back(Edge(4, 7, 6, vertices));
    edges.push_back(Edge(4, 8, 8, vertices));

    // Row 5
    edges.push_back(Edge(5, 1, 8, vertices));
    edges.push_back(Edge(5, 4, 7, vertices));
    edges.push_back(Edge(5, 8, 4, vertices));
    edges.push_back(Edge(5, 9, 7, vertices));

    // Row 6
    edges.push_back(Edge(6, 2, 5, vertices));
    edges.push_back(Edge(6, 3, 4, vertices));
    edges.push_back(Edge(6, 9, 7, vertices));
    edges.push_back(Edge(6, 10, 1, vertices));

    // Row 7
    edges.push_back(Edge(7, 0, 4, vertices));
    edges.push_back(Edge(7, 4, 6, vertices));
    edges.push_back(Edge(7, 8, 4, vertices));
    edges.push_back(Edge(7, 11, 8, vertices));

    // Row 8
    edges.push_back(Edge(8, 4, 8, vertices));
    edges.push_back(Edge(8, 5, 4, vertices));
    edges.push_back(Edge(8, 7, 4, vertices));
    edges.push_back(Edge(8, 11, 6, vertices));
    edges.push_back(Edge(8, 12, 8, vertices));

    // Row 9
    edges.push_back(Edge(9, 5, 7, vertices));
    edges.push_back(Edge(9, 6, 7, vertices));

    // Row 10
    edges.push_back(Edge(10, 6, 1, vertices));
    edges.push_back(Edge(10, 17, 5, vertices));

    // Row 11
    edges.push_back(Edge(11, 7, 8, vertices));
    edges.push_back(Edge(11, 8, 6, vertices));
    edges.push_back(Edge(11, 14, 1, vertices));

    // Row 12
    edges.push_back(Edge(12, 8, 8, vertices));

    // Row 13
    edges.push_back(Edge(13, 16, 6, vertices));

    // Row 14
    edges.push_back(Edge(14, 0, 1, vertices));
    edges.push_back(Edge(14, 11, 1, vertices));
    edges.push_back(Edge(14, 15, 5, vertices));

    // Row 15
    edges.push_back(Edge(15, 14, 5, vertices));
    edges.push_back(Edge(15, 16, 1, vertices));
    edges.push_back(Edge(15, 17, 1, vertices));

    // Row 16
    edges.push_back(Edge(16, 13, 6, vertices));
    edges.push_back(Edge(16, 15, 1, vertices));

    // Row 17
    edges.push_back(Edge(17, 10, 5, vertices));
    edges.push_back(Edge(17, 15, 1, vertices));

    index_t srcNumber, dstNumber;
    std::cout << "Enter Starting Node: ";
    std::cin >> srcNumber;
    std::cout << "Enter Ending Node: ";
    std::cin >> dstNumber;
    std::cout << std::endl
              << "Shortest Path:" << std::endl;

    Path path(srcNumber, dstNumber, vertices, edges);
    path.findPath();
    VectorI result = path.getPath();

    for (index_t i = 0; i < result.size(); ++i)
        std::cout << "  Node " << result[i] << std::endl;

    std::cout << "Total Costs: " << path.getCosts() << std::endl;

    system("pause");

    return 0;
}