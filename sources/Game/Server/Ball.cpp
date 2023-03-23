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
    , const ::xrn::engine::component::Position& position1
    , const ::glm::vec3& burstSpeed1
    , const ::xrn::engine::component::Position& position2
    , const ::glm::vec3& burstSpeed2
)
{
    this->updateBallRotation(position1, burstSpeed1, position2, burstSpeed2);
    m_rotation.updateDirection();

    // XRN_INFO(
        // "rotation: [{:.5};{:.5}]"
        // , m_rotation.get().x
        // , m_rotation.get().y
    // );

    m_position.update(deltaTime, m_control, m_rotation.getDirection());
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::Ball::updateBallRotation(
    const ::xrn::engine::component::Position& position1
    , const ::glm::vec3& burstSpeed1
    , const ::xrn::engine::component::Position& position2
    , const ::glm::vec3& burstSpeed2
)
{
    auto& ball{ m_position.get() };

    if (m_position.get().x >= ::game::client::Scene::maxMapPosition.x) { // left?
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.setRotation(normal - m_rotation.getXY());
        m_position.set(::game::client::Scene::maxMapPosition.x, ball.y, ball.z);
    } else if (m_position.get().x <= -::game::client::Scene::maxMapPosition.x) { // right?
        const ::glm::vec2 normal{ 0.f, 180.f };
        m_rotation.setRotation(normal - m_rotation.getXY());
        m_position.set(-::game::client::Scene::maxMapPosition.x, ball.y, ball.z);
    }
    if (m_position.get().y >= ::game::client::Scene::maxMapPosition.y) { // bot
        m_rotation.setRotationY(-m_rotation.get().y);
        m_position.set(ball.x, ::game::client::Scene::maxMapPosition.y, ball.z);
    } else if (m_position.get().y <= -::game::client::Scene::maxMapPosition.y) { // top
        m_rotation.setRotationY(-m_rotation.get().y);
        m_position.set(ball.x, -::game::client::Scene::maxMapPosition.y, ball.z);
    }

    for (auto& position : { position1.get(), position2.get() }) {
        auto beginHitbox{ position - (::game::client::Scene::playerScale) - ::glm::vec3{ .5f, .5f, 0.f } };
        auto endHitbox{ position + (::game::client::Scene::playerScale) + ::glm::vec3{ .5f, .5f, 0.f } };

        if (
            ball.x < beginHitbox.x || ball.x > endHitbox.x ||
            ball.y < beginHitbox.y || ball.y > endHitbox.y
        ) {
            continue;
        }
        if (position.z < 0) {
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

        if (position.z < 0) {
            const ::glm::vec2 normal{ 180.f, 180.f };
            m_rotation.setRotation(normal - m_rotation.getXY());
            m_position.set(ball.x, ball.y, endHitbox.z);
        } else {
            const ::glm::vec2 normal{ 180.f, 180.f };
            m_rotation.setRotation(normal - m_rotation.getXY());
            m_position.set(ball.x, ball.y, beginHitbox.z);
        }
        return;
    }

    if (m_position.get().z >= ::game::client::Scene::maxMapPosition.z + 5) { // player1 win
        XRN_INFO("Player1 won");
        m_rotation.setRotation(270, 0, 0);
        m_position.set(0, 0, 0);
    } else if (m_position.get().z <= -(::game::client::Scene::maxMapPosition.z + 5)) { // player2 win
        XRN_INFO("Player2 won");
        m_rotation.setRotation(90, 0, 0);
        m_position.set(0, 0, 0);
    }
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
