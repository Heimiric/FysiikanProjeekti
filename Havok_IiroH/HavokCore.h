#pragma once

#include <iostream>

//Havok includes
#include <Common\Base\hkBase.h>
#include <Common\Base\System\hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common\Base\System\Hardware\hkHardwareInfo.h>
#include <Common\Base\Thread\Pool\hkCpuThreadPool.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common\Base\Memory\System\Util\hkMemoryInitUtil.h>
#include <Common\Base\Memory\Allocator\Malloc\hkMallocAllocator.h>
#include <Common/Base/Fwd/hkcstdio.h>
//Havok Physics
#include <Physics2012\Dynamics\hkpDynamics.h>
#include <Physics2012\Dynamics\World\hkpWorld.h>
#include <Physics2012\Dynamics\Entity\hkpRigidBody.h>
#include <Physics2012\Collide\hkpCollide.h>
#include <Physics2012\Collide\Dispatch\hkpAgentRegisterUtil.h>

#include <Common\Visualize\hkVisualDebugger.h>
#include <Physics2012\Utilities\VisualDebugger\hkpPhysicsContext.h>

class HavokCore
{
public:
	HavokCore(bool vdbEnabled);
	~HavokCore();

	void stepSimulation(float deltaTime);

	hkpWorld* getWorld() { return _pWorld; }

private:
	bool _vdbEnabled; // vdb = visual debugger
	hkpPhysicsContext* _context;
	hkVisualDebugger* _vdb;

	hkJobQueue* _jobQueue;

	hkpWorld* _pWorld;
	hkpWorldCinfo _pWorldInfo;

	hkCpuThreadPool* _threadPool;

	void initHavok();
	void initMemory();
	void initPhysicsWorld();
	void initVDB();

	void deinitHavok();
	void deinitVDB();

	void stepPhysicsSimulation(float deltaTime);
	void stepVDBSimulation();
};

