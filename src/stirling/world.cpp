#include "world.h"

namespace stirling {

    World::World() :
        number_of_entities(0) {
    }
    
    void World::addEntity(Entity* entity) {
        assert(number_of_entities < MAX_NUMBER_OF_ENTITIES);
        entities[number_of_entities++] = entity;
    }

    void World::update(float delta_seconds) {
		for (int i = 0; i < number_of_entities; ++i) {
			entities[i]->update(delta_seconds);
		}
    }

}