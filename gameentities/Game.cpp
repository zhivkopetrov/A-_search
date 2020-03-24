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
    if (EXIT_SUCCESS != _animHandler.init(this, &_gridContainer)) {
      LOGERR("Error, _animHandler.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void Game::deinit() {

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

void Game::onNodeChanged(const NodeType nodeType, const Point &nodePos) {
  switch (nodeType) {
  case NodeType::WALL_ADD:
    _generator.addCollision(nodePos);
    break;

  case NodeType::NODE_REMOVE:
    _generator.removeNode(nodePos);
    break;

  case NodeType::START_CHANGE:
    _animHandler.perform(AnimEvent::SET_SCALE_AMIM_START_TARGET,
        _gridContainer.getNodeCoordinates(nodePos));
    _generator.setStartNodePos(nodePos);
    break;

  case NodeType::END_CHANGE:
    _animHandler.perform(AnimEvent::SET_SCALE_AMIM_END_TARGET,
        _gridContainer.getNodeCoordinates(nodePos));
    _generator.setEndNodePos(nodePos);
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

void Game::onEndAnimFinished() {
  _generator.clear();
  _gridContainer.clearGrid();
}

