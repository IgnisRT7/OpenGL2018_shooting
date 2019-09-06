/**
*	@file	Landscape.cpp
*/

#include "Landscape.h"

namespace Application {

	Landscape::Landscape(bool l) :
		isLoop(l) {
	}

	void Landscape::Initialize() {
		entity->CastShadow(false);
		start = entity->Position();
	}

	void Landscape::Update(float deltaTime) {

		timer += deltaTime;
		glm::vec3 newPos = entity->Position() + glm::vec3(0, 0, -10.0f * deltaTime);

		entity->Position(newPos);


		if (newPos.z < -80 && isLoop) {
			entity->Position(glm::vec3(glm::vec2(newPos), newPos.z + 200.0f));
		}
	}

	void SpaceSphereMain::Initialize() {
		entity->CastShadow(false);
	}

	/**
	*	”wŒi‚ÌXVˆ—
	*/
	void SpaceSphereMain::Update(float deltaTime) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += glm::radians(2.5f) * deltaTime;
		entity->Rotation(rotSpace);
	}

}