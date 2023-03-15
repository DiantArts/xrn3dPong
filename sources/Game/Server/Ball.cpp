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
    , const ::xrn::engine::component::Position& position2
)
{
    this->updateBallDirection(position1, position2);

    m_position.update(deltaTime, m_control, m_rotation.getDirection());
}

///////////////////////////////////////////////////////////////////////////
void ::game::server::Ball::updateBallDirection(
    const ::xrn::engine::component::Position& position1
    , const ::xrn::engine::component::Position& position2
)
{
    if (m_position.get().x >= ::game::client::Scene::maxMapPosition.x) {
        const ::glm::vec2 normal{ 90.f, 0.f };
        m_rotation.setRotation(m_rotation.getXY() - normal);
    } else if (m_position.get().x <= -::game::client::Scene::maxMapPosition.x) {
        const ::glm::vec2 normal{ -90.f, 0.f };
        m_rotation.setRotation(m_rotation.getXY() - normal);
    }
    if (m_position.get().y >= ::game::client::Scene::maxMapPosition.y) {
        const ::glm::vec2 normal{ 0.f, 90.f };
        m_rotation.setRotation(m_rotation.getXY() - normal);
    } else if (m_position.get().y <= -::game::client::Scene::maxMapPosition.y) {
        const ::glm::vec2 normal{ 0.f, -90.f };
        m_rotation.setRotation(m_rotation.getXY() - normal);
    }

    for (auto& position : { position1.get(), position2.get() }) {
        auto beginHitbox{ position - (::game::client::Scene::playerScale) - ::glm::vec3{ .5f, .5f, 0.f } };
        auto endHitbox{ position + (::game::client::Scene::playerScale) + ::glm::vec3{ .5f, .5f, 0.f } };
        auto& ball{ m_position.get() };

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
            XRN_NOTE(
                "collision: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
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
            XRN_NOTE(
                "collision: [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}] < [{:.5};{:.5};{:.5}]"
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

        if (position.z < 0) {
            const ::glm::vec2 normal{ 0.f, 0.f };
            m_rotation.setRotation(m_rotation.getXY() - normal);
            m_position.set(ball.x, ball.y, endHitbox.z);
        } else {
            const ::glm::vec2 normal{ -10.f, 190.f };
            m_rotation.setRotation(m_rotation.getXY() - normal);
            m_position.set(ball.x, ball.y, beginHitbox.z);
        }
        break;
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
    m_rotation.updateDirection();
    XRN_INFO(
        "rotation: [{:.5};{:.5}]"
        , m_rotation.get().x
        , m_rotation.get().y
    );
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
    // m_rotation.setRotationX(45); // left to right
    // m_rotation.setRotationY(45); // left to right
    // m_rotation.setRotationX(90); // forward to backward
    m_rotation.setRotationX(85); // forward to backward
    m_control.startMovingForward();
}

///////////////////////////////////////////////////////////////////////////
auto ::game::server::Ball::getPosition() const
    -> const ::glm::vec3&
{
    return m_position.get();
}
