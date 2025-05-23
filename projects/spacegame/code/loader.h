#pragma once
#include <string>
#include <vector>

#include "render/model.h"
#include "render/physics.h"

using namespace Render;

namespace loader {
	void loadLevel(std::string levelName, std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>>& levelArray, ModelId golfModels[14], Physics::ColliderMeshId golfColliderMeshes[14]);

	void writeHighscore(std::string highscorePath, int score);
}
