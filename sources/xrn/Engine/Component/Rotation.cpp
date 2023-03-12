#include <pch.hpp>
#include <xrn/Engine/Component/Rotation.hpp>
#include <xrn/Engine/Component/Control.hpp>
#include <xrn/Engine/Configuration.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Rotation::Rotation() = default;

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Rotation::Rotation(
    ::glm::vec3 offset
)
{
    this->setRotation(::std::move(offset));
}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Rotation::Rotation(
    float rotationXOffset
    , float rotationYOffset
    , float rotationZOffset
)
{
    this->rotate(rotationXOffset, rotationYOffset, rotationZOffset);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rotation
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::rotate(
    const ::glm::vec3& offset
)
{
    if (offset.x != 0) {
        this->rotateX(offset.x);
    }
    if (offset.y != 0) {
        this->rotateY(offset.y);
    }
    if (offset.z != 0) {
        this->rotateZ(offset.z);
    }
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::rotate(
    const float rotationXOffset
    , const float rotationYOffset
    , const float rotationZOffset
)
{
    if (rotationXOffset != 0) {
        this->rotateX(rotationXOffset);
    }
    if (rotationYOffset != 0) {
        this->rotateY(rotationYOffset);
    }
    if (rotationZOffset != 0) {
        this->rotateZ(rotationZOffset);
    }
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::rotateX(
    const float offset
)
{
    m_rotation.x += offset;
    while (m_rotation.x >= 360) {
        m_rotation.x -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::rotateY(
    const float offset
)
{
    m_rotation.y += offset;
    while (m_rotation.y >= 360) {
        m_rotation.y -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::rotateZ(
    const float offset
)
{
    m_rotation.z += offset;
    while (m_rotation.z >= 360) {
        m_rotation.z -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::setRotation(
    ::glm::vec3 rotation
)
{
    m_rotation = ::std::move(rotation);
    while (m_rotation.x >= 360) {
        m_rotation.x -= 360;
    }
    while (m_rotation.y >= 360) {
        m_rotation.y -= 360;
    }
    while (m_rotation.z >= 360) {
        m_rotation.z -= 360;
    }
    m_isChanged = true;
}


///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::setRotation(
    const float rotationX
    , const float rotationY
    , const float rotationZ
)
{
    m_rotation = ::glm::vec3{ rotationX, rotationY, rotationZ };
    while (m_rotation.x >= 360) {
        m_rotation.x -= 360;
    }
    while (m_rotation.y >= 360) {
        m_rotation.y -= 360;
    }
    while (m_rotation.z >= 360) {
        m_rotation.z -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::setRotationX(
    const float rotationX
)
{
    m_rotation.x = rotationX;
    while (m_rotation.x >= 360) {
        m_rotation.x -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::setRotationY(
    const float rotationY
)
{
    m_rotation.y = rotationY;
    while (m_rotation.y >= 360) {
        m_rotation.y -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::setRotationZ(
    const float rotationZ
)
{
    m_rotation.y = rotationZ;
    while (m_rotation.z >= 360) {
        m_rotation.z -= 360;
    }
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] auto ::xrn::engine::component::Rotation::get() const
    -> const ::glm::vec3&
{
    return m_rotation;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] ::xrn::engine::component::Rotation::operator const ::glm::vec3&() const
{
    return m_rotation;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Direction
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::updateDirection(
    ::xrn::engine::component::Control& control
)
{
    if (control.isRotated() || this->isChanged()) {
        auto newRotation{ m_rotation + control.getRotation() };

        if (newRotation.y > ::xrn::engine::configuration.maxPitch) {
            newRotation.y = ::xrn::engine::configuration.maxPitch;
        } else if (newRotation.y < ::xrn::engine::configuration.minPitch) {
            newRotation.y = ::xrn::engine::configuration.minPitch;
        }

        this->setRotation(::std::move(newRotation));
        m_direction = ::glm::normalize(::glm::vec3(
            ::glm::cos(::glm::radians(m_rotation.x)) * ::glm::cos(::glm::radians(m_rotation.y))
            , ::glm::sin(::glm::radians(m_rotation.y))
            , ::glm::sin(::glm::radians(m_rotation.x)) * ::glm::cos(::glm::radians(m_rotation.y))
        ));
        control.resetRotatedFlag();
    }
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Rotation::getDirection() const
    -> const ::glm::vec3&
{
    return m_direction;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Others
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Rotation::isChanged() const
    -> bool
{
    return m_isChanged;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Rotation::resetChangedFlag()
{
    m_isChanged = false;
}
