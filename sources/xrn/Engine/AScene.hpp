#pragma once

#include <xrn/Engine/Vulkan/Window.hpp>
#include <xrn/Engine/Vulkan/Device.hpp>
#include <xrn/Engine/Vulkan/Renderer.hpp>
#include <xrn/Engine/Vulkan/FrameInfo.hpp>
#include <xrn/Engine/Vulkan/Descriptor.hpp>
#include <xrn/Engine/System/Graphic/Render.hpp>
#include <xrn/Engine/System/Graphic/PointLight.hpp>
#include <xrn/Engine/System/MoveControlled.hpp>
#include <xrn/Engine/System/UpdateTransform3d.hpp>
#include <xrn/Engine/Camera.hpp>
#include <xrn/Engine/MousePointer.hpp>

namespace xrn::engine {

///////////////////////////////////////////////////////////////////////////
/// \brief No clue what it does xD
/// \ingroup vulkan
///
/// \include AScene.hpp <AScene.hpp>
///
///////////////////////////////////////////////////////////////////////////
class AScene {

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
    explicit AScene(
        bool isCameraDetached
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
    virtual ~AScene();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    AScene(
        const AScene&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        const AScene&
    ) noexcept
        -> AScene& = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    AScene(
        AScene&&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        AScene&&
    ) noexcept
        -> AScene& = delete;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Events
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// if return false, scene exits
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] virtual auto onUpdate()
        -> bool = 0;

    ///////////////////////////////////////////////////////////////////////////
    /// if return false, scene exits
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] virtual auto onPostUpdate()
        -> bool = 0;

    ///////////////////////////////////////////////////////////////////////////
    /// called with the m_tickFrequencyTime per seconds
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] virtual auto onTick(
        ::xrn::Time deltaTime
    ) -> bool = 0;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Handles the default non-user behaviors
    ///////////////////////////////////////////////////////////////////////////
    void onSystemKeyPressed(
        ::std::int16_t keyCode
    );

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Handles the default non-user behaviors
    ///////////////////////////////////////////////////////////////////////////
    void onSystemKeyReleased(
        ::std::int16_t keyCode
    );

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Handles the default non-user behaviors
    ///////////////////////////////////////////////////////////////////////////
    void onSystemMouseMoved(
        ::glm::vec2 position
    );

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    virtual void onKeyPressed(
        ::std::int16_t keyCode
    ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    virtual void onKeyReleased(
        ::std::int16_t keyCode
    ) = 0;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    virtual void onMouseMoved(
        ::glm::vec2 position
    ) = 0;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Basic
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void run();

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto update()
        -> bool;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void draw();

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void limitFrameRate();



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Getters/Setters
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief set the m_tickFrequencyTime to match the given frequencya
    ///
    ///////////////////////////////////////////////////////////////////////////
    void setTickFrequency(
        ::std::size_t frequency
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getWindow()
        -> ::xrn::engine::vulkan::Window&;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getVulkanDevice()
        -> ::xrn::engine::vulkan::Device&;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getRegistry()
        -> ::entt::registry&;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getCameraId()
        -> ::entt::entity;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getPlayerId()
        -> ::entt::entity;

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getMousePointer()
        -> ::xrn::engine::MousePointer&;



protected:

    bool m_isCameraDetached;



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
    void updateCamera();



private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Member variables
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // Vulkan
    ::xrn::engine::vulkan::Window m_window{ true };
    ::xrn::engine::vulkan::Device m_device{ m_window };

    ::std::unique_ptr<::xrn::engine::vulkan::descriptor::SetLayout> m_pDescriptorSetLayout;
    ::std::unique_ptr<::xrn::engine::vulkan::descriptor::Pool> m_pDescriptorPool;
    ::std::vector<::VkDescriptorSet> m_descriptorSets{ ::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT };
    ::xrn::engine::vulkan::FrameInfo m_frameInfo;

    ::std::vector<::std::unique_ptr<::xrn::engine::vulkan::Buffer>> m_uboBuffers;
    ::xrn::engine::vulkan::Renderer m_renderer{ m_window, m_device };

    // systems
    ::xrn::engine::system::graphic::Render m_draw3d;
    ::xrn::engine::system::graphic::PointLight m_drawPointLight;
    ::xrn::engine::system::MoveControlled m_moveControlled;
    ::xrn::engine::system::UpdateTransform3d m_updateTransform3d;

    // Tick
    ::xrn::Clock m_tickClock;
    ::xrn::Time m_tickFrequencyTime;

    // ECS
    ::entt::registry m_registry;
    ::entt::entity m_player;

    // View
    ::xrn::engine::Camera m_camera;
    ::xrn::engine::MousePointer m_mousePointer;

};

} // namespace xrn::engine
