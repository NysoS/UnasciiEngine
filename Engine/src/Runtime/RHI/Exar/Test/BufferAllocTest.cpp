#include "Engine/Runtime/RHI/Exar/Test/BufferAllocTest.hpp"
#include "Engine/Runtime/RHI/Exar/ExarDevice.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Exar.hpp"
#include "Engine/Runtime/RHI/Exar/ExarAllocatorDesc.hpp"
#include "Engine/Runtime/RHI/Exar/ExarBufferView.hpp"

#include <algorithm>
#include <random>

UnasciiEngine::RHI::EXAR::BufferAllocTest::BufferAllocTest()
	: mDevice(std::make_unique<ExarDevice>())
{
	bool lMemoryCreated = mDevice->createMemory(1024 * 1024 * 1024);
	if (!lMemoryCreated)
	{
		std::cerr << "Memory can't created, maybe no space remaining" << std::endl;
		return;
	}

	ExarBufferDesc lBufferDesc;
	lBufferDesc.size = 1024 * 1024;
	lBufferDesc.bindFlags = ExarBindFlag::EXAR_BIND_CONSTANT_BUFFER;
	lBufferDesc.usage = ExarUsage::EXAR_USAGE_DEFAULT;

	IExarBuffer* lBuffer = mDevice->createBuffer(lBufferDesc);
	if (!lBuffer)
	{
		std::cerr << "Can't create buffer" << std::endl;
		return;
	}

	ExarMemoryRequirement lMemRequired = mDevice->getBufferMemoryRequirements(lBuffer);

	ExarAllocatorDesc lAllocDesc;
	lAllocDesc.align = ExarAlignMemory::EXAR_ALIGN_32;
	lAllocDesc.allocLocation = ExarAllocLocation::EXAR_ALLOC_HEAP;
	lAllocDesc.totalSize = lMemRequired.sizeInBytes;

	MemHandle lAddAllocated = mDevice->allocateResourceMemory(lAllocDesc, lMemRequired);
	if (!lAddAllocated)
	{
		std::cerr << "Error to trying allocate" << std::endl;
		return;
	}

	std::vector<u8> lData = initRandomBufferValue(1024 * 1024);

	ExarBufferInitData lBufferInitData;
	lBufferInitData.size = 1024 * 1024;
	lBufferInitData.data = lData.data();

	bool lDataUpdated = mDevice->updateResourceData(lAddAllocated, lMemRequired, lBufferInitData);
	if (!lDataUpdated)
	{
		std::cerr << "Error when trying update resource data" << std::endl;
		return;
	}

	bool lBufferBinded = mDevice->bindResource(lBuffer, lAddAllocated);
	if (!lDataUpdated)
	{
		std::cerr << "Error when trying binding resouce" << std::endl;
		return;
	}

	std::cout << "-------- Buffer view --------" << std::endl;

	ExarBufferView lBufferView = lBuffer->getView();
	std::cout << "{" << lBufferView.get<u32>(0) << "}" << std::endl;
	std::cout << "{" << lBufferView.get<u32>(1) << "}" << std::endl;
	std::cout << "{" << lBufferView.get<u32>(2) << "}" << std::endl;
	std::cout << "{" << lBufferView.get<u32>(3) << "}" << std::endl;



	ExarBufferDesc lBufferDesc2;
	lBufferDesc2.size = 1024;
	lBufferDesc2.bindFlags = ExarBindFlag::EXAR_BIND_CONSTANT_BUFFER;
	lBufferDesc2.usage = ExarUsage::EXAR_USAGE_DEFAULT;

	IExarBuffer* lBuffer2 = mDevice->createBuffer(lBufferDesc2);
	if (!lBuffer2)
	{
		std::cerr << "Can't create buffer" << std::endl;
		return;
	}

	ExarMemoryRequirement lMemRequired2 = mDevice->getBufferMemoryRequirements(lBuffer2);

	ExarAllocatorDesc lAllocDesc2;
	lAllocDesc2.align = ExarAlignMemory::EXAR_ALIGN_16;
	lAllocDesc2.allocLocation = ExarAllocLocation::EXAR_ALLOC_HEAP;
	lAllocDesc2.totalSize = lMemRequired2.sizeInBytes;

	MemHandle lAddAllocated2 = mDevice->allocateResourceMemory(lAllocDesc2, lMemRequired2);
	if (!lAddAllocated2)
	{
		std::cerr << "Error to trying allocate" << std::endl;
		return;
	}

	std::vector<u8> lData2 = initRandomBufferValue(1024);

	ExarBufferInitData lBufferInitData2;
	lBufferInitData2.size = 1024;
	lBufferInitData2.data = lData2.data();

	bool lDataUpdated2 = mDevice->updateResourceData(lAddAllocated2, lMemRequired2, lBufferInitData2);
	if (!lDataUpdated2)
	{
		std::cerr << "Error when trying update resource data" << std::endl;
		return;
	}

	bool lBufferBinded2 = mDevice->bindResource(lBuffer2, lAddAllocated2);
	if (!lDataUpdated2)
	{
		std::cerr << "Error when trying binding resouce" << std::endl;
		return;
	}

	std::cout << "-------- Buffer view --------" << std::endl;

	ExarBufferView lBufferView2 = lBuffer2->getView();
	std::cout << "{" << lBufferView2.get<u32>(0) << "}" << std::endl;
	std::cout << "{" << lBufferView2.get<u32>(1) << "}" << std::endl;
	std::cout << "{" << lBufferView2.get<u32>(2) << "}" << std::endl;
	std::cout << "{" << lBufferView2.get<u32>(3) << "}" << std::endl;

	/*for (ExarBufferView::iterator it = lBufferView.begin(); it != lBufferView.end(); ++it)
	{
		std::cout << "{" << *it << "}" << std::endl;
	}*/
}

UnasciiEngine::RHI::EXAR::BufferAllocTest::~BufferAllocTest()
{
}

std::vector<UnasciiEngine::u8> UnasciiEngine::RHI::EXAR::BufferAllocTest::initRandomBufferValue(size_t pSize)
{
	std::vector<u8> testBuffer(pSize, 0);

	// OPTIONNEL : Remplir avec des données aléatoires pour un vrai test
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	std::generate(testBuffer.begin(), testBuffer.end(), [&]() {
		return static_cast<u8>(dis(gen));
	});

	return testBuffer;
}
