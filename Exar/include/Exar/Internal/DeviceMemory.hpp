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

		inline std::vector<const AreaMemory*> getFamilies() noexcept {
			return {
				mSwapchainArea.get()
			};
		}

	private:
		std::unique_ptr<AreaMemory> mSwapchainArea;
	};
}

#endif // !EXAR_DEVICE_MEMORY_HPP
