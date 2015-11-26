 // SAMPLE_SOLUTION
#include "particle.h"
Particle::Particle() : _Mass(0.0), _Position(Vec3f(0, 0, 0)), _Velocity(Vec3f(0, 0, 0)) 
{}

Particle::Particle(float mass, const Vec3f &position, const Vec3f &velocity) : _Mass(mass), _Position(position), _Velocity(velocity) 
{}

Force::Force()
{}

Force::~Force()
{}

Gravity::Gravity(float G) : _Gravity(G)
{}

Gravity::~Gravity()
{}

Wind::Wind()
{}

Wind::~Wind()
{}