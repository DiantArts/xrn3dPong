#pragma once

#include <xrn/Engine/Components.hpp>
#include <Game/MessageType.hpp>

namespace game::server {

///////////////////////////////////////////////////////////////////////////
/// \brief Room for 2 players to play in
/// \ingroup game
///
/// \include GameRoom.hpp <Game/Server/GameRoom.hpp>
///
///////////////////////////////////////////////////////////////////////////
class GameRoom
{

public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Connection creating a new game room
    ///
    ///////////////////////////////////////////////////////////////////////////
    GameRoom(
        ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
    );




    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Player manipulation
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Is the room currently occupied by 2 players
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto isFull() const
        -> bool;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Is the room currently occupied by 2 players
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto isRunning() const
        -> bool;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Does the room contains the player in argument
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto contains(
        ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> player
    ) const
        -> bool;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Get the opponent of the player in argument
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getOpponent(
        ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> player
    ) -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Get the player
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getPlayer1()
        -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Get the player
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getPlayer2()
        -> ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>>;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Join an already existing game
    ///
    ///////////////////////////////////////////////////////////////////////////
    void joinGame(
        ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
    );



private:

    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> m_player1;
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> m_player2{ nullptr };

    bool m_isRunning{ false };

    // ball propreties
    ::xrn::engine::component::Position m_ballPosition;
    // ::xrn::engine::component::Position m_ballRotation;

    // Tick
    ::std::thread m_tickThread;
    ::xrn::Time m_tickFrequencTimey{ ::xrn::Time::createAsSeconds(1) / 30 };

};

} // namespace game::server
