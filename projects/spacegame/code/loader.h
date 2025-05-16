#pragma once
#include <string>
#include <vector>

#include "render/model.h"
#include "render/physics.h"

using namespace Render;

namespace levelLoader {
	void loadLevel(std::string levelName, std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>>& levelArray);


}