///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/AScene.hpp>
#include <xrn/Engine/Vulkan/Buffer.hpp>
#include <xrn/Engine/Configuration.hpp>
#include <xrn/Engine/Components.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::AScene::AScene(
    bool isCameraDetached
)
    : m_isCameraDetached{ isCameraDetached }
    // create the vulkan things (I dont remember what it does)
    , m_pDescriptorSetLayout{ ::xrn::engine::vulkan::descriptor::SetLayout::Builder{ m_device }
        .addBinding(0, ::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ::VK_SHADER_STAGE_ALL_GRAPHICS)
        .build()
    }
    // Keep track of information to help vulkan
    , m_frameInfo{ m_descriptorSets, *this }
    , m_uboBuffers{ ::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT }
    , m_draw3d{
        m_device
        , m_renderer.getSwapChainRenderPass()
        , m_pDescriptorSetLayout->getDescriptorSetLayout()
    }
    // draw lights and update their position
    , m_drawPointLight{
        m_device
        , m_renderer.getSwapChainRenderPass()
        , m_pDescriptorSetLayout->getDescriptorSetLayout()
    }
    , m_tickFrequencyTime{
        ::xrn::Time::createAsSeconds(1) / ::xrn::engine::Configuration::defaultTickFrequency
    }
    , m_player{ m_registry.create() }
    , m_camera{ m_isCameraDetached ? m_player : m_registry.create() }
{
    // vulkan stuff
    m_pDescriptorPool = ::xrn::engine::vulkan::descriptor::Pool::Builder{ m_device }
        .setMaxSets(::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ::xrn::engine::vulkan::SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();

    for (auto& pUbo : m_uboBuffers) {
        pUbo = ::std::make_unique<::xrn::engine::vulkan::Buffer>(
            m_device
            , sizeof(::xrn::engine::vulkan::Ubo)
            , 1
            , ::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
            , ::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        );
        pUbo->map();
    }

    for (auto i{ 0uz }; i < m_descriptorSets.size(); ++i) {
        auto bufferInfo{ m_uboBuffers[i]->descriptorInfo() };
        ::xrn::engine::vulkan::descriptor::Writer{ *m_pDescriptorSetLayout, *m_pDescriptorPool }
            .writeBuffer(0, &bufferInfo)
            .build(m_descriptorSets[i]);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::AScene::~AScene() = default;




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::onSystemKeyPressed(
    ::std::int16_t keyCode
)
{

    // default
    if (keyCode == ::xrn::engine::configuration.keyBinding.closeWindow) {
        return this->getWindow().close();
    }

    // if player is controllable
    if (m_isCameraDetached) {
        if (
            auto* playerController{ m_registry.try_get<::xrn::engine::component::Control>(m_player) };
            playerController
        ) {
            // move
            if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
                return playerController->startMovingForward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
                return playerController->startMovingBackward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
                return playerController->startMovingLeft();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
                return playerController->startMovingRight();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveUp) {
                return playerController->startMovingUp();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveDown) {
                return playerController->startMovingDown();

            // move arrows
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
                return playerController->startMovingForward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
                return playerController->startMovingBackward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
                return playerController->startMovingLeft();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
                return playerController->startMovingRight();

            // look
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookUp) {
                return playerController->rotateZ(-45 / 2);
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookDown) {
                return playerController->rotateZ(45 / 2);
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookLeft) {
                return playerController->rotateX(45 / 2);
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookRight) {
                return playerController->rotateX(-45 / 2);
            }
        }
    } else {
        return this->onKeyPressed(keyCode);
    }

    XRN_INFO("Key ({}) is not bound to press", keyCode);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::onSystemKeyReleased(
    ::std::int16_t keyCode
)
{
    // if player is controllable
    if (m_isCameraDetached) {
        if (
            auto* playerController{ m_registry.try_get<::xrn::engine::component::Control>(m_player) };
            playerController
        ) {
            // move
            if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
                return playerController->stopMovingForward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
                return playerController->stopMovingBackward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
                return playerController->stopMovingLeft();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
                return playerController->stopMovingRight();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveUp) {
                return playerController->stopMovingUp();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveDown) {
                return playerController->stopMovingDown();

            // move arrows
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
                return playerController->stopMovingForward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
                return playerController->stopMovingBackward();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
                return playerController->stopMovingLeft();
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
                return playerController->stopMovingRight();

            // look
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookUp) {
                return;
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookDown) {
                return;
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookLeft) {
                return;
            } else if (keyCode == ::xrn::engine::configuration.keyBinding.lookRight) {
                return;
            }
        }
    } else {
        return this->onKeyReleased(keyCode);
    }

    XRN_INFO("Key ({}) is not bound to press", keyCode);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::onSystemMouseMoved(
    ::glm::vec2 offset
)
{
    if (m_isCameraDetached) {
        if (
            auto* playerController{ m_registry.try_get<::xrn::engine::component::Control>(m_player) };
            playerController
        ) {
            playerController->rotateX(-offset.x);
            playerController->rotateY(offset.y);
        }
    } else {
        this->onMouseMoved(offset);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Getters/Setters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::setTickFrequency(
    ::std::size_t frequency
)
{
    m_tickFrequencyTime = ::xrn::Time::createAsSeconds(frequency) / 30;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::getWindow()
    -> ::xrn::engine::vulkan::Window&
{
    return m_window;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::getVulkanDevice()
    -> ::xrn::engine::vulkan::Device&
{
    return m_device;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::getRegistry()
    -> ::entt::registry&
{
    return m_registry;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::getCameraId()
    -> ::entt::entity
{
    return m_camera.getId();
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::getPlayerId()
    -> ::entt::entity
{
    return m_player;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Basic
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::run()
{
    ::xrn::Clock clock, fpsClock;
    ::std::size_t fpsCount{ 0 };

    while (!m_window.shouldClose()) {
        ++fpsCount;
        if (fpsClock.getElapsed() >= ::xrn::Time::createAsSeconds(1)) {
            XRN_INFO("FPS: {}", fpsCount);
            m_frameInfo.fps = fpsCount;
            fpsCount = 0;
            fpsClock.reset();
        }

        m_frameInfo.deltaTime = clock.restart();

        m_window.handleEvents(*this);

        if (!this->onUpdate() || !this->update() || !this->onPostUpdate()) {
            m_window.close();
            break;
        }

        if (m_tickClock.getElapsed() >= m_tickFrequencyTime) {
            if (!this->onTick(m_tickClock.restart())) {
                m_window.close();
                break;
            }
        }

        this->draw();
        this->limitFrameRate();
    }

    ::vkDeviceWaitIdle(m_device.device());
}

float deltaT;

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::AScene::update()
    -> bool
{
    this->updateCamera();

    // control
    for (auto [entity, control]: m_registry.view<::xrn::engine::component::Control>().each()) {
        auto* position{ m_registry.try_get<::xrn::engine::component::Position>(entity) };
        auto* rotation{ m_registry.try_get<::xrn::engine::component::Rotation>(entity) };
        m_moveControlled(m_frameInfo, control, position, rotation);
    }

    // transform (apply position rotation scale)
    for (auto [entity, transform]: m_registry.view<::xrn::engine::component::Transform3d>().each()) {
        auto* position{ m_registry.try_get<::xrn::engine::component::Position>(entity) };
        auto* rotation{ m_registry.try_get<::xrn::engine::component::Rotation>(entity) };
        auto* scale{ m_registry.try_get<::xrn::engine::component::Scale>(entity) };
        m_updateTransform3d(m_frameInfo, transform, position, rotation, scale);
    }

    {
        auto lightIndex{ 0uz };
        m_registry.view<::xrn::engine::component::PointLight, ::xrn::engine::component::Position>().each(
            [this, &lightIndex](auto& pointLight, auto& position) {
                // auto rotation{ ::glm::rotate(::glm::mat4(1.0f), static_cast<float>(frameInfo.deltaTime.get()) / 1000, { 0.0f, -1.0f, 0.0f }) };
                // position = ::glm::vec3{ rotation * ::glm::vec4{ ::glm::vec3{ position }, 1.0f } };
                // m_drawPointLight.draw(m_frameInfo, pointLight, position, lightIndex);
                ++lightIndex;
            }
        );
        m_frameInfo.ubo.numOfLights = lightIndex;
    }

    {
        auto& position{ m_registry.get<::xrn::engine::component::Position>(m_camera.getId()) };
        auto& rotation{ m_registry.get<::xrn::engine::component::Rotation>(m_camera.getId()) };
        m_camera.setViewDirection(position, rotation.getDirection());
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::draw()
{

    if (!(m_frameInfo.commandBuffer = m_renderer.beginFrame())) {
        return;
    }
    m_frameInfo.frameIndex = static_cast<::std::size_t>(m_renderer.getFrameIndex());

    m_uboBuffers[m_frameInfo.frameIndex]->writeToBuffer(&m_frameInfo.ubo);
    m_uboBuffers[m_frameInfo.frameIndex]->flush();

    m_renderer.beginSwapChainRenderPass(m_frameInfo.commandBuffer);

    m_draw3d.bind(m_frameInfo);
    m_registry.view<::xrn::engine::component::Transform3d>().each([this](auto& transform){
        m_draw3d(m_frameInfo, transform);
    });

    m_drawPointLight.bind(m_frameInfo);
    m_registry.view<::xrn::engine::component::PointLight, ::xrn::engine::component::Position>().each(
        [this, lightIndex = 0](auto& pointLight, auto& position) mutable {
            m_drawPointLight(m_frameInfo, pointLight, position, lightIndex);
            ++lightIndex;
        }
    );

    m_renderer.endSwapChainRenderPass(m_frameInfo.commandBuffer);
    m_renderer.endFrame();
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::limitFrameRate()
{
    auto t{ ::xrn::Time::createAsSeconds(1) / ::xrn::engine::configuration.maxFrameRate };
    if (t > m_frameInfo.deltaTime) {
        ::std::this_thread::sleep_for((t - m_frameInfo.deltaTime).getAsChronoMilliseconds());
    }
}



///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::AScene::updateCamera()
{
    const float aspect{ m_renderer.getAspectRatio() };
    // m_camera.setOrthographicProjection(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    m_camera.setPerspectiveProjection(::glm::radians(50.0f), aspect, 0.1f, 1000.0f);
    // m_camera.setViewTarget(
        // { 0.0f, 0.0f, 0.0f }
        // , this->getPlayerComponent<::xrn::engine::component::Transform3d>().getPosition()
    // );

    m_frameInfo.projectionView = m_camera.getProjection() * m_camera.getView();
    m_frameInfo.ubo.projection = m_camera.getProjection();
    m_frameInfo.ubo.view = m_camera.getView();
}
