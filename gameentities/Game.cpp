//Corresponding header
#include "Game.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "common/CommonDefines.h"
#include "utils/Log.h"

int32_t Game::init(const bool isDiagonalMovementAllowed) {
  int32_t err = EXIT_SUCCESS;

  if (EXIT_SUCCESS != _gridContainer.init(this, Textures::VERTICAL_LINE,
          Textures::HORIZONTAL_LINE, Textures::START_NODE, Textures::END_NODE,
          Textures::A_STAR_PATH, Textures::WALL)) {
    LOGERR("Error in _gridLineContainer.init()");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    HeuristicFunction heuristic = Heuristic::manhattan;

    //diagonal heuristic is better for diagonal movements
    if (isDiagonalMovementAllowed) {
      heuristic = Heuristic::diagonal;
    }

    if (EXIT_SUCCESS != _generator.init(Grid::GRID_WIDTH, Grid::GRID_HEIGHT,
            isDiagonalMovementAllowed, heuristic)) {
      LOGERR("Error, _generator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _animHandler.init(&_gridContainer)) {
      LOGERR("Error, _animHandler.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void Game::deinit() {
//#warning when hit space key if not possible solution is found - show the big batman and in the speech bubble say - "no solution found"
//#warning if a solution is found - again show the big batman and in the speech bubble say - "Let's roll out" and begin the shrink animation
//#warning I need to implement another animator that does the above two things
}

void Game::draw() {
  _gridContainer.draw();
  _animHandler.draw();
}

void Game::handleUserEvent(const SDL_Event &e) {
  if (_animHandler.isActive()) {
    return;
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {

    case SDLK_SPACE:
      if (_generator.isReadyToEvaluate()) {
        evaluateAStar();
      }
      break;

    case SDLK_c:
      _generator.clear();
      _gridContainer.clearGrid();
      break;
    }
  }

  _gridContainer.handleUserEvent(e);
}

void Game::onNodeChanged(const NodeType nodeType, const int32_t nodeX,
                         const int32_t nodeY) {
  switch (nodeType) {
  case NodeType::WALL_ADD:
    _generator.addCollision( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addCollision(nodeX, nodeY);
    break;

  case NodeType::WALL_REMOVE:
    _generator.removeCollision( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.removeCollision(nodeX, nodeY);
    break;

  case NodeType::START_CHANGE:
    _animHandler.perform(AnimEvent::SET_SCALE_AMIM_TARGET,
        _gridContainer.getNodeCoordinates(nodeX, nodeY));
    _generator.setStartNodePos( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addStartNode(nodeX, nodeY);
    break;

  case NodeType::END_CHANGE:
    _generator.setEndNodePos( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addEndNode(nodeX, nodeY);
    break;

  default:
    LOGERR("Error, received unknown NodeType: %d",
        static_cast<int32_t>(nodeType))
    ;
    break;
  }
}

void Game::evaluateAStar() {
  // This method returns vector of coordinates from target to source.
  // the data will be moved, so it's not const
  std::vector<Point> path = _generator.findPath();

  if ( (path.front() != _generator.getEndNodePos())) {
    _animHandler.perform(AnimEvent::START_NO_PATH_ANIM);
  } else {
    _animHandler.perform(AnimEvent::LOAD_ANIM_PATH, &path);
    _animHandler.perform(AnimEvent::START_PATH_ANIM);
  }
}

