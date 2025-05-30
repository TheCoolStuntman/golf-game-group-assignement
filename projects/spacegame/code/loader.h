#pragma once
#include <string>
#include <vector>

#include "render/model.h"
#include "render/physics.h"

namespace Level {
	struct Level;
}

using namespace Render;

namespace levelLoader {
	Physics::ColliderId loadLevel(std::string levelName, Level::Level& level, ModelId golfModels[14], Physics::ColliderMeshId golfColliderMeshes[14]);
}
