///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Map.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Helpers
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::game::Map::loadObjects(
    ::entt::registry& registry
    , ::xrn::engine::vulkan::Device* device
    , ::entt::entity player
    , ::entt::entity enemy
    , ::entt::entity ball
)
{
    { // player
        auto entity{ player };
        registry.emplace<::xrn::engine::component::Control>(entity);
        registry.get<::xrn::engine::component::Control>(entity).setSpeed(2500);
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Cube"));
        registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, -mapSize.z);
        registry.emplace<::xrn::engine::component::Scale>(entity, ::game::Map::playerScale);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // enemy
        auto entity{ enemy };
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Cube"));
        registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, mapSize.z);
        registry.emplace<::xrn::engine::component::Scale>(entity, ::game::Map::playerScale);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 0.0f });
    }

    { // ball
        auto entity{ ball };
        registry.emplace<::xrn::engine::component::Control>(entity);
        registry.emplace<::xrn::engine::component::PointLight>(entity, glm::vec3{ 1.0f, 1.0f, 1.0f });
        registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, 0.0f, 0.0f);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -90.0f, 0.0f, 0.0f });
    }
}

///////////////////////////////////////////////////////////////////////////
void ::game::Map::loadMap(
    ::entt::registry& registry
    , ::xrn::engine::vulkan::Device* device
)
{
    // bot
    {
        auto entity{ registry.create() };
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Floor"));
        registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, mapSize.y, 0.0f);
        registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
    }
    // top
    {
        auto entity{ registry.create() };
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Floor"));
        registry.emplace<::xrn::engine::component::Position>(entity, 0.0f, -mapSize.y, 0.0f);
        registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.x, mapSize.y, mapSize.z);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ -180.0f, 0.0f, 0.0f });
    }
    // left
    {
        auto entity{ registry.create() };
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Floor"));
        registry.emplace<::xrn::engine::component::Position>(entity, -mapSize.x, 0.0f, 0.0f);
        registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.x, mapSize.z);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, 90.0f });
    }
    // right
    {
        auto entity{ registry.create() };
        registry.emplace<::xrn::engine::component::Transform3d>(entity, ::xrn::engine::vulkan::Model::createFromFile(device, "Floor"));
        registry.emplace<::xrn::engine::component::Position>(entity, mapSize.x, 0.0f, 0.0f);
        registry.emplace<::xrn::engine::component::Scale>(entity, mapSize.y, mapSize.x, mapSize.z);
        registry.emplace<::xrn::engine::component::Rotation>(entity, ::glm::vec3{ 0.0f, 0.0f, -90.0f });
    }
}
