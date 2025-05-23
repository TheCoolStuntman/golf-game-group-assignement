#pragma once

#include <vector>

#include "glm.hpp"

#include "render/model.h"
#include "render/physics.h"

namespace Level {

	struct Tile {
		Render::ModelId model;
		Physics::ColliderId collider;
		glm::mat4 transform;
	};

	struct Level {
		std::vector<Tile> tiles;
	};

}