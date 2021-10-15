#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Rays have a point and a direction, evaluated at t

class ray {
public:
	ray() {}
	ray(const point3& origin, const vec3& direction)
		: orig(origin), dir(direction)
	{}

	point3 origin() const {
		return orig;
	}
	vec3 direction() const {
		return dir;
	}

	point3 at(double t) const {
		return orig + t * dir;
	}

public:
	point3 orig;
	vec3 dir;

};
#endif