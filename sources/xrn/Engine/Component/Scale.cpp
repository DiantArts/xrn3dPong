#include <pch.hpp>
#include <xrn/Engine/Component/Scale.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Scale::Scale() = default;

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Scale::Scale(
    float scale
)
    : m_scale{ ::glm::vec3{ scale } }
    , m_isChanged{ true }
{}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Scale::Scale(
    const ::glm::vec3& scale
)
    : m_scale{ ::glm::vec3{ scale } }
    , m_isChanged{ true }
{}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Scale::Scale(
    float scaleX
    , float scaleY
    , float scaleZ
)
    : m_scale{ ::glm::vec3{ scaleX, scaleY, scaleZ } }
    , m_isChanged{ true }
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Setters/Getters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scale(
    const float scale
)
{
    m_scale += ::glm::vec3{ scale };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scale(
    const ::glm::vec3& scale
)
{
    m_scale += scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scale(
    const float scaleX
    , const float scaleY
    , const float scaleZ
)
{
    m_scale += ::glm::vec3{ scaleX, scaleY, scaleZ };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scaleX(
    const float scale
)
{
    m_scale.x += scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scaleY(
    const float scale
)
{
    m_scale.y += scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::scaleZ(
    const float scale
)
{
    m_scale.z += scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::set(
    const float scale
)
{
    m_scale = ::glm::vec3{ scale };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::set(
    const ::glm::vec3& scale
)
{
    m_scale = ::std::move(scale);
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::set(
    const float scaleX
    , const float scaleY
    , const float scaleZ
)
{
    m_scale = ::glm::vec3{ scaleX, scaleY, scaleZ };
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::setX(
    const float scale
)
{
    m_scale.x = scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::setY(
    const float scale
)
{
    m_scale.y = scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::setZ(
    const float scale
)
{
    m_scale.z = scale;
    m_isChanged = true;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] auto ::xrn::engine::component::Scale::get() const
    -> const ::glm::vec3&
{
    return m_scale;
}

///////////////////////////////////////////////////////////////////////////
[[ nodiscard ]] ::xrn::engine::component::Scale::operator const ::glm::vec3&() const
{
    return m_scale;
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
auto ::xrn::engine::component::Scale::isChanged() const
    -> bool
{
    return m_isChanged;
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Scale::resetChangedFlag()
{
    m_isChanged = false;
}
