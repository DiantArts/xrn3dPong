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
#include <xrn/Engine/Component/PointLight.hpp>
#include <Game/Map.hpp>

#define ENABLE_CHEAT_01
// #define ENABLE_CHEAT_02
// #define ENABLE_KEY_PRESSED
#define ENABLE_SOUND
// #define ENABLE_MUSIC_FOR_ALL_CLIENTS



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
    this->getWindow().hideCursor(false);

    auto filepath{ "./data/Audio/PowerfulTrap.ogg" };
#ifdef ENABLE_SOUND
    if (m_music.openFromFile(filepath)) {
        XRN_ERROR("unable to load sound file: {}", filepath);
    }
    m_music.setLoop(true);
#endif // ENABLE_SOUND
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadScene()
{
    { // camera
        auto entity{ this->getCameraId() };
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(1000);
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -(::game::Map::mapSize.z + 30.0f) });
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }


    ::game::Map::loadObjects(
        this->getRegistry()
        , &this->getVulkanDevice()
        , m_isCameraDetached ? this->getRegistry().create() : this->getPlayerId()
        , m_enemy
        , m_ball
    );
    ::game::Map::loadMap(this->getRegistry(), &this->getVulkanDevice());
}

///////////////////////////////////////////////////////////////////////////
void ::game::client::Scene::loadObjects()
{
    { // camera
        auto entity{ this->getCameraId() };
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(1000);
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -(::game::Map::mapSize.z + 30.0f) });
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // player
        // if camera detached, create a random object to replace player
        auto entity{ m_isCameraDetached ? this->getRegistry().create() : this->getPlayerId()};
        this->getRegistry().emplace<::xrn::engine::component::Control>(entity);
        this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(2500);
        // this->getRegistry().get<::xrn::engine::component::Control>(entity).setSpeed(300);
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Cube"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, -::game::Map::mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::playerScale);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // enemy
        auto entity{ m_enemy };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Cube"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, ::game::Map::mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::playerScale);
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
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, ::game::Map::mapSize.y, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::mapSize.x, ::game::Map::mapSize.y, ::game::Map::mapSize.z);
    }
    // top
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, 0.0f, -::game::Map::mapSize.y, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::mapSize.x, ::game::Map::mapSize.y, ::game::Map::mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -180.0f, 0.0f, 0.0f });
    }
    // left
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, -::game::Map::mapSize.x, 0.0f, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::mapSize.y, ::game::Map::mapSize.x, ::game::Map::mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ this->getRegistry().create() };
        this->getRegistry().emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(this->getVulkanDevice(), "Floor"));
        this->getRegistry().emplace<::xrn::engine::component::Position>(entity, ::game::Map::mapSize.x, 0.0f, 0.0f);
        this->getRegistry().emplace<::xrn::engine::component::Scale>(entity, ::game::Map::mapSize.y, ::game::Map::mapSize.x, ::game::Map::mapSize.z);
        this->getRegistry().emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
    }
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
    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::client::Scene::onTick(
    ::xrn::Time deltaTime [[ maybe_unused ]]
) -> bool
{
    if (!m_isCameraDetached) {
        auto message{ ::std::make_unique<Scene::Message>(::game::MessageType::playerPosition) };
        auto& position{ this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()) };
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
#ifdef ENABLE_KEY_PRESSED
        // move
        if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->startMovingUp();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->startMovingDown();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->startMovingBackward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->startMovingForward();

        // camera distance
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            auto& cameraController{ this->getRegistry().get<::xrn::engine::component::Control>(this->getCameraId()) };
            return m_playerNumber == 1 ? cameraController.startMovingBackward() : cameraController.startMovingForward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            auto& cameraController{ this->getRegistry().get<::xrn::engine::component::Control>(this->getCameraId()) };
            return m_playerNumber == 1 ? cameraController.startMovingForward() : cameraController.startMovingBackward();

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
#endif // ENABLE_KEY_PRESSED
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

        // camera distance
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            auto& cameraController{ this->getRegistry().get<::xrn::engine::component::Control>(this->getCameraId()) };
            return m_playerNumber == 1 ? cameraController.stopMovingBackward() : cameraController.stopMovingForward();
        } else if (keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            auto& cameraController{ this->getRegistry().get<::xrn::engine::component::Control>(this->getCameraId()) };
            return m_playerNumber == 1 ? cameraController.stopMovingForward() : cameraController.stopMovingBackward();

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
    ::glm::vec2 _ [[ maybe_unused ]]
)
{
    auto movePlayer{ [this](){
        auto newPosition{ this->getMousePointer().castToPlane(
            this->getRegistry().get<::xrn::engine::component::Position>(this->getCameraId())
            , { 0.0f, 0.0f, ::game::Map::maxMapPosition.z * (m_playerNumber == 1 ? -1 : 1) }
            , { 0.0f, 0.0f, 0.0f }
        ) };

        auto& position{ this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()) };
        if (newPosition.x >= ::game::Map::maxMapPosition.x) {
            newPosition.x = ::game::Map::maxMapPosition.x;
        } else if (newPosition.x <= -::game::Map::maxMapPosition.x) {
            newPosition.x = -::game::Map::maxMapPosition.x;
        }
        if (newPosition.y >= ::game::Map::maxMapPosition.y) {
            newPosition.y = ::game::Map::maxMapPosition.y;
        } else if (newPosition.y <= -::game::Map::maxMapPosition.y) {
            newPosition.y = -::game::Map::maxMapPosition.y;
        }

        position.setX(newPosition.x);
        position.setY(newPosition.y);
    } };

    switch (m_playerNumber) {
#ifndef ENABLE_CHEAT_01
    case 1:
        movePlayer();
        break;
#endif // ENABLE_CHEAT_01
#ifndef ENABLE_CHEAT_02
    case 2:
        movePlayer();
        break;
#endif // ENABLE_CHEAT_02
    }
}

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
#ifdef ENABLE_CHEAT_01
            this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setX(pos.x);
            this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setY(pos.y);
