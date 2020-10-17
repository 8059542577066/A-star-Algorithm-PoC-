#include "graph.h"
#include "file.h"
#include <iostream>


int main()
{
    std::cout << "Welcome to A-star Program!" << std::endl;
    std::cout << "  Option 0: Exit" << std::endl;
    std::cout << "  Option 1: Insert Points" << std::endl;
    std::cout << "  Option 2: Erase Points" << std::endl;
    std::cout << "  Option 3: Clear Points" << std::endl;
    std::cout << "  Option 4: Insert Routes" << std::endl;
    std::cout << "  Option 5: Erase Routes" << std::endl;
    std::cout << "  Option 6: Clear Routes" << std::endl;
    std::cout << "  Option 7: Find Shortest Path" << std::endl;
    std::cout << "  Option 8: Update Weight" << std::endl;
    std::cout << "  Option 9: Backup" << std::endl;

    unsigned option = 0;
    Graph graph(0);
    std::string fileName;
    std::size_t srcID, dstID;
    double weight;

    do
    {
        std::cout << std::endl
                  << "Option> ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << "  File Name: ", std::cin >> fileName;

            try
            {
                insertPoints(fileName, graph);
            }
            catch (int e)
            {
                if (e == FILE_NOT_FOUND)
                    std::cout << "ERROR - FILE NOT FOUND" << std::endl;
            }

            break;

        case 2:
            std::cout << "  File Name: ", std::cin >> fileName;

            try
            {
                erasePoints(fileName, graph);
            }
            catch (int e)
            {
                if (e == FILE_NOT_FOUND)
                    std::cout << "ERROR - FILE NOT FOUND" << std::endl;
            }

            break;

        case 3:
            graph.clearPoints();
            break;

        case 4:
            std::cout << "  File Name: ", std::cin >> fileName;

            try
            {
                insertRoutes(fileName, graph);
            }
            catch (int e)
            {
                if (e == FILE_NOT_FOUND)
                    std::cout << "ERROR - FILE NOT FOUND" << std::endl;
            }

            break;

        case 5:
            std::cout << "  File Name: ", std::cin >> fileName;

            try
            {
                eraseRoutes(fileName, graph);
            }
            catch (int e)
            {
                if (e == FILE_NOT_FOUND)
                    std::cout << "ERROR - FILE NOT FOUND" << std::endl;
            }

            break;

        case 6:
            graph.clearRoutes();
            break;

        case 7:
            std::cout << "  From: ", std::cin >> srcID;
            std::cout << "    To: ", std::cin >> dstID;
            graph.initialize(srcID, dstID);
            graph.findPath();
            std::cout << "  File Name: ", std::cin >> fileName;
            savePath(graph, fileName);
            break;

        case 8:
            std::cout << "  New Weight: ", std::cin >> weight;

            try
            {
                graph.updateWeight(weight);
            }
            catch (int e)
            {
                if (e == NEGATIVE_WEIGHT)
                    std::cout << "ERROR - NEGATIVE WEIGHT" << std::endl;
                else if (e == INCREASED_WEIGHT)
                    std::cout << " *Increased weight cleared routes"
                              << std::endl;
            }

            break;

        case 9:
            backup(graph);
            break;

        default:
            break;
        }
    } while (option);

    return 0;
}