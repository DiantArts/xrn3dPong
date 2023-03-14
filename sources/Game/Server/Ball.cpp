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
    , ::xrn::engine::component::Position& position1
    , ::xrn::engine::component::Position& position2
)
{
    this->updateBallDirection(position1, position2);

    m_position.update(deltaTime, m_control, m_rotation.getDirection());
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::Ball::updateBallDirection(
    ::xrn::engine::component::Position& position1
    , ::xrn::engine::component::Position& position2
)
{
    // bind the ball inside the map
    if (m_position.get().x >= ::game::client::Scene::maxMapPosition.x) {
        m_control.rotateAbsoluteX(180);
    } else if (m_position.get().x <= -::game::client::Scene::maxMapPosition.x) {
        m_control.rotateAbsoluteX(180);
    }
    if (m_position.get().y >= ::game::client::Scene::maxMapPosition.y) {
        m_control.rotateAbsoluteX(180);
    } else if (m_position.get().y <= -::game::client::Scene::maxMapPosition.y) {
        m_control.rotateAbsoluteX(180);
    }

    for (auto& position : { position1.get(), position2.get() }) {
        auto beginHitbox{ position - (::game::client::Scene::playerScale) - ::glm::vec3{ .5f, .5f, 0.f } };
        auto endHitbox{ position + (::game::client::Scene::playerScale) + ::glm::vec3{ .5f, .5f, 0.f } };
        auto& ball{ m_position.get() };

        if (position.z < 0) {
            XRN_INFO(
                "detection: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
                , endHitbox.x
                , endHitbox.y
                , endHitbox.z
                , ball.x
                , ball.y
                , ball.z
                , beginHitbox.x
                , beginHitbox.y
                , beginHitbox.z - 2.f
            );
        } else {
            XRN_INFO(
                "detection: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
                , beginHitbox.x
                , beginHitbox.y
                , beginHitbox.z
                , ball.x
                , ball.y
                , ball.z
                , endHitbox.x
                , endHitbox.y
                , endHitbox.z + 2.f
            );
        }

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
        // continue;

        XRN_DEBUG(
            "player collision: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
            , beginHitbox.x
            , beginHitbox.y
            , beginHitbox.z
            , ball.x
            , ball.y
            , ball.z
            , endHitbox.x
            , endHitbox.y
            , endHitbox.z
        );

        m_control.rotateAbsoluteX(180);
        if (position.z < 0) {
            m_position.set(ball.x, ball.y, endHitbox.z);
        } else {
            m_position.set(ball.x, ball.y, beginHitbox.z);
        }
        XRN_INFO(
            "player collision resolved: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
            , beginHitbox.x
            , beginHitbox.y
            , beginHitbox.z
            , ball.x
            , ball.y
            , ball.z
            , endHitbox.x
            , endHitbox.y
            , endHitbox.z
        );

        XRN_DEBUG("player collision");
    }

    if (m_position.get().z >= ::game::client::Scene::maxMapPosition.z + 5) { // player1 win
        XRN_INFO("Player1 won");
        // m_control.rotateAbsoluteX(180);
        m_control.resetRotatedFlag();
        m_rotation.setRotation(270, 0, 0);
        m_position.set(0, 0, 0);
    } else if (m_position.get().z <= -(::game::client::Scene::maxMapPosition.z + 5)) { // player2 win
        XRN_INFO("Player2 won");
        // m_control.rotateAbsoluteX(180);
        m_control.resetRotatedFlag();
        m_rotation.setRotation(90, 0, 0);
        m_position.set(0, 0, 0);
    }

    m_rotation.updateDirection(m_control);
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
    // m_control.rotateAbsoluteX(0); // left to right
    m_control.rotateAbsoluteX(90); // forward to backward
    m_control.startMovingForward();
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::getPosition() const
    -> const ::glm::vec3&
{
    return m_position.get();
}
