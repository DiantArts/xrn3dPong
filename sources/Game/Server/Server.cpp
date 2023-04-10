///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Server/Server.hpp>
#include <Game/MessageType.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::Server::Server(
    ::std::uint16_t port
)
    : ::xrn::network::server::Server<::game::MessageType>{ port }
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
/// \brief Behavior when receiving a message
///
///////////////////////////////////////////////////////////////////////////
void ::game::Server::onReceive(
    Server::Message& message
    , ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
{
    switch (message.getType()) {
    case ::game::MessageType::playerPosition: {
        ::glm::vec3 pos;
        message >> pos;

        // TODO: optimize to not have to search on every room every message lol
        for (auto& room : m_rooms) {
            if (room->isRunning()) {
                switch (room->contains(connection)) {
                case 1: // player1
                    this->udpSendToClient(message, room->getPlayer2());
                    room->setPlayer1Position(::std::move(pos));
                    return;
                case 2: // player2
                    this->udpSendToClient(message, room->getPlayer1());
                    room->setPlayer2Position(::std::move(pos));
                    return;
                default: return; // Not present
                }
            }
        }
        return;
    } case ::game::MessageType::queuing: {
        ::std::scoped_lock lock{ m_mutex };

        // TODO: insert matchmaking
        if (m_rooms.empty() || m_rooms.back()->isFull()) {
            XRN_INFO("Create new roow");
            m_rooms.emplace_back(::std::make_unique<::game::server::GameRoom>(connection));
        } else {
            m_rooms.back()->joinGame(connection);
            if (m_rooms.back()->isFull()) {
                XRN_INFO("Roow now full");
                {
                    auto messageBack{ ::std::make_unique<Server::Message>(::game::MessageType::playerAttributionOne) };
                    this->tcpSendToClient(::std::move(messageBack), m_rooms.back()->getPlayer1());
                }
                {
                    auto messageBack{ ::std::make_unique<Server::Message>(::game::MessageType::playerAttributionTwo) };
                    this->tcpSendToClient(::std::move(messageBack), m_rooms.back()->getPlayer2());
                }
            }
        }
        return;
    } default: {
        return;
    }}
}
