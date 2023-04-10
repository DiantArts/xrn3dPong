#pragma once

#include <xrn/Engine/Components.hpp>

namespace game::server {

///////////////////////////////////////////////////////////////////////////
/// \brief Room for 2 players to play in
/// \ingroup game
///
/// \include Player.hpp <Game/Server/Player.hpp>
///
///////////////////////////////////////////////////////////////////////////
struct Player
{
    ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection{ nullptr };
    ::xrn::engine::component::Position position{ 0.0f, 0.0f, -50.0f };
    ::glm::vec3 burstSpeed{ 0.0f, 0.0f, 0.0f };
    ::std::uint8_t id{ 1 };
    ::std::uint8_t score{ 0 };

};

} // namespace game::server
