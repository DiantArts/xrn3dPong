#pragma once

#include <xrn/Network/Server/Server.hpp>
#include <Game/MessageType.hpp>
#include <Game/Server/GameRoom.hpp>

namespace game {

///////////////////////////////////////////////////////////////////////////
/// \brief Server of the game
/// \ingroup game
///
/// \include Server.hpp <Game/Server/Server.hpp>
///
///////////////////////////////////////////////////////////////////////////
class Server
    : public ::xrn::network::server::Server<::game::MessageType>
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
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    Server(
        ::std::uint16_t port
    );



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
    virtual void onReceive(
        ::xrn::network::Message<::game::MessageType>& message
        , ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
    ) override;

    ///////////////////////////////////////////////////////////////////////////
    /// called with the m_tickFrequencyTime per seconds
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto onTick(
        ::xrn::Time deltaTime
    ) -> bool;



private:

    ::std::vector<::std::unique_ptr<::game::server::GameRoom>> m_rooms;

    ::std::mutex m_mutex;

};

}
