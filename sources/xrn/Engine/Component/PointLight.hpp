#pragma once

#include <xrn/Engine/Component/Position.hpp>

namespace xrn::engine::component {

struct PointLight {

    struct PushConstant {
        PushConstant() = default;
        PushConstant(
            const ::xrn::engine::component::PointLight& pointLight,
            const ::xrn::engine::component::Position& position
        )
            : color{ pointLight.color }
            , position{ position.get(), pointLight.radius }
        {}
        ::glm::vec4 color; // w == intensity
        ::glm::vec4 position; // w == radius
    };

    PointLight(
        ::glm::vec3 thatColor = ::glm::vec3{ 1.0f },
        float intensity = 10.0f,
        float thatRadius = 1.0f
    )
        : color{ thatColor, intensity }
        , radius{ thatRadius }
    {}

    ::glm::vec4 color; // w == intensity
    float radius;
};

} // namespace xrn::engine::component
