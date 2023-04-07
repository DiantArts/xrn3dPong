///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/MousePointer.hpp>
#include "xrn/Engine/Component/Position.hpp"



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::MousePointer::MousePointer(
    ::xrn::engine::Camera& camera
    , ::xrn::engine::vulkan::Window& window
)
{
    this->updateRay(camera, window);
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Getters/setters
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::MousePointer::updateRay(
    ::xrn::engine::Camera& camera
)
{
    // convert to eye coordinates
    auto eyeCoordinate{ ::glm::inverse(camera.getProjection()) * m_normalizedMouseCoordinates };
    eyeCoordinate.z = -1.0f;
    eyeCoordinate.w = 0.0f;

    // convert to world coordinates
    auto rayWorld{ ::glm::inverse(camera.getView()) * eyeCoordinate };
    m_ray = ::glm::normalize(::glm::vec3{ rayWorld.x, rayWorld.y, rayWorld.z });
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::MousePointer::updateRay(
    ::xrn::engine::Camera& camera
    , ::xrn::engine::vulkan::Window& window
)
{
    auto mousePosition{ window.getCursorPosition() };
    m_normalizedMouseCoordinates = ::glm::vec4{
        (2.0f * mousePosition.x) / window.getSize().width - 1
        , (2.0f * mousePosition.y) / window.getSize().height - 1
        , -1.0f
        , 1.0f
    };

    this->updateRay(camera);
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::MousePointer::getRay() const
    -> const ::glm::vec3&
{
    return m_ray;
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::MousePointer::castToPlane(
    const ::xrn::engine::component::Position& cameraPosition
    , const ::glm::vec3& planePosition
    , const ::glm::vec3& planeRotation
) const
    -> ::glm::vec3
{
    auto normalizePlane{ [](const ::glm::vec3& planeRotation){
        ::glm::mat4 rotationMatrix{
            ::glm::rotate(::glm::mat4(1.0f), planeRotation.z, ::glm::vec3(0, 0, 1))
            * ::glm::rotate(::glm::mat4(1.0f), planeRotation.y, ::glm::vec3(0, 1, 0))
            * ::glm::rotate(::glm::mat4(1.0f), planeRotation.x, ::glm::vec3(1, 0, 0))
        };
        return ::glm::normalize(::glm::vec3{ rotationMatrix * ::glm::vec4{ 0, 0, 1, 0 } });
    } };
    ::glm::vec3 planeNormal{ normalizePlane(planeRotation) };
    auto t{ (::glm::dot(planeNormal, cameraPosition.get()) - ::glm::dot(planeNormal, planePosition)) / ::glm::dot(planeNormal, m_ray) };
    return cameraPosition.get() + t * m_ray;
}
