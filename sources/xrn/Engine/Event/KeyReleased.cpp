///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/Event/KeyReleased.hpp>
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
::xrn::engine::event::KeyReleased::KeyReleased(
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
void ::xrn::engine::event::KeyReleased::resolve(
    ::xrn::engine::AScene& scene
)
{
    auto* playerController{ scene.tryGetPlayerComponent<::xrn::engine::component::Control>() };

    // if player is controllable
    if (playerController) {

        // move
        if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveForward) {
            return playerController->stopMovingForward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveBackward) {
            return playerController->stopMovingBackward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveLeft) {
            return playerController->stopMovingLeft();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveRight) {
            return playerController->stopMovingRight();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveUp) {
            return playerController->stopMovingUp();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveDown) {
            return playerController->stopMovingDown();

        // move arrows
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveForward2) {
            return playerController->stopMovingForward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveBackward2) {
            return playerController->stopMovingBackward();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveLeft2) {
            return playerController->stopMovingLeft();
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.moveRight2) {
            return playerController->stopMovingRight();

        // look
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookUp) {
            return;
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookDown) {
            return;
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookLeft) {
            return;
        } else if (m_keyCode == ::xrn::engine::configuration.keyBinding.lookRight) {
            return;
        }
    }

    XRN_INFO("Key ({}) is not bound to press", m_keyCode);
}
