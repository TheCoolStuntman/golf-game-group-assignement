#include "config.h"
#include "loader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


using namespace Render;

namespace loader {

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

	void loadLevel(std::string levelName, std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>>& levelArray, ModelId golfModels[14], Physics::ColliderMeshId golfColliderMeshes[14])
	{
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
			std::get<1>(levelPiece) = Physics::CreateCollider(golfColliderMeshes[stoi(tileInfo[0])], transform);
			std::get<2 >(levelPiece) = transform;
			levelArray.push_back(levelPiece);
		}

		inputFile.close();
	}
}

