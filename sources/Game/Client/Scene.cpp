///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Client/Scene.hpp>
#include <xrn/Engine/Components.hpp>
#include <xrn/Engine/Configuration.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::client::Scene::Scene()
    : ::xrn::engine::AScene::AScene{ false /* isCameraDetached */}
    , m_enemy{ m_registry.create() }
    , m_ball{ m_registry.create() }
{
    this->loadScene();
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadScene()
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
::game::client::Scene::~Scene() = default;



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onUpdate()
    -> bool
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onPostUpdate()
    -> bool
{
    // TODO: tmp max position check (use a MaxPosition component or inivisble wall)
    if (!m_isCameraDetached) {
        auto& position{ m_registry.get<::xrn::engine::component::Position>(m_player) };
        if (position.get().x >= maxMapPosition.x) {
            position.setX(maxMapPosition.x);
        }
        if (position.get().x <= -maxMapPosition.x) {
            position.setX(-maxMapPosition.x);
        }
        if (position.get().y >= maxMapPosition.y) {
            position.setY(maxMapPosition.y);
        }
        if (position.get().y <= -maxMapPosition.y) {
            position.setY(-maxMapPosition.y);
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onTick()
    -> bool
{
    if (!m_isCameraDetached) {
        auto& position{ m_registry.get<::xrn::engine::component::Position>(m_player) };
        // send corrected position to server
        this->udpSendToServer(
            ::game::MessageType::playerPosition
            , position.get().x
            , position.get().y
            , position.get().z // ignored but send eitherway
        );
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::onKeyPressed(
    ::std::int16_t keyCode
)
{
    if (
        auto* playerController{ m_registry.try_get<::xrn::engine::component::Control>(m_player) };
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
void ::game::client::Scene::onKeyReleased(
    ::std::int16_t keyCode
)
{
    if (
        auto* playerController{ m_registry.try_get<::xrn::engine::component::Control>(m_player) };
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
void ::game::client::Scene::onMouseMoved(
    ::glm::vec2 position [[ maybe_unused ]]
)
{}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::onReceive(
    ::xrn::network::Message<::game::MessageType>& message
    , ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection [[ maybe_unused ]]
)
{
    switch (message.getType()) {
    case ::game::MessageType::playerPosition: {
        ::glm::vec3 pos{ message.pull<float>(), message.pull<float>(), message.pull<float>() };
        ::fmt::print("<- Player '[{};{};{}]'\n", pos.x, pos.y, pos.z);
        m_registry.get<::xrn::engine::component::Position>(m_enemy).set(::std::move(pos));
        break;
    } case ::game::MessageType::ballPosition: {
        ::glm::vec3 pos{ message.pull<float>(), message.pull<float>(), message.pull<float>() };
        ::fmt::print("<-  Ball  '[{};{};{}]'\n", pos.x, pos.y, pos.z);
        m_registry.get<::xrn::engine::component::Position>(m_ball).set(::std::move(pos));
        break;
    } case ::game::MessageType::playerAttributionOne: { // starts the game
        // this->tcpSendToServer(::game::MessageType::readyToPlay);
        break;
    } case ::game::MessageType::playerAttributionTwo: { // starts the game

        // move to the other side because player is player2
        // camera
        m_registry.get<::xrn::engine::component::Position>(m_camera.getId()).setZ(mapSize.z + 25.0f);
        m_registry.get<::xrn::engine::component::Rotation>(m_camera.getId()).rotateX(180); // TODO: bug here
        // player
        m_registry.get<::xrn::engine::component::Position>(m_player).setZ(mapSize.z);
        m_registry.get<::xrn::engine::component::Rotation>(m_player).rotateX(180);
        // enemy
        m_registry.get<::xrn::engine::component::Position>(m_enemy).setZ(-mapSize.z);
        m_registry.get<::xrn::engine::component::Rotation>(m_enemy).rotateX(180);

        // this->tcpSendToServer(::game::MessageType::readyToPlay);
        break;
    } default: {
    break;
    }}
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Queue
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::queueForGame()
{
    this->tcpSendToServer(::game::MessageType::queuing);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadObjects()
{
    { // camera
        auto entity{ m_camera.getId() };
        m_registry.emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        m_registry.get<::xrn::engine::component::Control>(entity).setSpeed(3000);
        m_registry.emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -(mapSize.z + 25.0f) });
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // player
        // if camera detached, create a random object to replace player
        auto entity{ m_isCameraDetached ? m_registry.create() : m_player };
        m_registry.emplace<::xrn::engine::component::Control>(entity);
        m_registry.get<::xrn::engine::component::Control>(entity).setSpeed(2500);
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.5f, -mapSize.z);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 2.0f, 0.1f, 2.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // enemy
        auto entity{ m_enemy };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.5f, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 2.0f, 0.1f, 2.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -90.0f, 0.0f, 0.0f });
    }

    {
        auto entity{ m_ball };
        m_registry.emplace<::xrn::engine::component::Control>(entity);
        m_registry.emplace<::xrn::engine::component::PointLight>(entity, glm::vec3{ 1.0f, 1.0f, 1.0f });
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -90.0f, 0.0f, 0.0f });

    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadMap()
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
        m_registry.emplace<::xrn::engine::component::Position>(entity, -mapSize.y, 0.0f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, mapSize.y, 0.0f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadLights()
{
    { // lights
        std::vector<glm::vec3> lightColors{
            { 1.0f, 0.1f, 0.1f }
            , { 0.1f, 0.1f, 1.0f }
            , { 0.1f, 1.0f, 0.1f }
            , { 1.0f, 1.0f, 0.1f }
            , { 0.1f, 1.0f, 1.0f }
        };

        // create the lights at equal distances from each other in circle
        for (auto i{ 0uz }; const auto& color : lightColors) {
            auto rotation{ ::glm::rotate(
                ::glm::mat4(1.0f)
                , i * ::glm::two_pi<float>() / lightColors.size()
                , { 0.0f, -1.0f, 0.0f }
            ) };
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Position>(
                entity, ::glm::vec3{ rotation * ::glm::vec4{ -0.0f, -0.0f, -15.0f, 1.0f } }
            );
            m_registry.emplace<::xrn::engine::component::Rotation>(entity, 90.0f, 0.0f, 0.0f);

            // make them move
            m_registry.emplace<::xrn::engine::component::Control>(entity);

            m_registry.emplace<::xrn::engine::component::PointLight>(entity, color);
            ++i;
        }
    }
}
