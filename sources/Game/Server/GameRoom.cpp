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
    m_isRunning = false;
    if (m_tickThread.joinable()) {
        m_tickThread.join();
    }
    XRN_INFO("Game room deleted");
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
    m_player1BurstSpeed = m_player1Position.get() - position;
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
    m_player2BurstSpeed = m_player2Position.get() - position;
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
        m_ball.setDefaultPropreties();
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
    // updade ball
    m_ball.onTick(
        deltaTime
        , m_player1Position
        , m_player2BurstSpeed
        , m_player2Position
        , m_player2BurstSpeed
    );

    { // send info to clients
        // create messages
        auto message1{ ::std::make_unique<GameRoom::Message>(::game::MessageType::ballPosition) };
        *message1 << m_ball.getPosition();
        auto message2{ ::std::make_unique<GameRoom::Message>(*message1) };

        // send
        m_player1->udpSend(::std::move(message1));
        m_player2->udpSend(::std::move(message2));
    }
}
