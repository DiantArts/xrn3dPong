#pragma once

#include <xrn/Engine/Vulkan/Device.hpp>

namespace xrn::engine::vulkan::descriptor {

class Writer;

class SetLayout {
 public:
  class Builder {
   public:
    Builder(::xrn::engine::vulkan::Device &device) : device{device} {}

    Builder &addBinding(
        ::std::uint32_t binding,
        ::VkDescriptorType descriptorType,
        ::VkShaderStageFlags stageFlags,
        ::std::uint32_t count = 1);
    ::std::unique_ptr<::xrn::engine::vulkan::descriptor::SetLayout> build() const;

   private:
    ::xrn::engine::vulkan::Device &device;
    ::std::unordered_map<::std::uint32_t, ::VkDescriptorSetLayoutBinding> bindings{};
  };

  SetLayout(
      ::xrn::engine::vulkan::Device &device, ::std::unordered_map<::std::uint32_t, ::VkDescriptorSetLayoutBinding> bindings);
  ~SetLayout();
  SetLayout(const ::xrn::engine::vulkan::descriptor::SetLayout &) = delete;
  SetLayout &operator=(const ::xrn::engine::vulkan::descriptor::SetLayout &) = delete;

  ::VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  ::xrn::engine::vulkan::Device &device;
  ::VkDescriptorSetLayout descriptorSetLayout;
  ::std::unordered_map<::std::uint32_t, ::VkDescriptorSetLayoutBinding> bindings;

  friend class ::xrn::engine::vulkan::descriptor::Writer;
};

}  // namespace xrn::engine::vulkan
