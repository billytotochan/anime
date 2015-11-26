#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() :
	bake_fps(0.0f), 
	bake_start_time(0.0f), 
	bake_end_time(0.0f), 
	simulate(false), 
	dirty(false)
{
	// TODO

}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_start_time = 0.0f;
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	bake_start_time = 0.0f;
	simulate = false;
	dirty = true;
	clearBaked();
}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	// TODO
	if (simulate){
		const float deltaT = 0.0025;
		for (auto &particle : _Particles) {
			Vec3f force = Vec3f(0.0f, 0.0f, 0.0f);
			Vec3f velocity = particle->getVelocity();
			Vec3f position = particle->getPosition();
			for(auto &f : _Forces)
				force += f->getForce()*particle->getMass();
			velocity += force / particle->getMass() * deltaT;
			position += velocity * deltaT;
			particle->setVelocity(velocity);
			particle->setPosition(position);
		}
		bakeParticles(t);
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{

	// TODO
	if (simulate) {
		for (vector<Particle*>::iterator bp = _BakedParticles[t].begin(); bp != _BakedParticles[t].end(); bp++) {
			(*bp)->Draw();
		}
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{

	// TODO
	_BakedParticles[t].clear();
	for (auto &particle : _Particles) {
		_BakedParticles[t].push_back(particle);
	}
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
	_BakedParticles.clear();
}





