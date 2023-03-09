#include <pch.hpp>
#include <xrn/Engine/Component/Collision.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
::xrn::engine::component::Collision::Collision()
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Matrix
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateMatrix(
    const xrn::engine::component::Position& position
    , const xrn::engine::component::Rotation& rotation
    , const xrn::engine::component::Scale& scale
)
{
    const float c3{ ::glm::cos(::glm::radians(rotation.get().z)) };
    const float s3{ ::glm::sin(::glm::radians(rotation.get().z)) };
    const float c2{ ::glm::cos(::glm::radians(rotation.get().x)) };
    const float s2{ ::glm::sin(::glm::radians(rotation.get().x)) };
    const float c1{ ::glm::cos(::glm::radians(rotation.get().y)) };
    const float s1{ ::glm::sin(::glm::radians(rotation.get().y)) };

    m_matrix = ::glm::mat4{
        {
            scale.get().x * (c1 * c3 + s1 * s2 * s3)
            , scale.get().x * (c2 * s3)
            , scale.get().x * (c1 * s2 * s3 - c3 * s1)
            , 0.0f
        }
        , {
            scale.get().y * (c3 * s1 * s2 - c1 * s3)
            , scale.get().y * (c2 * c3)
            , scale.get().y * (c1 * c3 * s2 + s1 * s3)
            , 0.0f
        }
        , {
            scale.get().z * (c2 * s1)
            , scale.get().z * (-s2)
            , scale.get().z * (c1 * c2)
            , 0.0f
        }
        , { position.get().x, position.get().y, position.get().z, 1.0f }
    };
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateMatrix(
    const xrn::engine::component::Position& position
    , const xrn::engine::component::Rotation& rotation
)
{
    this->updateMatrix(position, rotation, ::glm::vec3{ 1.0f });
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateMatrix(
    const xrn::engine::component::Position& position
    , const xrn::engine::component::Scale& scale
)
{
    this->updateMatrix(position, ::glm::vec3{ 0.0f }, scale);
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateMatrix(
    const xrn::engine::component::Position& position
)
{
    this->updateMatrix(position, ::glm::vec3{ 0.0f }, ::glm::vec3{ 1.0f });
}

///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Collision::getMatrix() const
    -> const ::glm::mat4&
{
    return m_matrix;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Normal Matrix
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateNormalMatrix(
    const xrn::engine::component::Rotation& rotation
    , const xrn::engine::component::Scale& scale
)
{
    const float c3{ ::glm::cos(::glm::radians(rotation.get().z)) };
    const float s3{ ::glm::sin(::glm::radians(rotation.get().z)) };
    const float c2{ ::glm::cos(::glm::radians(rotation.get().x)) };
    const float s2{ ::glm::sin(::glm::radians(rotation.get().x)) };
    const float c1{ ::glm::cos(::glm::radians(rotation.get().y)) };
    const float s1{ ::glm::sin(::glm::radians(rotation.get().y)) };
    const ::glm::vec3 inversedScale{ 1.0f / scale.get() };

    m_normalMatrix = ::glm::mat3{
        {
            inversedScale.x * (c1 * c3 + s1 * s2 * s3)
            , inversedScale.x * (c2 * s3)
            , inversedScale.x * (c1 * s2 * s3 - c3 * s1)
        }
        , {
            inversedScale.y * (c3 * s1 * s2 - c1 * s3)
            , inversedScale.y * (c2 * c3)
            , inversedScale.y * (c1 * c3 * s2 + s1 * s3)
        }
        , {
            inversedScale.z * (c2 * s1)
            , inversedScale.z * (-s2)
            , inversedScale.z * (c1 * c2)
        }
    };
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateNormalMatrix(
    const xrn::engine::component::Rotation& rotation
)
{
    return this->updateNormalMatrix(rotation.get(), ::glm::vec3{ 1.0f });
}

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::component::Collision::updateNormalMatrix(
    const xrn::engine::component::Scale& scale
)
{
    return this->updateNormalMatrix(::glm::vec3{ 0.0f }, scale);
}

///////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////
auto ::xrn::engine::component::Collision::getNormalMatrix() const
    -> const ::glm::mat3&
{
    return m_normalMatrix;
}
