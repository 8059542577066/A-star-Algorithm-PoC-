#ifndef FILE_H
#define FILE_H

#include "graph.h"
#include <string>


#ifndef FILE_NOT_FOUND
#define FILE_NOT_FOUND 1
#endif


void insertPoints(const std::string &,
                  Graph &);
void erasePoints(const std::string &,
                 Graph &);
void insertRoutes(const std::string &,
                  Graph &);
void eraseRoutes(const std::string &,
                 Graph &);
void backup(const Graph &);
void savePath(const Graph &,
              const std::string &);


#endif