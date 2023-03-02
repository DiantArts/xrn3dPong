#include <pch.hpp>
#include <xrn/Engine/Vulkan/Descriptor/Pool.hpp>

// *************** Descriptor Pool Builder *********************

::xrn::engine::vulkan::descriptor::Pool::Builder &::xrn::engine::vulkan::descriptor::Pool::Builder::addPoolSize(
    VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

::xrn::engine::vulkan::descriptor::Pool::Builder &::xrn::engine::vulkan::descriptor::Pool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}
::xrn::engine::vulkan::descriptor::Pool::Builder &::xrn::engine::vulkan::descriptor::Pool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

auto ::xrn::engine::vulkan::descriptor::Pool::Builder::build() const -> std::unique_ptr<::xrn::engine::vulkan::descriptor::Pool> {
  return std::make_unique<::xrn::engine::vulkan::descriptor::Pool>(device, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

::xrn::engine::vulkan::descriptor::Pool::Pool(
    ::xrn::engine::vulkan::Device &device,
    uint32_t maxSets,
    ::VkDescriptorPoolCreateFlags poolFlags,
    const ::std::vector<::VkDescriptorPoolSize> &poolSizes)
    : device{device} {
  ::VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = static_cast<::std::uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes = poolSizes.data();
  descriptorPoolInfo.maxSets = maxSets;
  descriptorPoolInfo.flags = poolFlags;

    XRN_ASSERT(
        vkCreateDescriptorPool(device.device(), &descriptorPoolInfo, nullptr, &descriptorPool) == VK_SUCCESS,
        "Create descriptor pool"
    );
}

::xrn::engine::vulkan::descriptor::Pool::~Pool() {
  vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);
}

bool ::xrn::engine::vulkan::descriptor::Pool::allocateDescriptor(
    const ::VkDescriptorSetLayout descriptorSetLayout, ::VkDescriptorSet &descriptor) const {
  ::VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.pSetLayouts = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
  // a new pool whenever an old pool fills up. But this is beyond our current scope
  if (vkAllocateDescriptorSets(device.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
    return false;
  }
  return true;
}

void ::xrn::engine::vulkan::descriptor::Pool::freeDescriptors(::std::vector<::VkDescriptorSet> &descriptors) const {
  vkFreeDescriptorSets(
      device.device(),
      descriptorPool,
      static_cast<::std::uint32_t>(descriptors.size()),
      descriptors.data());
}

void ::xrn::engine::vulkan::descriptor::Pool::resetPool() {
  vkResetDescriptorPool(device.device(), descriptorPool, 0);
}
