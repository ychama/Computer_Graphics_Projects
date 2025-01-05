#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<GameGeometry> _g)
    : geometry(_g),
      position(0.0f, 0.0f, 1.0f),
      theta(0),
      scale(0.075f),
      transformationMatrix(1.f) {} // Identity matrix