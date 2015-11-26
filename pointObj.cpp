 // SAMPLE_SOLUTION
#include "pointObj.h"
#include "vec.h"
#include "modelerdraw.h"

PointObj::PointObj() : Particle()
{
}

PointObj::~PointObj(){

}

PointObj::PointObj(const PointObj& rhs) {
	_Mass = rhs._Mass;
	_Position = rhs._Position;
	_Velocity = rhs._Velocity;
}

void PointObj::Draw(){
	
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glPushMatrix();
	setDiffuseColor(1, 1, 1);
	//printf("%f %f %f \n",_Position[0], _Position[1], _Position[2]);
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(_Position[0], _Position[1], _Position[2]);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	
}

Particle* PointObj::Clone() const {
	// i don't know how to copy la
	Particle* particle = new PointObj();
	particle->setMass(_Mass);
	particle->setPosition(Vec3f(_Position[0], _Position[1], _Position[2]));
	particle->setVelocity(Vec3f(_Velocity[0], _Velocity[1], _Velocity[2]));
	return particle;
}

PointObj& PointObj::operator=(const PointObj& rhs) {
	if (this != &rhs) {
		_Mass = rhs._Mass;
		_Position = rhs._Position;
		_Velocity = rhs._Velocity;
	}
	return *this;
}