#endif // ENABLE_CHEAT_01
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.r = 1 - (-pos.z / ::game::Map::mapSize.z);
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.g = 1 - (pos.z / (::game::Map::mapSize.z - 2));
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.b = 1 - (pos.z / ::game::Map::mapSize.z);
        } else {
#ifdef ENABLE_CHEAT_02
            this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setX(pos.x);
            this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setY(pos.y);
#endif // ENABLE_CHEAT_02
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.r = 1 - (pos.z / ::game::Map::mapSize.z);
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.g = 1 - (-pos.z / (::game::Map::mapSize.z - 2));
            this->getRegistry().get<::xrn::engine::component::PointLight>(m_ball).color.b = 1 - (-pos.z / ::game::Map::mapSize.z);
        }
        break;
#ifdef ENABLE_SOUND
    } case ::game::MessageType::playSound: { // starts the game
#ifndef ENABLE_MUSIC_FOR_ALL_CLIENTS
        if (m_playerNumber == 1) {
#endif // ENABLE_MUSIC_FOR_ALL_CLIENTS
        int soundIndex;
        message >> soundIndex;

        // load sound
        if (!m_soundBuffer.loadFromFile("./data/Audio/" + m_soundFilenames[soundIndex])) {
            XRN_ERROR("unable to load sound file: {} (index: {})"
                , m_soundFilenames[soundIndex]
                , soundIndex
            )
        } else {
            // play sound
            m_sound.setBuffer(m_soundBuffer);
            m_sound.play();
        }
#ifndef ENABLE_MUSIC_FOR_ALL_CLIENTS
        }
#endif // ENABLE_MUSIC_FOR_ALL_CLIENTS
        break;
#endif // ENABLE_SOUND
    } case ::game::MessageType::playerAttributionOne: { // starts the game
        m_playerNumber = 1;
        this->loadLights();

#ifdef ENABLE_SOUND
        m_music.play();
#endif // ENABLE_SOUND

        this->tcpSendToServer(::std::make_unique<Scene::Message>(::game::MessageType::readyToPlay));
        break;
    } case ::game::MessageType::playerAttributionTwo: { // starts the game
        m_playerNumber = 2;
        this->loadLights();

#ifdef ENABLE_SOUND
#ifdef ENABLE_MUSIC_FOR_ALL_CLIENTS
        m_music.play();
#endif // ENABLE_MUSIC_FOR_ALL_CLIENTS
#endif // ENABLE_SOUND

        // move to the other side because player is player2
        // camera
        this->getRegistry().get<::xrn::engine::component::Position>(this->getCameraId()).setZ(::game::Map::mapSize.z + 30.0f);
        this->getRegistry().get<::xrn::engine::component::Rotation>(this->getCameraId()).rotateX(180);
        // player
        this->getRegistry().get<::xrn::engine::component::Position>(this->getPlayerId()).setZ(::game::Map::mapSize.z);
        this->getRegistry().get<::xrn::engine::component::Rotation>(this->getPlayerId()).rotateX(180);
        // enemy
        this->getRegistry().get<::xrn::engine::component::Position>(m_enemy).setZ(-::game::Map::mapSize.z);
        this->getRegistry().get<::xrn::engine::component::Rotation>(m_enemy).rotateX(180);

        this->tcpSendToServer(::std::make_unique<Scene::Message>(::game::MessageType::readyToPlay));
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
void ::game::client::Scene::loadLights()
{
    { // lights
        std::vector<glm::vec3> lightPositions{
            { 0, (::game::Map::mapSize.y + 1), ::game::Map::mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { (::game::Map::mapSize.x + 1), 0, ::game::Map::mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { 0, -(::game::Map::mapSize.y + 1), ::game::Map::mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { -(::game::Map::mapSize.x + 1), 0, ::game::Map::mapSize.z / 2 * (m_playerNumber == 1 ? -1 : 1) }
            , { ::game::Map::mapSize.x + 1000, ::game::Map::mapSize.y + 0, 0 } // useless for now
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
