#pragma once

#include <xrn/Engine/Vulkan/Device.hpp>
#include <xrn/Engine/Vulkan/Pipeline.hpp>
#include <xrn/Engine/Camera.hpp>
#include <xrn/Engine/Vulkan/FrameInfo.hpp>
#include <xrn/Engine/Component/Transform3d.hpp>

namespace xrn::engine::system {

///////////////////////////////////////////////////////////////////////////
/// \brief No clue what it does xD
/// \ingroup vulkan
///
/// \include PointLight.hpp <PointLight.hpp>
///
///////////////////////////////////////////////////////////////////////////
class PointLight {

public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    explicit PointLight(
        ::xrn::engine::vulkan::Device& device
        , ::VkRenderPass renderPass
        , ::VkDescriptorSetLayout descriptorSetLayout
    );



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Rule of 5
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    ~PointLight();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    PointLight(
        const PointLight&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        const PointLight&
    ) noexcept
        -> PointLight& = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    PointLight(
        PointLight&&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        PointLight&&
    ) noexcept
        -> PointLight& = delete;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Basic
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void operator()(
        ::xrn::engine::vulkan::FrameInfo& frameInfo
        , const ::xrn::engine::component::PointLight& pointLight
        , const ::xrn::engine::component::Position& position
        , ::std::size_t lightIndex
    ) const;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void bind(
        ::xrn::engine::vulkan::FrameInfo& frameInfo
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void draw(
        ::xrn::engine::vulkan::FrameInfo& frameInfo
        , ::xrn::engine::component::PointLight& pointLight
        , ::xrn::engine::component::Position& position
        , ::std::size_t lightIndex
    );




private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Helpers
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void createPipelineLayout(
        ::VkDescriptorSetLayout descriptorSetLayout
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void createPipeline(VkRenderPass renderPass);



private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Member variables
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ::xrn::engine::vulkan::Device& m_device;
    ::std::unique_ptr<::xrn::engine::vulkan::Pipeline> m_pPipeline;
    ::VkPipelineLayout m_pipelineLayout;


};

} // namespace xrn::engine::system
