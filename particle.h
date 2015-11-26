 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "vec.h"
#include <memory>


class Force {
public:
	Force();
	~Force();
	virtual Vec3f getForce(){ return Vec3f(0.0f, 0.0f, 0.0f); }
	//Vec3f getForce(){ return Vec3f(0.0f, 0.0f, 0.0f); }
protected:
};

class Gravity : public Force {
public:
	Gravity(float g);
	~Gravity();
	void setGravity(float g){ _Gravity = g; }
	Vec3f getForce(){ return Vec3f(0.0f, -_Gravity, 0.0f); }
protected:
	float _Gravity;
};

class Wind : public Force
{
public:
	Wind();
	~Wind();
	void setMagnitube(float magnitube){ _Magnitube = magnitube; }
	void setDirection(Vec3f& direction){ _Direction = direction; }
	Vec3f getForce(){ return _Magnitube*_Direction; }
protected:
	float _Magnitube;
	Vec3f _Direction;

};

class Particle {
public:
	Particle();
	Particle(float Mass, const Vec3f &Position, const Vec3f &Velocity);
	float getMass() { return _Mass; }
	Vec3f getPosition() { return _Position; }
	Vec3f getVelocity() { return _Velocity; }
	void setMass(float Mass) { _Mass = Mass; }
	void setPosition(Vec3f Position) { _Position = Position; }
	void setVelocity(Vec3f Velocity) { _Velocity = Velocity; }
	virtual Particle* Clone() const = 0;
	virtual void Draw() = 0;
	void addForce(Force* f) { _Forces.push_back(f); }

protected:
	float _Mass;
	Vec3f _Position;
	Vec3f _Velocity;
	vector<Force *> _Forces;
};

#endif