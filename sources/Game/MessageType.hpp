#pragma once

namespace game {

///////////////////////////////////////////////////////////////////////////
/// \brief Message to describe the communication between client and server
///
///////////////////////////////////////////////////////////////////////////
enum class MessageType
    : ::std::uint16_t
{
    playerPosition // udp sending to the server the position of the player

    , queuing // notify the server that the client is queuing for a game
    , playerAttributionOne // notify the client that an opponent has been found and the player is now player1
    , playerAttributionTwo // notify the client that an opponent has been found and the player is now player2
    , readyToPlay // notify the server that the client is ready to start the match

    , last // required member (system)
};

} // namespace game
