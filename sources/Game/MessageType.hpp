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
    , ballPosition // server sending the position of the ball
    , createBall
    , resetBalls

    , playSound // server request the client to play a sound from index

    , queuing // notify the server that the client is queuing for a game
    , playerAttributionOne // notify the client that an opponent has been found and the player is now player1
    , playerAttributionTwo // notify the client that an opponent has been found and the player is now player2
    , readyToPlay // notify the server that the client is ready to start the match

    , message // just to debug

    , last // required member (system)
};

} // namespace game
