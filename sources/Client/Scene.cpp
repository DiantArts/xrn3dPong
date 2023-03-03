///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Util.hpp>
#include <Client/Scene.hpp>
#include <xrn/Engine/Components.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::client::Scene::Scene()
    : ::xrn::engine::AScene::AScene{ false /* isCameraDetached */}
{
    this->loadObjects();
    this->loadMap();
    this->loadLights();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Rule of 5
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::client::Scene::~Scene() = default;



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Overrides
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
auto ::client::Scene::onUpdate()
    -> bool
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
auto ::client::Scene::postUpdate()
    -> bool
{
    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadObjects()
{
    { // player as cube
        auto entity{ m_player };
        // m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.4f, -1.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 0.1f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // camera
        auto entity{ m_camera.getId() };
        m_registry.emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        m_registry.emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -6.5f });
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // vase 1
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "FlatVase"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, -0.25f, 0.5f, 0.0f);
    }

    { // vase 2
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "SmoothVase"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.25f, 0.5f, 0.0f);
    }
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadMap()
{
    // bot
    for (auto x{ -1.0f }; x <= 1.0f; x += 2.0f) {
        for (auto z{ -3.0f }; z <= 3.0f; z += 2.0f) {
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
            m_registry.emplace<::xrn::engine::component::Position>(entity, x, 1.0f, z);
        }
    }

    // top
    for (auto x{ -1.0f }; x <= 1.0f; x += 2.0f) {
        for (auto z{ -3.0f }; z <= 3.0f; z += 2.0f) {
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
            m_registry.emplace<::xrn::engine::component::Position>(entity, x, -1.0f, z);
        }
    }

    // left
    for (auto y{ 0.0f }; y <= 0.0f; y += 2.0f) {
        for (auto z{ -3.0f }; z <= 3.0f; z += 2.0f) {
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
            m_registry.emplace<::xrn::engine::component::Position>(entity, -2.0f, y, z);
            m_registry.emplace<::xrn::engine::component::Rotation>(entity, 0.0f, 0.0f, 90.0f);
        }
    }

    // right
    for (auto y{ 0.0f }; y <= 0.0f; y += 2.0f) {
        for (auto z{ -3.0f }; z <= 3.0f; z += 2.0f) {
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
            m_registry.emplace<::xrn::engine::component::Position>(entity, 2.0f, y, z);
            m_registry.emplace<::xrn::engine::component::Rotation>(entity, 0.0f, 0.0f, 90.0f);
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadLights()
{
    { // lights
        std::vector<glm::vec3> lightColors{
            { 1.f, .1f, .1f },
            { .1f, .1f, 1.f },
            { .1f, 1.f, .1f },
            { 1.f, 1.f, .1f },
            { .1f, 1.f, 1.f },
            { 1.f, 1.f, 1.f }
        };

        // create the lights at equal distances from each other in circle
        for (auto i{ 0uz }; const auto& color : lightColors) {
            auto rotation{ ::glm::rotate(
                ::glm::mat4(1.0f),
                i * ::glm::two_pi<float>() / lightColors.size(),
                { 0.0f, -1.0f, 0.0f }
            ) };
            auto entity{ m_registry.create() };
            m_registry.emplace<::xrn::engine::component::Position>(
                entity, ::glm::vec3{ rotation * ::glm::vec4{ -0.0f, -0.0f, -1.5f, 1.0f } }
            );
            m_registry.emplace<::xrn::engine::component::Rotation>(entity, 90.0f, 0.0f, 0.0f);

            // make them move
            m_registry.emplace<::xrn::engine::component::Control>(entity);

            m_registry.emplace<::xrn::engine::component::PointLight>(entity, color);
            ++i;
        }
    }
}
