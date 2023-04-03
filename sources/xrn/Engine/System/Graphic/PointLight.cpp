///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/System/Graphic/PointLight.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::system::graphic::PointLight::PointLight(
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
::xrn::engine::system::graphic::PointLight::~PointLight()
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
void ::xrn::engine::system::graphic::PointLight::createPipelineLayout(
    ::VkDescriptorSetLayout descriptorSetLayout
)
{
    ::VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = ::VK_SHADER_STAGE_VERTEX_BIT | ::VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(::xrn::engine::component::PointLight::PushConstant);

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
void ::xrn::engine::system::graphic::PointLight::createPipeline(
    ::VkRenderPass renderPass
)
{
    XRN_SASSERT(m_pipelineLayout != nullptr, "Cannot create pipeline before pipeline layout");

    ::xrn::engine::vulkan::Pipeline::Configuration pipelineConfig{};
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_pPipeline = ::std::make_unique<::xrn::engine::vulkan::Pipeline>(m_device, pipelineConfig, "pointLight");
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::PointLight::operator()(
    ::xrn::engine::vulkan::FrameInfo& frameInfo
    , const ::xrn::engine::component::PointLight& pointLight
    , const ::xrn::engine::component::Position& position
    , ::std::size_t lightIndex
) const
{
    frameInfo.ubo.pointLights[lightIndex] = ::xrn::engine::component::PointLight::PushConstant{ pointLight, position };
    // frameInfo.ubo.pointLights[lightIndex] = ::xrn::engine::component::PointLight::PushConstant{ pointLight, position };
    ::xrn::engine::component::PointLight::PushConstant pushConstant{ pointLight, position };
    ::vkCmdPushConstants(
        frameInfo.commandBuffer
        , m_pipelineLayout
        , VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
        , 0
        , sizeof(pushConstant)
        , &pushConstant
    );
    ::vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::PointLight::bind(
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

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::graphic::PointLight::draw(
    ::xrn::engine::vulkan::FrameInfo& frameInfo
    , const ::xrn::engine::component::PointLight& pointLight
    , const ::xrn::engine::component::Position& position
    , ::std::size_t lightIndex
) const
{
    frameInfo.ubo.pointLights[lightIndex] = ::xrn::engine::component::PointLight::PushConstant{ pointLight, position };
}
