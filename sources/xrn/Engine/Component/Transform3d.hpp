#pragma once

#include <xrn/Engine/Vulkan/Model.hpp>
#include <xrn/Engine/Component/Position.hpp>
#include <xrn/Engine/Component/Rotation.hpp>
#include <xrn/Engine/Component/Scale.hpp>

namespace xrn::engine::component {

class Transform3d {

public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    Transform3d(
        ::std::unique_ptr<::xrn::engine::vulkan::Model>&& model
    );



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Matrix
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateMatrix(
        const xrn::engine::component::Position& position
        , const xrn::engine::component::Rotation& rotation
        , const xrn::engine::component::Scale& scale
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateMatrix(
        const xrn::engine::component::Position& position
        , const xrn::engine::component::Rotation& rotation
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateMatrix(
        const xrn::engine::component::Position& position
        , const xrn::engine::component::Scale& scale
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateMatrix(
        const xrn::engine::component::Position& position
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getMatrix() const
        -> const ::glm::mat4&;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Normal Matrix
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateNormalMatrix(
        const xrn::engine::component::Rotation& rotation
        , const xrn::engine::component::Scale& scale
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateNormalMatrix(
        const xrn::engine::component::Rotation& rotation
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void updateNormalMatrix(
        const xrn::engine::component::Scale& scale
    );

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getNormalMatrix() const
        -> const ::glm::mat3&;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Model
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto getModel() const
        -> ::xrn::engine::vulkan::Model&;



private:

    ::std::unique_ptr<::xrn::engine::vulkan::Model> m_model;
    ::glm::mat4 m_matrix{ 1.0f };
    ::glm::mat3 m_normalMatrix{ 1.0f };

};

} // namespace xrn::engine::component
