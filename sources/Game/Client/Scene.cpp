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
#include "xrn/Engine/Component/PointLight.hpp"



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::client::Scene::Scene()
    : ::xrn::engine::AScene::AScene{ false /* isCameraDetached */}
    , m_enemy{ this->getRegistry().create() }
    , m_ball{ this->getRegistry().create() }
{
    this->loadScene();
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadScene()
{
    this->loadObjects();
    this->loadMap();
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
    if (!this->isConnectedToServer()) {
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onPostUpdate()
    -> bool
{
    // TODO: tmp max position check (use a MaxPosition component or inivisble wall)
    if (!m_isCameraDetached) {
        auto& position{ this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()) };
        if (position.get().x >= maxMapPosition.x) {
            position.setX(maxMapPosition.x);
        } else if (position.get().x <= -maxMapPosition.x) {
            position.setX(-maxMapPosition.x);
        }
        if (position.get().y >= maxMapPosition.y) {
            position.setY(maxMapPosition.y);
        } else if (position.get().y <= -maxMapPosition.y) {
            position.setY(-maxMapPosition.y);
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onTick(
    ::xrn::Time deltaTime [[ maybe_unused ]]
) -> bool
{
    if (!m_isCameraDetached) {
        auto& position{ this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()) };

        XRN_INFO(
            "New position: [{:.5};{:.5};{:.5}]"
            , position.get().x
            , position.get().y
            , position.get().z
        );

        // send corrected position to server
        auto message{::std::make_unique<Scene::Message>(::game::MessageType::playerPosition) };
        *message << position.get();
        this->udpSendToServer(::std::move(message));
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::onKeyPressed(
    ::std::int16_t keyCode
)
{
    if (
        auto* playerController{ this->getRegistry().try_get<::xrn::engine::component::Control>(this->getPlayerId()) };
        playerController
    ) {
        // move
        if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->startMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->startMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->startMovingBackward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->startMovingForward();

        // move arrows
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->startMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->startMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
            return playerController->startMovingBackward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
            return playerController->startMovingForward();

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
        auto* playerController{ this->getRegistry().try_get<::xrn::engine::component::Control>(this->getPlayerId()) };
        playerController
    ) {
        // move
        if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->stopMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->stopMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->stopMovingBackward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->stopMovingForward();

        // move arrows
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->stopMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->stopMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
            return playerController->stopMovingBackward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
            return playerController->stopMovingForward();

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
    Scene::Message& message
    , ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection [[ maybe_unused ]]
)
{
    switch (message.getType()) {
    case ::game::MessageType::playerPosition: {
        ::glm::vec3 pos;
        message >> pos;
        this->getRegistry().get<::xrn::engine::component::Position>(m_enemy).set(::std::move(pos));
        break;
    } case ::game::MessageType::ballPosition: {
        ::glm::vec3 pos;
        message >> pos;
        this->getRegistry().get<::xrn::engine::component::Position>(m_ball).set(::std::move(pos));
        if (m_playerNumber == 1) {
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.r = 1 - (-pos.z / mapSize.z);
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.g = 1 - (pos.z / (mapSize.z - 2));
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.b = 1 - (pos.z / mapSize.z);
        } else {
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.r = 1 - (pos.z / mapSize.z);
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.g = 1 - (-pos.z / (mapSize.z - 2));
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.b = 1 - (-pos.z / mapSize.z);
        }
        break;
    } case ::game::MessageType::playerAttributionOne: { // starts the game
        m_playerNumber = 1;
        this->loadLights();

        // this->tcpSendToServer(::game::MessageType::readyToPlay);
        break;
    } case ::game::MessageType::playerAttributionTwo: { // starts the game
        m_playerNumber = 2;
        this->loadLights();

        // move to the other side because player is player2
        // camera
        this->getRegistry().get<::xrn::engine::component::Position>(getCameraId()).setZ(mapSize.z + 25.0f);
        this->getRegistry().get<::xrn::engine::component::Rotation>(getCameraId()).rotateX(180);
        // player
        this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setZ(mapSize.z);
        this->getRegistry().get<::xrn::engine::component::Rotation>(this->getPlayerId()).rotateX(180);
        // enemy
        this->getRegistry().get<::xrn::engine::component::Position>(m_enemy).setZ(-mapSize.z);
        this->getRegistry().get<::xrn::engine::component::Rotation>(m_enemy).rotateX(180);

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
    this->tcpSendToServer(::std::make_unique<Scene::Message>(::game::MessageType::queuing));
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
        auto entity{ getCameraId() };
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(3000);
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -(mapSize.z + 25.0f) });
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // player
        // if camera detached, create a random object to replace player
        auto entity{ m_isCameraDetached ? this->getRegistry().create() : this->getPlayerId()};
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity);
        // this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(2500);
        this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(300);
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Cube"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, -mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, this->playerScale);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // enemy
        auto entity{ m_enemy };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Cube"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, this->playerScale);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // ball
        auto entity{ m_ball };
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity);
        this->getRegistry().emplace<::xrn::engine::component::PointLight>(entity, glm::vec3{ 1.0f, 1.0f, 1.0f });
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -90.0f, 0.0f, 0.0f });
    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadMap()
{
    // bot
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, mapSize.y, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
    }
    // top
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, -mapSize.y, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -180.0f, 0.0f, 0.0f });
    }
    // left
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, -mapSize.x, 0.0f, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.x, mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, mapSize.x, 0.0f, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.x, mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadLights()
{
    { // lights
        std::vector<glm::vec3> lightPositions{
            { 0, (mapSize.y + 1), mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { (mapSize.x + 1), 0, mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { 0, -(mapSize.y + 1), mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { -(mapSize.x + 1), 0, mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { mapSize.x * 2, mapSize.y + 0, 0 } // useless for now
        };

        // create the lights at equal distances from each other in circle
        for (const auto& color : lightPositions) {
            auto entity{ this->getRegistry().create() };
            this->getRegistry().emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ color });
            this->getRegistry().emplace<::xrn::engine::component::Control>(entity);
            this->getRegistry().emplace<::xrn::engine::component::PointLight>(entity, ::glm::vec3{ 1.0f, 1.0f, 1.0f });
            this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, 0.0f, 0.0f, 0.0f);
        }
    }
}
