#pragma once
#include <string>
#include <vector>

#include "render/model.h"
#include "render/physics.h"
#include "core/level.h"

using namespace Render;

namespace levelLoader {
	void loadLevel(std::string levelName, Level::Level& level, ModelId golfModels[14], Physics::ColliderMeshId golfColliderMeshes[14]);


}
