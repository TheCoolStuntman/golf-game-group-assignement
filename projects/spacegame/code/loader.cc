#include "config.h"
#include "loader.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace Render;

namespace levelLoader {

	std::vector<std::string> splitString(const std::string& input)
	{
		std::vector<std::string> tokens;

		std::istringstream inputString(input);
		std::string token;

		while (inputString >> token) {
			tokens.push_back(token);
		}

		return tokens;
	}

	void loadLevel(std::string levelName, std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>>& levelArray)
	{

		ModelId golfModels[11] = {
			LoadModel("assets/golf/open.glb"),
			LoadModel("assets/golf/side.glb"),        
			LoadModel("assets/golf/corner.glb"),
			LoadModel("assets/golf/round-corner-c.glb"),
			LoadModel("assets/golf/inner-corner.glb"),
			LoadModel("assets/golf/straight.glb"),
			LoadModel("assets/golf/walls-to-open.glb"),
			LoadModel("assets/golf/end.glb"),
			LoadModel("assets/golf/hole-open.glb"),
			LoadModel("assets/golf/hole-square.glb"),
			LoadModel("assets/golf/flag-red.glb")
		};

		Physics::ColliderMeshId golfColliderMeshes[11] = {
			Physics::LoadColliderMesh("assets/golf/open.glb"),
			Physics::LoadColliderMesh("assets/golf/side.glb"),
			Physics::LoadColliderMesh("assets/golf/corner.glb"),
			Physics::LoadColliderMesh("assets/golf/round-corner-c.glb"),
			Physics::LoadColliderMesh("assets/golf/inner-corner.glb"),
			Physics::LoadColliderMesh("assets/golf/straight.glb"),
			Physics::LoadColliderMesh("assets/golf/walls-to-open.glb"),
			Physics::LoadColliderMesh("assets/golf/end.glb"),
			Physics::LoadColliderMesh("assets/golf/hole-open.glb"),
			Physics::LoadColliderMesh("assets/golf/hole-square.glb"),
			Physics::LoadColliderMesh("assets/golf/flag-red.glb")
		};

		std::tuple<ModelId, Physics::ColliderId, glm::mat4> levelPiece;

		std::ifstream inputFile(levelName);

		if (!inputFile) {
			throw(std::exception("no level file found"));
		}

		std::string tile;

		while (std::getline(inputFile, tile)) {
			std::vector<std::string> tileInfo = splitString(tile);
			
			if (stoi(tileInfo[0]) == -1) {
				//Do startpos shit
				continue;
			}

			std::get<0>(levelPiece) = golfModels[stoi(tileInfo[0])];
			glm::vec3 translation = glm::vec3(stoi(tileInfo[1]), stoi(tileInfo[2]), stoi(tileInfo[3]));
			
			float rotation = glm::radians(stof(tileInfo[4]));
			glm::mat4 transform = glm::translate(translation) * glm::rotate(rotation, glm::vec3(0, 1, 0));
			std::get<1>(levelPiece) = Physics::CreateCollider(golfColliderMeshes[0], transform);
			std::get<2 >(levelPiece) = transform;
			levelArray.push_back(levelPiece);
		}

		inputFile.close();
	}
}
