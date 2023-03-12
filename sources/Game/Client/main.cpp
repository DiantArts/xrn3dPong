///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <Game/Client/Scene.hpp>

///////////////////////////////////////////////////////////////////////////
auto main()
    -> int
{
    ::game::client::Scene scene;
    if (scene.connectToServer("127.0.0.1", 8080)) {
        scene.queueForGame(); // automatically load the scene and start the game
        scene.run();
    }
    return EXIT_SUCCESS;
}
