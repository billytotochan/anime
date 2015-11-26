 // SAMPLE_SOLUTION
#ifndef POINT_OBJ_H
#define POINT_OBJ_H

#include "particle.h"
#include "vec.h"
#include "modelerdraw.h"

class PointObj :
	public Particle
{
public:
	PointObj();
	PointObj(const PointObj& rhs);
	PointObj(float mass, const Vec3f &position, const Vec3f &velocity) : Particle(mass, position, velocity){};
	~PointObj();
	Particle* Clone() const;
	void Draw();
	PointObj& operator=(const PointObj& rhs);
};

#endif