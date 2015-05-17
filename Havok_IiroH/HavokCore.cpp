#include "HavokCore.h"


//Keycode
#include <Common\Base\KeyCode.cxx>

//Define Havok product features
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_PHYSICS

#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_RegisterReflectedClasses
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities

#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common\Serialize\Util\hkBuiltinTypeRegistry.cxx>

#include <Common/Base/Config/hkProductFeatures.cxx>

static void HK_CALL errorReport(const char* message, void* userContext)
{
	std::cout << "Havok: " << message << std::endl;
}


HavokCore::HavokCore(bool vdbEnabled)
{
	_vdbEnabled = vdbEnabled;

	initHavok();
}


HavokCore::~HavokCore()
{
	deinitHavok();
}


void HavokCore::initHavok()
{
	initMemory();
	initPhysicsWorld();

	if (_vdbEnabled)
		initVDB();
}


void HavokCore::initMemory()
{
	hkMemoryRouter* memoryrouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024 * 1024));
	hkBaseSystem::init(memoryrouter, errorReport);

	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

	int totalNumThreadsUsed;

	hkHardwareInfo hwInfo;
	totalNumThreadsUsed = hwInfo.calcNumHardwareThreads();

	hkCpuThreadPoolCinfo threadPoolCinfo;
	threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

	threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
	_threadPool = new hkCpuThreadPool(threadPoolCinfo);

	hkJobQueueCinfo JobInfo;
	JobInfo.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
	_jobQueue = new hkJobQueue(JobInfo);

	hkMonitorStream::getInstance().resize(200000);
}


void HavokCore::initPhysicsWorld()
{
	_pWorldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
	_pWorldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

	//_pWorldInfo.m_gravity = hkVector4(0, -1000, 0);

	_pWorld = new hkpWorld(_pWorldInfo);
	_pWorld->m_wantDeactivation = false;
	_pWorld->markForWrite();

	hkpAgentRegisterUtil::registerAllAgents(_pWorld->getCollisionDispatcher());

	_pWorld->registerWithJobQueue(_jobQueue);
}


void HavokCore::initVDB()
{
	hkArray<hkProcessContext*> contexts;
	{
		_context = new hkpPhysicsContext();
		hkpPhysicsContext::registerAllPhysicsProcesses();
		_context->addWorld(_pWorld);
		contexts.pushBack(_context);

		_pWorld->unmarkForWrite();
	}

	_vdb = new hkVisualDebugger(contexts);
	_vdb->serve();
}


void HavokCore::deinitHavok()
{
	_pWorld->markForWrite();
	_pWorld->removeReference();

	delete _jobQueue;

	if (_vdbEnabled)
		deinitVDB();

	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}


void HavokCore::deinitVDB()
{
	_vdb->removeReference();
	_context->removeReference();
}


void HavokCore::stepSimulation(float deltaTime)
{
	stepPhysicsSimulation(deltaTime);

	if (_vdbEnabled)
		stepVDBSimulation();

	hkMonitorStream::getInstance().reset();
	_threadPool->clearTimerData();
}


void HavokCore::stepPhysicsSimulation(float deltaTime)
{
	_pWorld->stepMultithreaded(_jobQueue, _threadPool, deltaTime);
}

void HavokCore::stepVDBSimulation()
{
	_context->syncTimers(_threadPool);
	_vdb->step();
}

