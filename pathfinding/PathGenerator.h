#ifndef PATHFINDING_PATHGENERATOR_H_
#define PATHFINDING_PATHGENERATOR_H_

//C system headers

//C++ system headers
#include <set>
#include <vector>

//Other libraries headers

//Own components headers
#include "pathfinding/Heuristic.h"

//Forward declarations
struct Node;
class ObstacleHandlerProxyInterface;

class PathGenerator {
public:
  PathGenerator();

  ~PathGenerator() = default;

  int32_t init(const int32_t mazeWidth, const int32_t mazeHeight,
               const bool isDiagonalMovementEnabled,
               HeuristicFunction heuristic,
               ObstacleHandlerProxyInterface *obstacleHandlerInterface);

  std::vector<Point> findPath(const Point &source, const Point &target) const;

private:
  enum InternalDefines {
    DIAGONAL_MOVEMENTS = 8, NON_DIAGONAL_MOVEMENTS = 4,
  };

  bool detectCollision(const Point &position) const;

  Node* findNodeOnList(const std::set<Node*> &nodes,
                       const Point &position) const;

  void releaseNodes(std::set<Node*> &nodes) const;

  HeuristicFunction _heuristic;
  std::vector<Point> _moveDirections;
  ObstacleHandlerProxyInterface *_obstacleHandlerInterface;

  uint32_t _allowedDirectionsCount;
  int32_t _mazeWidth;
  int32_t _mazeHeight;
};

#endif /* PATHFINDING_PATHGENERATOR_H_ */