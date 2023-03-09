#include <pch.hpp>
#include <xrn/Engine/Vulkan/Descriptor/Writer.hpp>

::xrn::engine::vulkan::descriptor::Writer::Writer(
    ::xrn::engine::vulkan::descriptor::SetLayout &setLayout,
    ::xrn::engine::vulkan::descriptor::Pool &pool
)
    : setLayout{setLayout}, pool{pool}
{}

::xrn::engine::vulkan::descriptor::Writer &::xrn::engine::vulkan::descriptor::Writer::writeBuffer(
    ::std::uint32_t binding, ::VkDescriptorBufferInfo *bufferInfo) {
  XRN_SASSERT(setLayout.bindings.count(binding) == 1, "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  XRN_SASSERT(
      bindingDescription.descriptorCount == 1,
      "Binding single descriptor info, but binding expects multiple");

  ::VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pBufferInfo = bufferInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

::xrn::engine::vulkan::descriptor::Writer &::xrn::engine::vulkan::descriptor::Writer::writeImage(
    ::std::uint32_t binding, ::VkDescriptorImageInfo *imageInfo) {
  XRN_SASSERT(setLayout.bindings.count(binding) == 1, "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  XRN_SASSERT(
      bindingDescription.descriptorCount == 1,
      "Binding single descriptor info, but binding expects multiple");

  ::VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pImageInfo = imageInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

bool ::xrn::engine::vulkan::descriptor::Writer::build(::VkDescriptorSet &set) {
  bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) {
    return false;
  }
  overwrite(set);
  return true;
}

void ::xrn::engine::vulkan::descriptor::Writer::overwrite(::VkDescriptorSet &set) {
  for (auto &write : writes) {
    write.dstSet = set;
  }
  vkUpdateDescriptorSets(pool.device.device(), static_cast<::std::uint32_t>(writes.size()), writes.data(), 0, nullptr);
}
