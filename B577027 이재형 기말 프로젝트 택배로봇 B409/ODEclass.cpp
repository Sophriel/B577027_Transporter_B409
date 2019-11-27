
#include "ODEclass.h"

ODEclass::ODEclass()
{

}
ODEclass::~ODEclass()
{
}

bool ODEclass::Initialize()
{
	bool result = false;

	result = dInitODE2(0);
	if (!result)
	{
		return false;
	}
	ODEWorld = dWorldCreate();
	ODESpace = dSimpleSpaceCreate(0);
	ODEContactgroup = dJointGroupCreate(0);

	dWorldSetGravity(ODEWorld, 0, -9.81, 0);
	dWorldSetCFM(ODEWorld, 1e-5);
	dWorldSetERP(ODEWorld, 0.1);
	dWorldSetAutoDisableFlag(ODEWorld, 1);

	return true;
}

void ODEclass::SimulationUpdate()
{
	dSpaceCollide(ODESpace, 0, &nearCallback);
	dWorldStep(ODEWorld, 0.02);
	dJointGroupEmpty(ODEContactgroup);
}

void ODEclass::nearCallback(void *, dGeomID o1, dGeomID o2)
{
	int i;

	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) 
		return;

	dContact contact[MAX_CONTACTS];   // up to MAX_CONTACTS contacts per box-box
	for (i = 0; i < MAX_CONTACTS; i++) 
	{
		contact[i].surface.mode = dContactBounce | dContactSoftERP | dContactSoftCFM | dContactApprox1;
		contact[i].surface.mu = 0.2;
		contact[i].surface.mu2 = 0.1;
		contact[i].surface.bounce = 0.1;
		contact[i].surface.bounce_vel = 0.1;
		contact[i].surface.soft_erp = 0.2;
		contact[i].surface.soft_cfm = 0.00001;
	}
	
	//dBodySetForce(b1, 0, 0, 0);
	//dBodySetForce(b2, 0, 0, 0);

	for (int i = 0; i < 8; i++)
		if (o1 == GetGeomID(i))
			dBodyAddForce(b2, GetBounceVec3(i)[0], GetBounceVec3(i)[1], GetBounceVec3(i)[2]);

	for (int i = 0; i < 8; i++)
		if (o2 == GetGeomID(i))
			dBodyAddForce(b1, GetBounceVec3(i)[0], GetBounceVec3(i)[1], GetBounceVec3(i)[2]);
	

	return;

	//int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));

	//if (numc)
	//{
	//	for (i = 0; i < numc; i++)
	//	{
	//		dJointID c = dJointCreateContact(ODEWorld, ODEContactgroup, contact + i);
	//		dJointAttach(c, b1, b2);
	//	}
	//}
}



dWorldID ODEclass::GetWorldID()
{
	return ODEWorld;
}
dSpaceID ODEclass::GetSpaceID()
{
	return ODESpace;
}
dJointGroupID ODEclass::GetContactgroupID()
{
	return ODEContactgroup;
}

void ODEclass::SetGeomIDandBounceVec3(ODEclass *ode, dGeomID geomID, dReal *vec3, int index)
{
	ODE = ode;
	ModelGeoms[index] = geomID;
	bounceVec3[index][0] = vec3[0];
	bounceVec3[index][1] = vec3[1];
	bounceVec3[index][2] = vec3[2];
}
dGeomID ODEclass::GetGeomID(int index)
{
	ODEclass *ode = ODE;

	return ode->ModelGeoms[index];
}
dReal* ODEclass::GetBounceVec3(int index)
{
	ODEclass *ode = ODE;

	return ode->bounceVec3[index];
} 
