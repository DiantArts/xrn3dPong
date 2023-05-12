///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Server/Ball.hpp>
#include <Game/Client/Scene.hpp>
#include <Game/MessageType.hpp>
#include <Game/Map.hpp>
#include <xrn/Engine/Configuration.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::server::Ball::Ball() = default;



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::game::server::Ball::~Ball() = default;

///////////////////////////////////////////////////////////////////////////
::game::server::Ball::Ball(
    Ball&&
) noexcept = default;

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::operator=(
    Ball&&
) noexcept
    -> Ball& = default;



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Events
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::server::Ball::onTick(
    ::xrn::Time deltaTime
)
{
    m_rotation.rotate(m_rotationEffect);
    m_rotation.updateDirection();
    m_position.update(deltaTime, m_control, m_rotation.getDirection());
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::resolveBallCollisionWithWalls()
    -> bool
{
    auto isCollided{ false };
    auto& ball{ m_position.get() };

    if (m_position.get().x >= ::game::Map::maxMapPosition.x) { // left
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.set(normal - m_rotation.getXY());
        m_position.set(::game::Map::maxMapPosition.x, ball.y, ball.z);
        isCollided = true;
    } else if (m_position.get().x <= -::game::Map::maxMapPosition.x) { // right
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.set(normal - m_rotation.getXY());
        m_position.set(-::game::Map::maxMapPosition.x, ball.y, ball.z);
        isCollided = true;
    }
    if (m_position.get().y >= ::game::Map::maxMapPosition.y) { // bot
        m_rotation.setY(-m_rotation.get().y);
        m_position.set(ball.x, ::game::Map::maxMapPosition.y, ball.z);
        isCollided = true;
    } else if (m_position.get().y <= -::game::Map::maxMapPosition.y) { // top
        m_rotation.setY(-m_rotation.get().y);
        m_position.set(ball.x, -::game::Map::maxMapPosition.y, ball.z);
        isCollided = true;
    }

    if (isCollided) {
        m_rotationEffect = ::glm::vec3{ 0 };
    }
    return isCollided;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::updateBallRotation(
    ::game::server::Player& player1
    , ::game::server::Player& player2
) -> ::std::uint8_t
{
    auto& ball{ m_position.get() };

    if (this->resolveBallCollisionWithWalls()) {
        return 3;
    }

    for (auto& player : { player1, player2 }) {
        auto beginHitbox{ player.position.get() - (::game::Map::playerScale) - ::glm::vec3{ .5f, .5f, 0.f } };
        auto endHitbox{ player.position.get() + (::game::Map::playerScale) + ::glm::vec3{ .5f, .5f, 0.f } };

        if (
            ball.x < beginHitbox.x || ball.x > endHitbox.x ||
            ball.y < beginHitbox.y || ball.y > endHitbox.y
        ) {
            continue;
        }
        if (player.position.get().z < 0) {
            // add 1 to componsate the lag
            if (ball.z < beginHitbox.z - 2.f || ball.z > endHitbox.z) {
                continue;
            }
        } else {
            // add 1 to componsate the lag
            if (ball.z > endHitbox.z + 2.f || ball.z < beginHitbox.z) {
                continue;
            }
        }

        const ::glm::vec2 normal{ 180.f, 180.f };
        m_rotation.set(normal - m_rotation.getXY());

        if (m_rotation.get().x < 40.f) {
            m_rotation.setX(40.f);
        } else if (m_rotation.get().x > 120.f) {
            m_rotation.setX(120.f);
        }
        if (m_position.get().z < 0) { // player1 max ball rotation
            if (m_rotation.get().y < 20.f || m_rotation.get().y > 340.f) { // temporary fix
            } else if (m_rotation.get().y < 180.f) {
                m_rotation.setY(20.f);
            } else {
                m_rotation.setY(340.f);
            }
        } else { // player2 max ball rotation
            if (m_rotation.get().y < 160.f) {
                m_rotation.setY(160.f);
            } else if (m_rotation.get().y > 200.f) {
                m_rotation.setY(200.f);
            }
        }

        // .z is always 0 becacuse the player cannot move on z axis
        m_rotationEffect.x = player.burstSpeed.x * .5f;
        m_rotationEffect.y = player.burstSpeed.y * -.5f;

        m_position.set(ball.x, ball.y, beginHitbox.z);
        return player.id;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::checkWinCondition()
    -> ::std::uint8_t
{
    if (m_position.get().z >= ::game::Map::maxMapPosition.z + 5) { // player1 win
        m_rotation.set(270, 0, 0);
        m_position.set(0, 0, 0);
        m_rotationEffect = ::glm::vec3{ 0 };
        return 1;
    } else if (m_position.get().z <= -(::game::Map::maxMapPosition.z + 5)) { // player2 win
        m_rotation.set(90, 0, 0);
        m_position.set(0, 0, 0);
        m_rotationEffect = ::glm::vec3{ 0 };
        return 2;
    }
    return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Getters/setters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::server::Ball::setDefaultPropreties(
    bool oppositeDirection // = false
)
{
    m_control.setSpeed(3000);
    m_position.set(0, 0, 0);
    if (oppositeDirection) {
        m_rotation.setX(90 + 180);
    } else {
        m_rotation.setX(90);
    }
    m_control.startMovingForward();
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::getPosition() const
    -> const ::glm::vec3&
{
    return m_position.get();
}
