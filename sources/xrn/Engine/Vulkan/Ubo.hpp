#pragma once

#include <xrn/Engine/Component/PointLight.hpp>

namespace xrn::engine::vulkan {

///////////////////////////////////////////////////////////////////////////
/// \brief No clue what it does xD
/// \ingroup vulkan
///
/// \include Ubo.hpp <Ubo.hpp>
///
///////////////////////////////////////////////////////////////////////////
struct Ubo {
    ::glm::mat4 projection{ 1.0f };
    ::glm::mat4 view{ 1.0f };
    ::glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.03f }; // w is intensity

    static inline constexpr const ::std::size_t maxLights{ 10uz };
    ::xrn::engine::component::PointLight::PushConstant pointLights[maxLights];
    ::std::size_t numOfLights;
};

} // namespace xrn::engine::vulkan
