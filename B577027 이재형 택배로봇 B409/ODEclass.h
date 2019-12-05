#pragma once

#ifndef _ODECLASS_H_
#define _ODECLASS_H_

#define dDOUBLE

#include <ode/ode.h>

#define DENSITY (5.0)		// density of all objects
#define MAX_CONTACTS 64		// maximum number of contact points per body


static dWorldID ODEWorld;
static dSpaceID ODESpace;
static dJointGroupID ODEContactgroup;


class ODEclass
{
public:
	ODEclass();
	~ODEclass();

	bool Initialize();
	void SimulationUpdate();
	static void nearCallback(void *, dGeomID o1, dGeomID o2);

	dWorldID GetWorldID();
	dSpaceID GetSpaceID();
	dJointGroupID GetContactgroupID();

	dGeomID ModelGeoms[8];
	dReal bounceVec3[8][3];

	void SetGeomIDandBounceVec3(ODEclass *ode, dGeomID geomID, dReal *vec3, int index);
	static dGeomID GetGeomID(int index);
	static dReal* GetBounceVec3(int index);

private:

};

static ODEclass *ODE;

#endif
