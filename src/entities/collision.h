#include "ball.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>

struct circle {
	float x, y, radius;
};

class Collision : public Entity {
	private:
	::entities entities;
	::entities collision_entities;

	public:
	void set_entities(const ::entities &entities) {
		this->entities = entities;
	}

	public:
	void add_event_listener(Entity *entity) {
		collision_entities.push_back(entity);
	}

	bool check_collision(circle& circle, Rectangle& rect) {
		float closestX = 
			std::clamp(circle.x, 
					rect.x - rect.width / 2.0f,
					rect.x + rect.width / 2.0f);
		float closestY = 
			std::clamp(circle.y,
					rect.y - rect.height / 2.0f,
					rect.y + rect.height / 2.0f);

	    float distanceSquared = 
			(circle.x - closestX) * (circle.x - closestX) +
	        (circle.y - closestY) * (circle.y - closestY);

		return distanceSquared <= 
			(circle.radius * circle.radius);
	}
	
	public:
	void update(float &delta_time) override {
		for (auto &collision_entity : collision_entities) {
			bool was_collision = false;
			for (auto &entity : entities) {
				if (collision_entity == entity) {
					continue;
				}

				/*
				 * I use the built in rectangles
				 * wrong so the rec.x is not 
				 * the actual x of the rec
				 */

				float x = collision_entity->get_x();
				float y = collision_entity->get_y();

				Rectangle rec = collision_entity->get_rec();
				circle circle;
				circle.radius = rec.width / 2;
				circle.x = x + rec.width / 2;
				circle.y = y + rec.height / 2;
				
				x = entity->get_x();
				y = entity->get_y();

				Rectangle rec1;
				rec1 = entity->get_rec();
				rec1.x = x + rec1.width / 2;
				rec1.y = y + rec1.height / 2;

				bool collision =
					check_collision(circle, rec1); 

				if (collision) {
					was_collision = true;
					collision_entity->set_collides(true);
				}
			}

			if (!was_collision) {
				collision_entity->set_collides(false);
			}
		}
	}
};

