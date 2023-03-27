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
    , ::game::server::Player& player1
    , ::game::server::Player& player2
)
{
    m_rotation.updateDirection();

    // XRN_INFO(
        // "rotation: [{:.5};{:.5}]"
        // , m_rotation.get().x
        // , m_rotation.get().y
    // );

    m_position.update(deltaTime, m_control, m_rotation.getDirection());
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::updateBallRotation(
    ::game::server::Player& player1
    , ::game::server::Player& player2
) -> ::std::uint8_t
{
    auto isCollided{ false };
    auto& ball{ m_position.get() };

    if (m_position.get().x >= ::game::client::Scene::maxMapPosition.x) { // left?
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.setRotation(normal - m_rotation.getXY());
        m_position.set(::game::client::Scene::maxMapPosition.x, ball.y, ball.z);
        isCollided = true;
    } else if (m_position.get().x <= -::game::client::Scene::maxMapPosition.x) { // right?
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.setRotation(normal - m_rotation.getXY());
        m_position.set(-::game::client::Scene::maxMapPosition.x, ball.y, ball.z);
        isCollided = true;
    }
    if (m_position.get().y >= ::game::client::Scene::maxMapPosition.y) { // bot
        m_rotation.setRotationY(-m_rotation.get().y);
        m_position.set(ball.x, ::game::client::Scene::maxMapPosition.y, ball.z);
        isCollided = true;
    } else if (m_position.get().y <= -::game::client::Scene::maxMapPosition.y) { // top
        m_rotation.setRotationY(-m_rotation.get().y);
        m_position.set(ball.x, -::game::client::Scene::maxMapPosition.y, ball.z);
        isCollided = true;
    }

    for (auto& player : { player1, player2 }) {
        auto beginHitbox{ player.position.get() - (::game::client::Scene::playerScale) - ::glm::vec3{ .5f, .5f, 0.f } };
        auto endHitbox{ player.position.get() + (::game::client::Scene::playerScale) + ::glm::vec3{ .5f, .5f, 0.f } };

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

        if (player.position.get().z < 0) {
            const ::glm::vec2 normal{ 180.f, 180.f };
            m_rotation.setRotation(normal - m_rotation.getXY());
            m_position.set(ball.x, ball.y, endHitbox.z);
        } else {
            const ::glm::vec2 normal{ 180.f, 180.f };
            m_rotation.setRotation(normal - m_rotation.getXY());
            m_position.set(ball.x, ball.y, beginHitbox.z);
        }
        return player.id;
    }

    return isCollided ? 1 : 0;
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::checkWinCondition()
    -> ::std::uint8_t
{
    if (m_position.get().z >= ::game::client::Scene::maxMapPosition.z + 5) { // player1 win
        m_rotation.setRotation(270, 0, 0);
        m_position.set(0, 0, 0);
        return 1;
    } else if (m_position.get().z <= -(::game::client::Scene::maxMapPosition.z + 5)) { // player2 win
        m_rotation.setRotation(90, 0, 0);
        m_position.set(0, 0, 0);
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
void ::game::server::Ball::setDefaultPropreties()
{
    m_control.setSpeed(3000);
    m_position.set(0, 0, 0);
    // m_rotation.setRotationX(90);
    m_rotation.setRotation(70, 20, 0);
    m_control.startMovingForward();
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::getPosition() const
    -> const ::glm::vec3&
{
    return m_position.get();
}
