#pragma once

#include <xrn/Engine/AScene.hpp>
#include <Game/MessageType.hpp>

namespace game::client {

///////////////////////////////////////////////////////////////////////////
/// \brief Scene of the came
/// \ingroup game
///
/// \include Scene.hpp <Game/Scene.hpp>
///
///////////////////////////////////////////////////////////////////////////
class Scene
    : public ::xrn::engine::AScene
    , public ::xrn::network::client::Client<::game::MessageType>
{

public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Static
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    using Message = ::xrn::network::Message<::game::MessageType>;



public:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \warning Call to connectToServer() is required to run the scene
    ///
    /// \see connectToServer()
    ///
    ///////////////////////////////////////////////////////////////////////////
    explicit Scene();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief loads the scene
    ///
    ///////////////////////////////////////////////////////////////////////////
    void loadScene();



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Rule of 5
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    ~Scene();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    Scene(
        const Scene&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Copy assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        const Scene&
    ) noexcept
        -> Scene& = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ///////////////////////////////////////////////////////////////////////////
    Scene(
        Scene&&
    ) noexcept = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Move assign operator
    ///
    ///////////////////////////////////////////////////////////////////////////
    auto operator=(
        Scene&&
    ) noexcept
        -> Scene& = delete;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Events
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// if return false, scene exits
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto onUpdate()
        -> bool override;

    ///////////////////////////////////////////////////////////////////////////
    /// if return false, scene exits
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto onPostUpdate()
        -> bool override;

    ///////////////////////////////////////////////////////////////////////////
    /// called with the m_tickFrequencyTime per seconds
    ///////////////////////////////////////////////////////////////////////////
    [[ nodiscard ]] auto onTick(
        ::xrn::Time deltaTime
    ) -> bool override;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    void onKeyPressed(
        ::std::int16_t keyCode
    ) override;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    void onKeyReleased(
        ::std::int16_t keyCode
    ) override;

    ///////////////////////////////////////////////////////////////////////////
    /// Automatically called when handling events
    /// Called if camera is not detached
    ///////////////////////////////////////////////////////////////////////////
    void onMouseMoved(
        ::glm::vec2 position
    ) override;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief On message received from the server
    ///
    ///////////////////////////////////////////////////////////////////////////
    void onReceive(
        Scene::Message& message
        , ::std::shared_ptr<::xrn::network::Connection<::game::MessageType>> connection
    ) override;



    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Queue
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Wait for the server to find a gamme
    ///
    ///////////////////////////////////////////////////////////////////////////
    void queueForGame();




private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Helpers
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void createBall();

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void resetBalls();

    ///////////////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////////////
    void moveBotCheat();



private:

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Member variables
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ::xrn::Id m_playerNumber{ 0 };

    ::entt::entity m_enemy;
    ::std::vector<::entt::entity> m_balls;

    ::sf::Music m_music;
    sf::SoundBuffer m_soundBuffer;
    ::sf::Sound m_sound;
    ::std::array<::std::string, 2> m_soundFilenames{
        "ballBounceOffWall.ogg"
        , "ballToPlayer.ogg"
    };

    bool m_clearBalls{ false };

};

} // namespace game::client
