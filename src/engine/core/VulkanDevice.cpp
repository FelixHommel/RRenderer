#include "VulkanDevice.hpp"

#include "constants.hpp"

#include "exception/EngineException.hpp"
#include "exception/VulkanException.hpp"
#include "spdlog/spdlog.h"
#include <source_location>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <cstring>
#include <functional>
#include <set>
#include <string>
#include <vector>

namespace rr
{

/// \brief Construct a new \ref VulkanDevice
///
/// \param instance `VkInstance` to which the VkDevice is bound
/// \param surface the `VkSurfaceKHR` that is being used
VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface)
	: instance(instance)
	, surface(surface)
{
	pickPhysicalDevice();
	createLogicalDevice();
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

/// \brief Determine which formats the `VkPhysicalDevice` is supporting
///
/// \return `VkFormat`
/// \thorws \ref VulkanException if no supported format could be found
VkFormat VulkanDevice::findSupportedFormat(
	const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
{
	for (const auto format : candidates)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features ||
			tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throwWithLog<VulkanException>(
		std::source_location::current(), VulkanExceptionCause::FIND_SUPPORTED_FORMAT);
}

/// \brief Create a new Image on the `VkDevice`
///
/// Allocate memory on the `VkDevice` for the image and then bind the image to the memory.
///
/// \param createInfo `VkImageCreateInfo&` initialized with the needed information about the image
/// \param properties properties that the image memory should fulfill
/// \param image `VkImage&` to store the image handle in
/// \param imageMemory `VkDeviceMemory&` to store the image memory in
///
/// \thorws \ref VulkanException if anything goes wrong while allocating or binding
void VulkanDevice::createImageWithInfo(
	const VkImageCreateInfo& createInfo,
	VkMemoryPropertyFlags properties,
	VkImage& image,
	VkDeviceMemory& imageMemory) const
{
	if (vkCreateImage(m_device, &createInfo, nullptr, &image) != VK_SUCCESS)
		throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_IMAGE);

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_device, image, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{ .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
										  .allocationSize = memRequirements.size,
										  .memoryTypeIndex =
											  findMemoryType(memRequirements.memoryTypeBits, properties) };

	if (vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
		throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::ALLOCATE_MEMORY);

	if (vkBindImageMemory(m_device, image, imageMemory, 0) != VK_SUCCESS)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::BIND_IMAGE_MEMORY);
}

/// \brief Create a new Buffer on the VkDevice
///
/// Allocate memory for it on the `VkDevice` and then bind the buffer to the allocated memory.
///
/// \param size size of the buffer in byte
/// \param usage how the buffer is used
/// \param properties properties that the buffer memory should fulfill
/// \param buffer `VkBuffer&` to store the buffer handle in
/// \param bufferMemory `VkDeviceMemory&` to store the memory handle in
///
/// \thorws \ref VulkanException if anything goes wrong while allocating or binding
void VulkanDevice::createBuffer(
	VkDeviceSize size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkBuffer& buffer,
	VkDeviceMemory& bufferMemory) const
{
	const VkBufferCreateInfo createInfo{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
										 .size = size,
										 .usage = usage,
										 .sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	if (vkCreateBuffer(m_device, &createInfo, nullptr, &buffer) != VK_SUCCESS)
		throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_BUFFER);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{ .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
										  .allocationSize = memRequirements.size,
										  .memoryTypeIndex =
											  findMemoryType(memRequirements.memoryTypeBits, properties) };

	if (vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::ALLOCATE_MEMORY);

	vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
}

/// \brief Choose the first suitable available physical device
///
/// \throws \ref VulkanException if no suitable device was found
void VulkanDevice::pickPhysicalDevice()
{
	std::uint32_t deviceCount{ 0 };
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::NO_PHYSICAL_DEVICE_FOUND);

	spdlog::info("Found {} vulkan compatible device(s)", deviceCount);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& dev : devices)
	{
		if (isPhysicalDeviceSuitable(dev))
		{
			m_physicalDevice = dev;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::NO_SUITABLE_DEVICE_FOUND);

	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
	spdlog::info(
		"physical device: {}\n\tID: {}\n\tvendorID: {}\n\tdeviceType: {}",
		m_physicalDeviceProperties.deviceName,
		m_physicalDeviceProperties.deviceID,
		m_physicalDeviceProperties.vendorID,
		static_cast<int>(m_physicalDeviceProperties.deviceType));
}

