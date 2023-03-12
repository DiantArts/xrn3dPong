///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Server/GameRoom.hpp>
#include <Game/Client/Scene.hpp>
#include <Game/MessageType.hpp>
#include <xrn/Engine/Configuration.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::server::GameRoom::GameRoom(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
    : m_player1{ ::std::move(connection) }
    , m_tickFrequencTime{
        ::xrn::Time::createAsSeconds(1) / ::xrn::engine::Configuration::defaultTickFrequency
    }
{
    XRN_INFO("New game room created, owner:{}", m_player1->getId());
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::server::GameRoom::~GameRoom()
{
    XRN_INFO("Game room deleted");
    m_isRunning = false;
    m_tickThread.join();
}

///////////////////////////////////////////////////////////////////////////
::game::server::GameRoom::GameRoom(
    GameRoom&&
) noexcept = default;

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::operator=(
    GameRoom&&
) noexcept
    -> GameRoom& = default;




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Player manipulation
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::isFull() const
    -> bool
{
    return m_player2 != nullptr;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::isRunning() const
    -> bool
{
    return m_isRunning;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::contains(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
) const
    -> ::std::uint8_t
{
    if (!connection) {
        return 0;
    }
    if (connection == m_player1) {
        return 1;
    } else if (connection == m_player2) {
        return 2;
    } else {
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::getOpponent(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> player
) -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    if (player == m_player1) {
        return m_player2;
    } else if (player == m_player2) {
        return m_player1;
    }

    XRN_FATAL("Get opponent of a room that is not the player's one");
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::getPlayer1()
    -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    return m_player1;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::setPlayer1Position(
    ::glm::vec3&& position
)
{
    m_player1Position = ::std::move(position);
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::getPlayer2()
    -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    return m_player2;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::setPlayer2Position(
    ::glm::vec3&& position
)
{
    m_player2Position = ::std::move(position);
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::joinGame(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
{
    // if player1 is disconnected
    if (!m_player1->isConnected()) {
        m_player1 = ::std::move(connection);
        XRN_INFO("Game room owner changed:{}", m_player1->getId());
        return;
    }

    m_player2 = ::std::move(connection);
    XRN_INFO("Game room joined by:{}", m_player2->getId());
    m_isRunning = true;
    m_tickThread = ::std::thread{ [this]() {
        ::xrn::Clock m_clock;
        m_ballControl.setSpeed(3000);
        // m_ballControl.rotateAbsoluteX(0); // left to right
        m_ballControl.rotateAbsoluteX(90); // forward to backward
        m_ballControl.startMovingForward();
        do {
            if (!m_player1->isConnected()) {
                return m_player2->disconnect();
            } else if (!m_player2->isConnected()) {
                return m_player1->disconnect();
            }
            auto deltaTime{ m_clock.restart() };
            this->onTick(deltaTime);
            ::std::this_thread::sleep_for((m_tickFrequencTime).getAsChronoMilliseconds());
        } while (this->isRunning());
    } };
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::onTick(
    ::xrn::Time deltaTime
)
{
    this->updateBallDirection();

    m_ballPosition.update(deltaTime, m_ballControl, m_ballRotation.getDirection());

    {
        // create messages
        auto message1{ ::std::make_unique<GameRoom::Message>(::game::MessageType::ballPosition) };
        *message1 << m_ballPosition.get();
        auto message2{ ::std::make_unique<GameRoom::Message>(*message1) };

        // send
        m_player1->udpSend(::std::move(message1));
        m_player2->udpSend(::std::move(message2));
    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::updateBallDirection()
{
    // bind the ball inside the map
    if (m_ballPosition.get().x >= ::game::client::Scene::maxMapPosition.x) {
        m_ballControl.rotateAbsoluteX(180);
    } else if (m_ballPosition.get().x <= -::game::client::Scene::maxMapPosition.x) {
        m_ballControl.rotateAbsoluteX(180);
    }
    if (m_ballPosition.get().y >= ::game::client::Scene::maxMapPosition.y) {
        m_ballControl.rotateAbsoluteX(180);
    } else if (m_ballPosition.get().y <= -::game::client::Scene::maxMapPosition.y) {
        m_ballControl.rotateAbsoluteX(180);
    }

    for (auto& position : { m_player1Position, m_player2Position }) {
        auto beginHitbox{ position.get() - ::game::client::Scene::playerScale };
        auto endHitbox{ position.get() + ::game::client::Scene::playerScale };
        auto& ball{ m_ballPosition.get() };

        if (
            ball.x < beginHitbox.x || ball.x > endHitbox.x ||
            ball.y < beginHitbox.y || ball.y > endHitbox.y ||
            ball.z < beginHitbox.z || ball.z > endHitbox.z
        ) {
            continue;
        }
        continue;

        XRN_DEBUG(
            "player collision: [{};{};{}] < [{};{};{}] < [{};{};{}]"
            , beginHitbox.x
            , beginHitbox.y
            , beginHitbox.z
            , ball.x
            , ball.y
            , ball.z
            , endHitbox.x
            , endHitbox.y
            , endHitbox.z
        );

        m_ballControl.rotateAbsoluteX(180);
        XRN_INFO(
            "collision resolved: [{};{};{}] < [{};{};{}] < [{};{};{}]"
            , beginHitbox.x
            , beginHitbox.y
            , beginHitbox.z
            , ball.x
            , ball.y
            , ball.z
            , endHitbox.x
            , endHitbox.y
            , endHitbox.z
        );

        XRN_DEBUG("player collision");
        return;
    }

    if (m_ballPosition.get().z >= ::game::client::Scene::maxMapPosition.z + 5) { // player1 win
        XRN_DEBUG("Player1 won");
        m_ballControl.resetRotatedFlag();
        m_ballRotation.setRotation(270, 0, 0);
        m_ballPosition.set(0, 0, 0);
    } else if (m_ballPosition.get().z <= -(::game::client::Scene::maxMapPosition.z + 5)) { // player2 win
        XRN_DEBUG("Player2 won");
        m_ballControl.resetRotatedFlag();
        m_ballRotation.setRotation(90, 0, 0);
        m_ballPosition.set(0, 0, 0);
    }

    m_ballRotation.updateDirection(m_ballControl);
}
