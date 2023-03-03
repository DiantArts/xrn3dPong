///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/Event/KeyPressed.hpp>
#include <xrn/Engine/Component/Control.hpp>
#include <xrn/Engine/Configuration.hpp>
#include <xrn/Engine/AScene.hpp>


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
::xrn::engine::event::KeyPressed::KeyPressed(
    ::std::int16_t keyCode
)
    : m_keyCode{ keyCode }
{}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Resolve
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::event::KeyPressed::resolve(
    ::xrn::engine::AScene& scene
)
{
    auto* playerController{ scene.tryGetPlayerComponent<::xrn::engine::component::Control>() };

    // default
    if (m_keyCode == ::xrn::engine::configuration.keyBinding.closeWindow) {
        return scene.getWindow().close();
    }

    // if player is controllable
    if (playerController) {
        // move
        if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->startMovingForward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->startMovingBackward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->startMovingLeft();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->startMovingRight();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveUp) {
            return playerController->startMovingUp();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveDown) {
            return playerController->startMovingDown();

        // move arrows
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->startMovingForward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->startMovingBackward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
            return playerController->startMovingLeft();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
            return playerController->startMovingRight();

        // look
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookUp) {
            return playerController->rotateZ(-45 / 2);
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookDown) {
            return playerController->rotateZ(45 / 2);
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookLeft) {
            return playerController->rotateX(45 / 2);
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookRight) {
            return playerController->rotateX(-45 / 2);
        }

    }

    XRN_INFO("Key ({}) is not bound to press", m_keyCode);
}
