///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/Event/MouseMoved.hpp>
#include <xrn/Engine/Component/Control.hpp>
#include <xrn/Engine/Configuration.hpp>
#include <xrn/Engine/AScene.hpp>


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::event::MouseMoved::MouseMoved(
    ::glm::vec2 offset
)
    : m_offset{ offset.x, offset.y }
{
}

///////////////////////////////////////////////////////////////////////////
::xrn::engine::event::MouseMoved::MouseMoved(
    float offsetX,
    float offsetY
)
    : m_offset{ offsetX, offsetY }
{
}

///////////////////////////////////////////////////////////////////////////
::xrn::engine::event::MouseMoved::MouseMoved(
    double offsetX,
    double offsetY
)
    : m_offset{
        static_cast<float>(offsetX),
        static_cast<float>(offsetY)
    }
{
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Resolve
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::event::MouseMoved::resolve(
    ::xrn::engine::AScene& scene
)
{
    scene.onMouseMoved(m_offset);
}
