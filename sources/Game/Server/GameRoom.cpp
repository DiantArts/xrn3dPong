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
{}




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
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> player
) const
    -> bool
{
    return (player == m_player1) || (player == m_player2);
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
auto ::game::server::GameRoom::getPlayer2()
    -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>
{
    return m_player2;
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::GameRoom::joinGame(
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
{
    // if player1 is disconnected
    if (!m_player1->isConnected()) {
        m_player1 = ::std::move(connection);
        return;
    }

    m_player2 = ::std::move(connection);
    m_isRunning = true;
    m_tickThread = ::std::thread{ [this]() {
        ::xrn::Clock m_clock;
        m_ballControl.startMovingForward();
        do {
            if (!m_player1->isConnected() || !m_player2->isConnected()) {
                return;
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
    // bind the ball inside the map
    if (m_ballPosition.get().x >= ::game::client::Scene::maxMapPosition.x) {
        m_ballControl.rotateX(180);
        m_ballRotation.updateDirection(m_ballControl);
    } else if (m_ballPosition.get().x <= -::game::client::Scene::maxMapPosition.x) {
        m_ballControl.rotateX(180);
        m_ballRotation.updateDirection(m_ballControl);
    }
    if (m_ballPosition.get().y >= ::game::client::Scene::maxMapPosition.y) {
        m_ballControl.rotateY(180);
        m_ballRotation.updateDirection(m_ballControl);
    } else if (m_ballPosition.get().y <= -::game::client::Scene::maxMapPosition.y) {
        m_ballControl.rotateY(180);
        m_ballRotation.updateDirection(m_ballControl);
    }
    m_ballPosition.update(deltaTime, m_ballControl, m_ballRotation.getDirection());
    ::fmt::print("{}\n", deltaTime.getAsMilliseconds());
    ::fmt::print("[{};{};{}]\n", m_ballPosition.get().x, m_ballPosition.get().y, m_ballPosition.get().z);

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
