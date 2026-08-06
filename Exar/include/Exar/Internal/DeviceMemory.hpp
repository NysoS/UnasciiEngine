#ifndef EXAR_DEVICE_MEMORY_HPP
#define EXAR_DEVICE_MEMORY_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar
{
	class AreaMemory;
	struct AreaMemoryCreateInfo;

	class DeviceMemory final
	{
	public:
		explicit DeviceMemory(size_t pSize);
		~DeviceMemory();

		Result createAreaMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram);
		Memory clearAreaMemory();

		DeviceMemory(const DeviceMemory&) = delete;
		DeviceMemory& operator=(const DeviceMemory&) = delete;

		AreaMemory* getAreaMemory(const AreaMemoryType& pAreaType) const;

		inline std::vector<const AreaMemory*> getAreaFamilies() noexcept {
			return {
				mSwapchainArea.get(),
				mCommandPoolArea.get(),
				mFenceArea.get()
			};
		}

	private:
		std::unique_ptr<AreaMemory> mSwapchainArea;
		std::unique_ptr<AreaMemory> mCommandPoolArea;
		std::unique_ptr<AreaMemory> mFenceArea;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
