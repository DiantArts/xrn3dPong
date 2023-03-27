#pragma once

#include <xrn/Engine/Components.hpp>
#include <Game/MessageType.hpp>
#include <Game/Server/Player.hpp>

namespace game::server {

///////////////////////////////////////////////////////////////////////////
/// \brief Room for 2 players to play in
/// \ingroup game
///
/// \include Ball.hpp <Game/Server/Ball.hpp>
///
///////////////////////////////////////////////////////////////////////////
class Ball
{

public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Static
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    using Message = ::xrn::network::Message<::game::MessageType>;




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
    Ball();



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Rule of 5
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    virtual ~Ball();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    Ball(
        const Ball&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        const Ball&
    ) noexcept
        -> Ball& = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    Ball(
        Ball&&
    ) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        Ball&&
    ) noexcept
        -> Ball&;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Events
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// called with the m_tickFrequencyTime per seconds
    ///
    /// Start ticking when the second player is in the lobby
    ///////////////////////////////////////////////////////////////////////////
    void onTick(
        ::xrn::Time deltaTime
        , ::game::server::Player& player1
        , ::game::server::Player& player2
    );

    ///////////////////////////////////////////////////////////////////////////
    /// \brief update pos and check and resolve collision
    ///
    /// returns:
    ///     0: no collision
    ///     1: if collided with a player
    ///     2: if collided with a wall
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto updateBallRotation(
        ::game::server::Player& player1
        , ::game::server::Player& player2
    ) -> ::std::uint8_t;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief update pos and check and resolve collision
    ///
    /// returns:
    ///     0: no win condition met
    ///     1: player1 win
    ///     2: player2 win
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto checkWinCondition()
        -> ::std::uint8_t;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Getters/setters
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void setDefaultPropreties();

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getPosition() const
        -> const ::glm::vec3&;



private:

    ::xrn::engine::component::Control m_control;
    ::xrn::engine::component::Position m_position;
    ::xrn::engine::component::Rotation m_rotation;

};

} // namespace game::server
