#include <pch.hpp>
#include <xrn/Engine/Component/Position.hpp>
#include <xrn/Engine/Component/Control.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Position::Position()
    : m_position{ 0.0f, 0.0f, 0.0f }
    , m_isChanged{ true }
{}

///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Position::Position(
    ::glm::vec3 position
)
    : m_position{ ::std::move(position) }
    , m_isChanged{ true }
{}

///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Position::Position(
    float positionX
    , float positionY
    , float positionZ
)
    : m_position{ positionX, positionY, positionZ }
    , m_isChanged{ true }
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Update
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::update(
    float deltaTime
    , ::xrn::engine::component::Control& control
    , const ::glm::vec3& direction
)
{
    // search the number of directions moving in and removing speed when multiple direction at once
    auto speedDirectionDivider{ 2.0f };

    // TODO: error left/right - backward - down

    // bot top
    if (control.isAbleToFly()) {
        if (control.isMovingUp()) {
            if (!control.isMovingDown()) {
                speedDirectionDivider /= 1.5f;
            }
        } else if (control.isMovingDown()) {
            speedDirectionDivider /= 1.5f;
        }
    }

    // left right
    if (control.isMovingLeft()) {
        if (!control.isMovingRight()) {
            speedDirectionDivider /= 1.5f;
        }
    } else if (control.isMovingRight()) {
        speedDirectionDivider /= 1.5f;
    }

    // forward backward
    if (speedDirectionDivider > 1.0f) {
        if (control.isMovingForward()) {
            if (!control.isMovingBackward()) {
                speedDirectionDivider /= 1.5f;
            }
        } else if (control.isMovingBackward()) {
            speedDirectionDivider /= 1.5f;
        } else if (speedDirectionDivider == 2.0f) {
            return; // not any direction
        }
    }

    // apply movement
    auto velocity{ control.getSpeed() * deltaTime * speedDirectionDivider / 100'000 };

    // bot top
    if (control.isAbleToFly()) {
        if (control.isMovingUp()) {
            if (!control.isMovingDown()) {
                this->moveUp(velocity, direction);
            }
            speedDirectionDivider /= 2;
        } else if (control.isMovingDown()) {
            this->moveDown(velocity, direction);
        }
    }

    // left right
    if (control.isMovingLeft()) {
        if (!control.isMovingRight()) {
            this->moveLeft(velocity, direction);
        }
    } else if (control.isMovingRight()) {
        this->moveRight(velocity, direction);
    }

    // forward backward
    if (control.isMovingForward()) {
        if (!control.isMovingBackward()) {
            this->moveForward(velocity, direction);
        }
    } else if (control.isMovingBackward()) {
        this->moveBackward(velocity, direction);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Position
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveForward(
    const float velocity
    , const ::glm::vec3& direction
)
{
    m_position += velocity * direction;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveBackward(
    const float velocity
    , const ::glm::vec3& direction
)
{
    m_position -= velocity * direction;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveRight(
    const float velocity
    , const ::glm::vec3& direction
)
{
    m_position -= ::glm::normalize(::glm::cross(direction, ::glm::vec3{ 0.0f, 1.0f, 0.0f })) * velocity;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveLeft(
    const float velocity
    , const ::glm::vec3& direction
)
{
    m_position += ::glm::normalize(::glm::cross(direction, ::glm::vec3{ 0.0f, 1.0f, 0.0f })) * velocity;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveUp(
    const float velocity
    , const ::glm::vec3& direction [[ maybe_unused ]]
)
{
    m_position.y -= velocity;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveDown(
    const float velocity
    , const ::glm::vec3& direction [[ maybe_unused ]]
)
{
    m_position.y += velocity;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::move(
    const ::glm::vec3& offset
)
{
    m_position += offset;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::move(
    const float offsetX
    , const float offsetY
    , const float offsetZ
)
{
    m_position += ::glm::vec3{ offsetX, offsetY, offsetZ };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveX(
    const float offset
)
{
    m_position.x += offset;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveY(
    const float offset
)
{
    m_position.y -= offset;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::moveZ(
    const float offset
)
{
    m_position.z += offset;
    m_isChanged = true;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Setters getters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::set(
    ::glm::vec3 position
)
{
    m_position = ::std::move(position);
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::set(
    const float positionX
    , const float positionY
    , const float positionZ
)
{
    m_position = ::glm::vec3{ positionX, positionY, positionZ };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::setX(
    const float position
)
{
    m_position.x = position;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::setY(
    const float position
)
{
    m_position.y = position;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::setZ(
    const float position
)
{
    m_position.z = position;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Position::get() const
    -> const ::glm::vec3&
{
    return m_position;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] ::xrn::engine::component::Position::operator const ::glm::vec3&() const
{
    return m_position;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Others
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Position::isChanged() const
    -> bool
{
    return m_isChanged;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Position::resetChangedFlag()
{
    m_isChanged = false;
}