/// \brief Create the `VkDevice` and get the queues that are being used from the `VkDevice`
///
/// \throws \ref VulkanException if there was an error with `VkDevice` creation or Queue finding
void VulkanDevice::createLogicalDevice()
{
	QueueFamilyIndices indices{ findQueueFamilies(m_physicalDevice) };

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set uniqueQueueFamilies{ indices.getUniqueFamilies() };

	float queuePriority{ 1.f };
	for (auto queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
												 .queueFamilyIndex = queueFamily,
												 .queueCount = 1,
												 .pQueuePriorities = &queuePriority };

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{ .samplerAnisotropy = VK_TRUE };

	VkDeviceCreateInfo createInfo{ .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
								   .queueCreateInfoCount =
									   static_cast<std::uint32_t>(queueCreateInfos.size()),
								   .pQueueCreateInfos = queueCreateInfos.data(),
								   .enabledLayerCount = 0,
								   .ppEnabledLayerNames = nullptr,
								   .enabledExtensionCount =
									   static_cast<std::uint32_t>(deviceExtensions.size()),
								   .ppEnabledExtensionNames = deviceExtensions.data(),
								   .pEnabledFeatures = &deviceFeatures };

	if (useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}

	if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
		throwWithLog<VulkanException>(std::source_location::current(), VulkanExceptionCause::CREATE_DEVICE);

	if (indices.graphicsFamily.has_value() && indices.presentFamily.has_value())
	{
		vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
	}
	else
		throwWithLog<VulkanException>(
			std::source_location::current(), VulkanExceptionCause::QUEUE_FAMILY_INDEX_IS_EMPTY);

	spdlog::info("Logical device created successfully...");
}

/// \brief Check whether \p device is a suitable physical device for the Renderer
///
/// A physical device is suitable if all needed queues are supported, all needed extensions are supported
/// and if it supports sampler anisotropy.
///
/// \param device candidate `VkPhysicalDevice`
///
/// \return `true` if \p device is suitable, `false` if not
bool VulkanDevice::isPhysicalDeviceSuitable(VkPhysicalDevice device) const
{
	const QueueFamilyIndices indices{ findQueueFamilies(device) };
	const bool extensionsSupported{ checkDeviceExtensionsSupported(device) };
	bool swapchainSuitable{ false };

	if (extensionsSupported)
	{
		SwapchainSupportDetails swapchainSupport{ querySwapchainSupport(device) };
		swapchainSuitable = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapchainSuitable &&
		   static_cast<bool>(supportedFeatures.samplerAnisotropy);
}

/// \brief Find the Queue families present on \p device
///
/// \param device `VkPhysicalDevice` where the queue families are searched on
///
/// \return \ref QueueFamilyIndices
/// \sa QueueFamilyIndices
QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device) const
{
	QueueFamilyIndices indices;

	std::uint32_t queueFamilyCount{ 0 };
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i{ 0 };
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && static_cast<bool>(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			indices.graphicsFamily.emplace(i);

		VkBool32 presentSupport{ VK_FALSE };
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && static_cast<bool>(presentSupport))
			indices.presentFamily.emplace(i);

		if (indices.isComplete())
			break;

		++i;
	}

	return indices;
}

/// \brief Check if \p device supports all required extensions
///
/// \param device candidate `VkPhysicalDevice`
///
/// \return `true` if \p device supports all required extensions, `false` if not
bool VulkanDevice::checkDeviceExtensionsSupported(VkPhysicalDevice device) const
{
	std::uint32_t extensionCount{ 0 };
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string, std::less<>> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}

/// \brief Get the needed information about what Swapchain features \p supports
///
/// \param device `VkPhysicalDevice`
///
/// \return \ref SwapchainSupportDetails
/// \sa SwapchainSupportDetails
SwapchainSupportDetails VulkanDevice::querySwapchainSupport(VkPhysicalDevice device) const
{
	SwapchainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	std::uint32_t formatCount{ 0 };
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	std::uint32_t presentModeCount{ 0 };
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
			device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

/// \brief Find suitable memory type on the physical device
///
/// Memory type is suitable if it matches \p typeFilter and fulfills all \p properties
///
/// \param typeFilter the type of memory that is needed
/// \param properties `VkMemoryPropertyFlags` that specify what properties the memory must have
///
/// \return index of the suitable memory type
/// \throws \ref VulkanException if no suitable memory type was found
std::uint32_t VulkanDevice::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (std::uint32_t i{ 0 }; i < memProperties.memoryTypeCount; ++i)
	{
		if (static_cast<bool>(typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties) //NOLINT
			return i;
	}

	throwWithLog<VulkanException>(
		std::source_location::current(), VulkanExceptionCause::NO_SUITABLE_MEMORY_TYPE_FOUND);
}

} // namespace rr
