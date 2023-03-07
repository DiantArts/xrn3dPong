///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Server/Server.hpp>
#include "Game/MessageType.hpp"



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
    ::xrn::network::Message<::game::MessageType>& message,
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
)
{
    switch (message.getType()) {
    case ::game::MessageType::playerPosition: {
        for (auto& room : m_rooms) {
            if (room.contains(connection)) {
                if (auto opponent{ room.getOpponent(connection) }; opponent) {
                    this->udpSendToClient(message, opponent);
                }
            }
        }
        break;
    } case ::game::MessageType::queuing: {
        ::std::scoped_lock lock{ m_mutex };

        // TODO: insert matchmaking
        if (m_rooms.empty() || m_rooms.back().isFull()) {
            m_rooms.emplace_back(connection);
        } else {
            m_rooms.back().joinGame(connection);
            this->tcpSendToClient(
                ::xrn::network::Message<::game::MessageType>{ ::game::MessageType::playerAttributionOne }
                , m_rooms.back().getPlayer1()
            );
            this->tcpSendToClient(
                ::xrn::network::Message<::game::MessageType>{ ::game::MessageType::playerAttributionTwo }
                , m_rooms.back().getPlayer2()
            );
        }

        break;
    } default: {
        this->tcpSendToAllClients(message, connection);
        break;
    }
    }
}
