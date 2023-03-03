///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Util.hpp>
#include <Client/Scene.hpp>
#include <xrn/Engine/Components.hpp>
#include <xrn/Engine/Configuration.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::client::Scene::Scene()
    : ::xrn::engine::AScene::AScene{ true /* isCameraDetached */}
    , m_enemy{ m_registry.create() }
{
    this->loadObjects();
    this->loadMap();
    this->loadLights();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::client::Scene::~Scene() = default;



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::client::Scene::onUpdate()
    -> bool
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::client::Scene::postUpdate()
    -> bool
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onKeyPressed(
    ::std::int16_t keyCode
)
{
    if (
        auto* playerController{ this->tryGetPlayerComponent<::xrn::engine::component::Control>() };
        playerController
    ) {
        // move
        if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->startMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->startMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->startMovingLeft();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->startMovingRight();

        // move arrows
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->startMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->startMovingDown();
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
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onKeyReleased(
    ::std::int16_t keyCode
)
{
    if (
        auto* playerController{ this->tryGetPlayerComponent<::xrn::engine::component::Control>() };
        playerController
    ) {
        // move
        if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->stopMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->stopMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->stopMovingLeft();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->stopMovingRight();

        // move arrows
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->stopMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->stopMovingDown();
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
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onMouseMoved(
    ::glm::vec2 position
)
{}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadObjects()
{
    { // camera
        auto entity{ m_camera.getId() };
        m_registry.emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        m_registry.get<::xrn::engine::component::Control>(entity).setSpeed(300);
        m_registry.emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -(mapSize.z + 2.5f) });
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // player
        // if camera detached, create a random object to replace player
        auto entity{ m_isCameraDetached ? m_registry.create() : m_player };
        m_registry.emplace<::xrn::engine::component::Control>(entity);
        m_registry.get<::xrn::engine::component::Control>(entity).setSpeed(250);
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.5f, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 0.2, 0.01f, 0.2f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // enemy
        auto entity{ m_enemy };
        m_registry.emplace<::xrn::engine::component::Control>(entity);
        m_registry.get<::xrn::engine::component::Control>(entity).setSpeed(250);
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.5f, -mapSize.z);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 0.2, 0.01f, 0.2f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -90.0f, 0.0f, 0.0f });
    }
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadMap()
{
    // bot
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, mapSize.x, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.y, mapSize.z);
    }
    // top
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, -mapSize.x, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.y, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -180.0f, 0.0f, 0.0f });
    }
    // left
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, -mapSize.y, 0.f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, mapSize.y, 0.f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
        m_debugEntity = entity;
    }
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadLights()
{
    { // lights
        std::vector<glm::vec3> lightColors{
            { 1.f, .1f, .1f },
            { .1f, .1f, 1.f },
            { .1f, 1.f, .1f },
            { 1.f, 1.f, .1f },
            { .1f, 1.f, 1.f },
            { 1.f, 1.f, 1.f }
        };

        // create the lights at equal distances from each other in circle
        for (auto i{ 0uz }; const auto& color : lightColors) {
            auto rotation{ ::glm::rotate(
                ::glm::mat4(1.0f),
                i * ::glm::two_pi<float>() / lightColors.size(),
                { 0.0f, -1.0f, 0.0f }
            ) };
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Position>(
                entity, ::glm::vec3{ rotation * ::glm::vec4{ -0.0f, -0.0f, -1.5f, 1.0f } }
            );
            m_registry.emplace<::xrn::engine::component::Rotation>(entity, 90.0f, 0.0f, 0.0f);

            // make them move
            m_registry.emplace<::xrn::engine::component::Control>(entity);

            m_registry.emplace<::xrn::engine::component::PointLight>(entity, color);
            ++i;
        }
    }
}
