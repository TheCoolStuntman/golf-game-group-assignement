#include "config.h"
#include "loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>


using namespace Render;

namespace levelLoader {
	void loadLevel(std::string levelName, std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>>& levelArray)
	{

		ModelId golfModels[7] = {
			LoadModel("assets/golf/open.glb"),
			LoadModel("assets/golf/side.glb"),
			LoadModel("assets/golf/corner.glb"),
			LoadModel("assets/golf/straight.glb"),
			LoadModel("assets/golf/end.glb"),
			LoadModel("assets/golf/hole-open.glb"),
			LoadModel("assets/golf/hole-square.glb")
		};

		Physics::ColliderMeshId golfColliderMeshes[7] = {
			Physics::LoadColliderMesh("assets/golf/open.glb"),
			Physics::LoadColliderMesh("assets/golf/side.glb"),
			Physics::LoadColliderMesh("assets/golf/corner.glb"),
			Physics::LoadColliderMesh("assets/golf/straight.glb"),
			Physics::LoadColliderMesh("assets/golf/end.glb"),
			Physics::LoadColliderMesh("assets/golf/hole-open.glb"),
			Physics::LoadColliderMesh("assets/golf/hole-square.glb")
		};

		std::tuple<ModelId, Physics::ColliderId, glm::mat4> levelPiece;

		std::ifstream inputFile(levelName);

		std::string tile;

		while (std::getline(inputFile, tile)) {
			//std::istringstream segmented(tile);

			std::cout << "HELLO HELLO \n";
			auto split_strings = tile | std::ranges::views::split(' ');



		}

		inputFile.close();

		/*for (size_t i = 0; i < 2; i++) {
		    std::get<0>(levelPiece) = golfModels[0];
		    glm::vec3 translation = glm::vec3((float)i, 0.0f, 0.0f);
		    glm::vec3 rotationAxis = normalize(translation);
		    float rotation = 0;
		    glm::mat4 transform = glm::rotate(rotation, glm::vec3(0, 1, 0)) * glm::translate(translation);
		    std::get<1>(levelPiece) = Physics::CreateCollider(golfColliderMeshes[0], transform);
		    std::get<2 >(levelPiece) = transform;
		    levelArray.push_back(levelPiece);
		}*/
	}
}
