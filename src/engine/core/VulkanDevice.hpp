#ifndef RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
#define RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"

#include <vulkan/vulkan_core.h>

#include <array>
#include <optional>
#include <set>
#include <source_location>
#include <vector>

namespace rr
{

/// \brief \ref SwapchainSupportDetails is a collection of information about the Swapchain
///
/// \ref SwapchainSupportDetails saves information about the capabilities, format and present
/// mode of the surface.
///
/// \author Felix Hommel
/// \date 5/26/2025
struct SwapchainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

/// \brief \ref QueueFamilyIndices is a collection about different Queues that are utilized by the Renderer
///
/// \ref QueueFamilyIndices stores the handles of a graphics and present Queue family. In addition, it implements
/// several convenience and utility functions.
///
/// \author Felix Hommel
/// \date 5/26/2025
struct QueueFamilyIndices
{
	std::optional<std::uint32_t> graphicsFamily;
	std::optional<std::uint32_t> presentFamily;

	[[nodiscard]] constexpr bool isComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
	[[nodiscard]] constexpr bool areSameQueue() const
	{
		if (graphicsFamily.has_value() && presentFamily.has_value())
			return graphicsFamily.value() == presentFamily.value();

		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);
	}

	[[nodiscard]] std::set<std::uint32_t> getUniqueFamilies() const
	{
		return { graphicsFamily.value_or(0), presentFamily.value_or(0) };
	}
	[[nodiscard]] constexpr std::array<std::uint32_t, 2> toAray() const
	{
		if (graphicsFamily.has_value() && presentFamily.has_value())
			return { graphicsFamily.value(), presentFamily.value() };

		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);
	}
};

/// \brief \ref VulkanDevice is a wrapper around VkDevice and VkPhysicalDevice
///
/// \ref VulkanDevice manages a VkDevice, VkPhysicalDevice nad all Queues associated
/// with the VkDevice.
///
/// @author Felix Hommel
/// @date 5/26/2025
class VulkanDevice
{
public:
	VulkanDevice(VkInstance instance, VkSurfaceKHR surface);
	~VulkanDevice();

	VulkanDevice(const VulkanDevice&) = delete;
	VulkanDevice(VulkanDevice&&) = delete;
	VulkanDevice& operator=(const VulkanDevice&) = delete;
	VulkanDevice& operator=(VulkanDevice&&) = delete;

	[[nodiscard]] VkDevice getHandle() const { return m_device; }
	[[nodiscard]] VkQueue getGraphicsQueueHandle() const { return m_graphicsQueue; }
	[[nodiscard]] VkQueue getPresentQueueHandle() const { return m_presentQueue; }

	/// \brief Determine the swapchain support of the VkPhysicalDevice
	///
	/// \return \ref SwapchainSupportDetails
	/// \sa SwapchainSupportDetails
	[[nodiscard]] SwapchainSupportDetails getSwapchainSupport() const
	{
		return querySwapchainSupport(m_physicalDevice);
	}

	/// \brief Determine which queue families the VkPhysicalDevice is supporting
	///
	/// \return \ref QueueFamilyIndices
	/// \sa QueueFamilyIndices
	[[nodiscard]] QueueFamilyIndices findPhysicalQueueFamilies() const
	{
		return findQueueFamilies(m_physicalDevice);
	}

	[[nodiscard]] VkFormat findSupportedFormat(
		const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	void createImageWithInfo(
		const VkImageCreateInfo& createInfo,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory) const;
	void createBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory) const;

private:
	VkInstance instance;
	VkSurfaceKHR surface;

	VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
	VkPhysicalDeviceProperties m_physicalDeviceProperties{};
	VkDevice m_device{ VK_NULL_HANDLE };
	VkQueue m_graphicsQueue{ VK_NULL_HANDLE };
	VkQueue m_presentQueue{ VK_NULL_HANDLE };

	const std::vector<const char*> deviceExtensions{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	}; ///< List of extensions that the device must support

	void pickPhysicalDevice();
	void createLogicalDevice();

	bool isPhysicalDeviceSuitable(VkPhysicalDevice device) const;
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
	bool checkDeviceExtensionsSupported(VkPhysicalDevice device) const;
	SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device) const;
	std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};

} // namespace rr

#endif // !RRENDERER_ENGINE_CORE_VULKAN_DEVICE_HPP
