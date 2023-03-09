#pragma once

#include <xrn/Engine/Configuration/KeyBindings.hpp>
#include <xrn/Engine/Configuration/Filepaths.hpp>

namespace xrn::engine::detail::configuration {

///////////////////////////////////////////////////////////////////////////
/// \brief No clue what it does xD
/// \ingroup vulkan
///
/// \include KeyBindings.hpp <KeyBindings.hpp>
///
///////////////////////////////////////////////////////////////////////////
struct Configuration
{
    ::xrn::engine::detail::configuration::Filepaths filepath{};

    // Keys
    ::xrn::engine::detail::configuration::KeyBindings keyBinding{};
    float moveSpeed{ 3.0f };
    ::glm::vec3 sensitivity{ 0.05f }; // used for mouse especially

    // Rotation
    ::glm::vec3 rotateSpeed{ 1.5f }; // when rotating with keys
    static inline constexpr float minPitch{ -89.999f };
    static inline constexpr float maxPitch{ 89.999f };

    // others
    ::std::uint16_t maxFrameRate{ 60 };
    static inline constexpr ::std::size_t defaultTickFrequency{ 5 };
};

} // namespace xrn::engine::detail::configuration



///////////////////////////////////////////////////////////////////////////
// Alias
///////////////////////////////////////////////////////////////////////////
namespace xrn::engine::detail { using Configuration = ::xrn::engine::detail::configuration::Configuration; }
namespace xrn::engine { using Configuration = ::xrn::engine::detail::Configuration; }



///////////////////////////////////////////////////////////////////////////
// Global
///////////////////////////////////////////////////////////////////////////
namespace xrn::engine::detail::configuration {
    extern ::xrn::engine::detail::configuration::Configuration configuration;
    static inline constexpr auto& config{ ::xrn::engine::detail::configuration::configuration };
}
namespace xrn::engine::detail {
    static inline constexpr auto& config{ ::xrn::engine::detail::configuration::configuration };
}
namespace xrn::engine {
    static inline constexpr auto& configuration{ ::xrn::engine::detail::configuration::configuration };
    static inline constexpr auto& config{ ::xrn::engine::detail::configuration::configuration };
}
