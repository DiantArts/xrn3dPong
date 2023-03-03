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
    : ::xrn::engine::AScene::AScene{ true /* isCameraDetached */}
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
// events
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

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onKeyPressed(
    ::std::int16_t keyCode
)
{}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onKeyReleased(
    ::std::int16_t keyCode
)
{}

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::onMouseMoved(
    ::glm::vec2 position
)
{}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::client::Scene::loadObjects()
{
    { // camera
        auto entity{ m_camera.getId() };
        m_registry.emplace<::xrn::engine::component::Control>(entity); // cameras are always controlled
        m_registry.emplace<::xrn::engine::component::Position>(entity, ::glm::vec3{ 0.0f, 0.0f, -7.5f });
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 90.0f, 0.0f, 0.0f });
    }

    { // player as cube
        // auto entity{ m_player };
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Cube"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.5f, -5.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 0.2, 0.2f, 0.01);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
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
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 1.0f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 2.0f, 2.0f, 5.0f);
    }
    // top
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, -1.f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 2.0f, 2.0f, 5.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -180.0f, 0.0f, 0.0f });
    }
    // left
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, -2.0f, 0.f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 1.0f, 2.0f, 5.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ m_registry.create() };
        m_registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(m_device, "Floor"));
        m_registry.emplace<::xrn::engine::component::Position>(entity, 2.0f, 0.f, 0.0f);
        m_registry.emplace<::xrn::engine::component::Scale>(entity, 1.0f, 2.0f, 5.0f);
        m_registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
        m_debugEntity = entity;
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
