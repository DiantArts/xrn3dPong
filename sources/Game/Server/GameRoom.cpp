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
    : m_tickFrequencTime{
        ::xrn::Time::createAsSeconds(1) / ::xrn::engine::Configuration::defaultTickFrequency
    }
    , m_ballSpawnFrequencyTime{ ::xrn::Time::createAsSeconds(8) }
{
    m_player1.connection = ::std::move(connection);
    XRN_INFO("New game room created, owner:{}", m_player1.connection->getId());

    // defaultValues of player2
    m_player2.position.setZ(50.0f);
    m_player2.id = 2;
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
    return m_player2.connection != nullptr;
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
    if (connection == m_player1.connection) {
        return 1;
    } else if (connection == m_player2.connection) {
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
    if (player == m_player1.connection) {
        return m_player2.connection;
    } else if (player == m_player2.connection) {
        return m_player1.connection;
    }

    XRN_FATAL("Get opponent of a room that is not the player's one");
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::getPlayer1()
    -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    return m_player1.connection;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::setPlayer1Position(
    ::glm::vec3&& position
)
{
    m_player1.burstSpeed = m_player1.position.get() - position;
    m_player1.position = ::std::move(position);
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::getPlayer2()
    -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    return m_player2.connection;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::setPlayer2Position(
    ::glm::vec3&& position
)
{
    m_player2.burstSpeed = m_player2.position.get() - position;
    m_player2.position = ::std::move(position);
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::udpSendToBothClients(
    ::std::unique_ptr<GameRoom::Message> message1
)
{
    auto message2{ ::std::make_unique<GameRoom::Message>(*message1) };
    m_player1.connection->udpSend(::std::move(message1));
    m_player2.connection->udpSend(::std::move(message2));
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::tcpSendToBothClients(
    ::std::unique_ptr<GameRoom::Message> message1
)
{
    auto message2{ ::std::make_unique<GameRoom::Message>(*message1) };
    m_player1.connection->tcpSend(::std::move(message1));
    m_player2.connection->tcpSend(::std::move(message2));
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::joinGame(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
{
    // if player1 is disconnected
    if (!m_player1.connection->isConnected()) {
        m_player1.connection = ::std::move(connection);
        XRN_INFO("Game room owner changed:{}", m_player1.connection->getId());
        return;
    }

    m_player2.connection = ::std::move(connection);
    XRN_INFO("Game room joined by:{}", m_player2.connection->getId());
    m_isRunning = true;
    m_tickThread = ::std::thread{ [this]() {
        ::xrn::Clock m_clock, ballSpawn;

        m_balls.emplace_back();
        m_balls.back().setDefaultPropreties();
        do {
            if (!m_player1.connection->isConnected()) {
                return m_player2.connection->disconnect();
            } else if (!m_player2.connection->isConnected()) {
                return m_player1.connection->disconnect();
            }
            auto deltaTime{ m_clock.restart() };
            if (ballSpawn.getElapsed() >= m_ballSpawnFrequencyTime) {
                ballSpawn.reset();
                this->createBall();
            }
            for (auto i{ 0uz }; i < m_balls.size(); ++i) {
                if (this->onTick(deltaTime, i)) {
                    goto BREAK_THREAD_LOOP;
                }
            }
            ::std::this_thread::sleep_for((m_tickFrequencTime).getAsChronoMilliseconds());
        } while (this->isRunning());
BREAK_THREAD_LOOP:
        m_player1.connection->disconnect();
        m_player2.connection->disconnect();
        m_isRunning = false;
    } };
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::createBall()
{
    m_balls.emplace_back();
    m_balls.back().setDefaultPropreties();
    auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::createBall) };
    this->tcpSendToBothClients(::std::move(message));
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::resetBalls()
{
    // m_balls.clear();
    // m_balls.emplace_back();
    // m_balls.back().setDefaultPropreties();
    // auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::resetBalls) };
    // this->tcpSendToBothClients(::std::move(message));
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::GameRoom::onTick(
    ::xrn::Time deltaTime
    , ::std::size_t index
) -> bool
{
    // ball behaviors
    switch (m_balls[index].updateBallRotation(m_player1, m_player2)) {
    case 1: { // collided with player 1
        auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::playSound) };
        *message << 0;
        this->tcpSendToBothClients(::std::move(message));
        break;
    } case 2: { // collided with player 2
        auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::playSound) };
        *message << 0;
        this->tcpSendToBothClients(::std::move(message));
        break;
    } case 3: { // collided with wall
        auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::playSound) };
        *message << 0;
        this->tcpSendToBothClients(::std::move(message));
        break;
    }}

    m_balls[index].onTick(
        deltaTime
        , m_player1
        , m_player2
    );



    switch (m_balls[index].checkWinCondition()) {
    case 1: { // player1 win
        if (++m_player1.score >= 5) {
            this->triggerWin(m_player1.id);
            return true;
        }
        this->resetBalls();
        return false;
    } case 2: { // playclearBallser2 win
        if (++m_player2.score >= 5) {
            this->triggerWin(m_player2.id);
            return true;
        }
        this->resetBalls();
        return false;
    } };

    { // send info to clients
        auto message{ ::std::make_unique<GameRoom::Message>(::game::MessageType::ballPosition) };
        *message << static_cast<int>(index);
        *message << m_balls[index].getPosition();
        this->udpSendToBothClients(::std::move(message));
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::triggerWin(
    ::std::uint8_t winnerId
)
{
    if (m_player1.id == winnerId) {
        XRN_INFO("Player1 won");
    } else {
        XRN_INFO("Player2 won");
    }
}
