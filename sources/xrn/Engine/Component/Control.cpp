#include <pch.hpp>
#include <xrn/Engine/Component/Control.hpp>
#include <xrn/Engine/Component/Transform3d.hpp>
#include <xrn/Engine/Configuration.hpp>


// ------------------------------------------------------------------ *structors

///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Control::Control(
    bool ableToFly
)
    : m_ableToFly{ ableToFly }
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Speed
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::getSpeed() const
    -> float
{
    return m_moveSpeed;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::setSpeed(
    float speed
)
{
    m_moveSpeed = speed;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Start moving
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingForward()
{
    m_movementState[Control::MovementState::forward] = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingBackward()
{
    m_movementState[Control::MovementState::backward] = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingRight()
{
    m_movementState[Control::MovementState::right] = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingLeft()
{
    m_movementState[Control::MovementState::left] = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingUp()
{
    m_movementState[Control::MovementState::up] = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::startMovingDown()
{
    m_movementState[Control::MovementState::down] = true;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Stop moving
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingForward()
{
    m_movementState[Control::MovementState::forward] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingBackward()
{
    m_movementState[Control::MovementState::backward] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingRight()
{
    m_movementState[Control::MovementState::right] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingLeft()
{
    m_movementState[Control::MovementState::left] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingUp()
{
    m_movementState[Control::MovementState::up] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMovingDown()
{
    m_movementState[Control::MovementState::down] = false;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::stopMoving()
{
    m_movementState.reset();
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::immobilize()
{
    m_movementState.reset();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Check if moving
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingForward() const
    -> bool
{
    return m_movementState.test(Control::MovementState::forward);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingBackward() const
    -> bool
{
    return m_movementState.test(Control::MovementState::backward);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingRight() const
    -> bool
{
    return m_movementState.test(Control::MovementState::right);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingLeft() const
    -> bool
{
    return m_movementState.test(Control::MovementState::left);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingUp() const
    -> bool
{
    return m_movementState.test(Control::MovementState::up);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isMovingDown() const
    -> bool
{
    return m_movementState.test(Control::MovementState::down);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rotation
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotate(
    const ::glm::vec3& offset
)
{
    this->rotate(offset.x, offset.y, offset.z);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotate(
    const float rotationOffsetX,
    const float rotationOffsetY,
    const float rotationOffsetZ
)
{
    this->rotateX(rotationOffsetX);
    this->rotateY(rotationOffsetY);
    this->rotateZ(rotationOffsetZ);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateX(
    const float offset
)
{
    m_rotation.x += offset * ::xrn::engine::configuration.sensitivity.x;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateY(
    const float offset
)
{
    m_rotation.y += offset * ::xrn::engine::configuration.sensitivity.y;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateZ(
    const float offset
)
{
    m_rotation.y += offset * ::xrn::engine::configuration.sensitivity.z;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateAbsolute(
    ::glm::vec3 rotation
)
{
    m_rotation = ::std::move(rotation);
}


///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateAbsolute(
    const float rotationX,
    const float rotationY,
    const float rotationZ
)
{
    m_rotation = ::glm::vec3{ rotationX, rotationY, rotationZ };
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateAbsoluteX(
    const float rotationX
)
{
    m_rotation.x = rotationX;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateAbsoluteY(
    const float rotationY
)
{
    m_rotation.y = rotationY;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::rotateAbsoluteZ(
    const float rotationZ
)
{
    m_rotation.y = rotationZ;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] auto ::xrn::engine::component::Control::isRotated() const
    -> bool
{
    return m_rotation.x || m_rotation.y || m_rotation.z;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Control::resetRotatedFlag()
{
    m_rotation.x = 0;
    m_rotation.y = 0;
    m_rotation.z = 0;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::getRotation() const
    -> const ::glm::vec3&
{
    return m_rotation;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Others
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Control::isAbleToFly() const
    -> bool
{
    return m_ableToFly;
}
