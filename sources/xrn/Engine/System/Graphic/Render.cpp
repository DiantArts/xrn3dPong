///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/System/Graphic/Render.hpp>



struct SimplePushConstantData {
    ::glm::mat4 modelMatrix{ 1.0f };
    ::glm::mat4 normalMatrix{ 1.0f };
};



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::system::graphic::Render::Render(
    ::xrn::engine::vulkan::Device& device
    , ::VkRenderPass renderPass
    , ::VkDescriptorSetLayout descriptorSetLayout
)
    : m_device{ device }
{
    this->createPipelineLayout(descriptorSetLayout);
    this->createPipeline(renderPass);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::system::graphic::Render::~Render()
{
    ::vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Basic
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::Render::createPipelineLayout(
    ::VkDescriptorSetLayout descriptorSetLayout
)
{
    ::VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = ::VK_SHADER_STAGE_VERTEX_BIT | ::VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(::SimplePushConstantData);

    ::std::vector<::VkDescriptorSetLayout> descriptorSetLayouts{ descriptorSetLayout };

    ::VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = ::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<::std::uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    XRN_ASSERT(
        ::vkCreatePipelineLayout(
            m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout
        ) == ::VK_SUCCESS
        , "Create pipeline layout"
    );
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::Render::createPipeline(
    ::VkRenderPass renderPass
)
{
    XRN_SASSERT(m_pipelineLayout != nullptr, "Cannot create pipeline before pipeline layout");

    ::xrn::engine::vulkan::Pipeline::Configuration pipelineConfig{};
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    pipelineConfig.enableAlphaBlending();
    m_pPipeline = ::std::make_unique<::xrn::engine::vulkan::Pipeline>(m_device, pipelineConfig, "simple");
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::Render::operator()(
    ::xrn::engine::vulkan::FrameInfo& frameInfo
    , ::xrn::engine::component::Transform3d& transform
) const
{
    ::SimplePushConstantData push{
        .modelMatrix = transform.getMatrix()
        , .normalMatrix = transform.getNormalMatrix()
    };

    ::vkCmdPushConstants(
        frameInfo.commandBuffer
        , m_pipelineLayout
        , ::VK_SHADER_STAGE_VERTEX_BIT | ::VK_SHADER_STAGE_FRAGMENT_BIT
        , 0
        , sizeof(::SimplePushConstantData)
        , &push
    );
    transform.getModel().bind(frameInfo.commandBuffer);
    transform.getModel().draw(frameInfo.commandBuffer);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::Render::bind(
    ::xrn::engine::vulkan::FrameInfo& frameInfo
)
{
    m_pPipeline->bind(frameInfo.commandBuffer);
    ::vkCmdBindDescriptorSets(
        frameInfo.commandBuffer
        , VK_PIPELINE_BIND_POINT_GRAPHICS
        , m_pipelineLayout
        , 0
        , 1
        , &frameInfo.descriptorSets[frameInfo.frameIndex]
        , 0
        , nullptr
    );
}
