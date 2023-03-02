#pragma once

#include <xrn/Engine/Vulkan/Device.hpp>

namespace xrn::engine::vulkan::descriptor {

class Writer;

class Pool {
 public:
  class Builder {
   public:
    Builder(::xrn::engine::vulkan::Device &device) : device{device} {}

    Builder &addPoolSize(::VkDescriptorType descriptorType, ::std::uint32_t count);
    Builder &setPoolFlags(::VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(::std::uint32_t count);
    ::std::unique_ptr<::xrn::engine::vulkan::descriptor::Pool> build() const;

   private:
    ::xrn::engine::vulkan::Device &device;
    ::std::vector<::VkDescriptorPoolSize> poolSizes{};
    ::std::uint32_t maxSets = 1000;
    ::VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  Pool(
      ::xrn::engine::vulkan::Device &device,
      ::std::uint32_t maxSets,
      ::VkDescriptorPoolCreateFlags poolFlags,
      const ::std::vector<::VkDescriptorPoolSize> &poolSizes);
  ~Pool();
  Pool(const ::xrn::engine::vulkan::descriptor::Pool &) = delete;
  Pool &operator=(const ::xrn::engine::vulkan::descriptor::Pool &) = delete;

  bool allocateDescriptor(
      const ::VkDescriptorSetLayout descriptorSetLayout, ::VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<::VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  ::xrn::engine::vulkan::Device &device;
  ::VkDescriptorPool descriptorPool;

  friend class ::xrn::engine::vulkan::descriptor::Writer;
};

}  // namespace xrn::engine::vulkan
