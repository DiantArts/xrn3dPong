///////////////////////////////////////////////////////////////////////////
// Precompilled headers
///////////////////////////////////////////////////////////////////////////
#include <pch.hpp>

///////////////////////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////////////////////
#include <xrn/Engine/System/UpdateTransform3d.hpp>



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Basic
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void ::xrn::engine::system::UpdateTransform3d::operator()(
    ::xrn::engine::vulkan::FrameInfo& frameInfo
    , ::xrn::engine::component::Transform3d& transform
    , ::xrn::OptRef<::xrn::engine::component::Position> position
    , ::xrn::OptRef<::xrn::engine::component::Rotation> rotation
    , ::xrn::OptRef<::xrn::engine::component::Scale> scale
) const
{
    if (position) {
        if (rotation) {
            if (scale) {
                if (position->isChanged() || rotation->isChanged() || scale->isChanged()) {
                    transform.updateMatrix(*position, *rotation, *scale);
                    transform.updateNormalMatrix(*rotation, *scale);
                }
                scale->resetChangedFlag();
                rotation->resetChangedFlag();
            } else if (position->isChanged() || rotation->isChanged()) {
                transform.updateMatrix(*position, *rotation);
                transform.updateNormalMatrix(*rotation);
                rotation->resetChangedFlag();
            }
        } else {
            if (scale) {
                if (position->isChanged() || scale->isChanged()) {
                    transform.updateMatrix(*position);
                    transform.updateMatrix(*position, { 0.0f, 0.0f, 0.0f }, *scale);
                    transform.updateNormalMatrix(*scale);
                    scale->resetChangedFlag();
                }
            } else if (position->isChanged()) {
                transform.updateMatrix(*position);
            }
        }
        position->resetChangedFlag();
    }
}
