#pragma once

#include <xrn/Engine/Vulkan/Device.hpp>
#include <xrn/Engine/Vulkan/Descriptor/SetLayout.hpp>
#include <xrn/Engine/Vulkan/Descriptor/Pool.hpp>

namespace xrn::engine::vulkan::descriptor {

class Writer {
 public:
  Writer(::xrn::engine::vulkan::descriptor::SetLayout &setLayout, ::xrn::engine::vulkan::descriptor::Pool &pool);

  Writer &writeBuffer(::std::uint32_t binding, ::VkDescriptorBufferInfo *bufferInfo);
  Writer &writeImage(::std::uint32_t binding, ::VkDescriptorImageInfo *imageInfo);

  bool build(::VkDescriptorSet &set);
  void overwrite(::VkDescriptorSet &set);

 private:
  ::xrn::engine::vulkan::descriptor::SetLayout &setLayout;
  ::xrn::engine::vulkan::descriptor::Pool &pool;
  ::std::vector<::VkWriteDescriptorSet> writes;
};

}  // namespace xrn::engine::vulkan::descriptor
