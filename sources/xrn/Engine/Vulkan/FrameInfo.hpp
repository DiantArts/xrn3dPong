#pragma once

#include <xrn/Util.hpp>
#include <xrn/Engine/Vulkan/Ubo.hpp>

namespace xrn::engine { class AScene; }

namespace xrn::engine::vulkan {

///////////////////////////////////////////////////////////////////////////
/// \brief No clue what it does xD
/// \ingroup vulkan
///
/// \include FrameInfo.hpp <FrameInfo.hpp>
///
///////////////////////////////////////////////////////////////////////////
struct FrameInfo {

    explicit FrameInfo (
        ::std::vector<::VkDescriptorSet>& parentDescriptorSets,
        ::xrn::engine::AScene& parentScene
    );

    ::std::size_t frameIndex;
    ::xrn::Time deltaTime;

    ::VkCommandBuffer commandBuffer;
    ::xrn::engine::vulkan::Ubo ubo;
    ::glm::mat4 projectionView;

    ::std::vector<::VkDescriptorSet>& descriptorSets;

    ::xrn::engine::AScene& scene;
};

} // namespace xrn::engine::vulkan
