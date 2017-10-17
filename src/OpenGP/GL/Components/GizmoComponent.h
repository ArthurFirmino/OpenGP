// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <OpenGP/GL/Entity.h>
#include <OpenGP/GL/Components/TransformComponent.h>
#include <OpenGP/GL/Components/GUICanvasComponent.h>


//=============================================================================
namespace OpenGP {
//=============================================================================

class GizmoComponent : public Component {
private:

    Mat4x4 matrix;

public:

    ImGuizmo::OPERATION operation = ImGuizmo::ROTATE;

    void init() {
        auto &transform = require<TransformComponent>();
        transform.scale = Vec3(0.1, 0.1, 0.2);
    }

    void update() {
        if (!ImGuizmo::IsUsing()) {
            matrix = get<TransformComponent>().get_matrix();
        }
    }

    void on_gui(const GUIElementDrawEvent &event) {

        auto &transform = get<TransformComponent>();

        Mat4x4 proj = event.camera.get_projection();
        Mat4x4 view = event.camera.get_view();
        Mat4x4 model = transform.get_matrix();
        Mat4x4 dmodel = Mat4x4::Zero();

        ImGuizmo::Manipulate(view.data(), proj.data(), operation, ImGuizmo::LOCAL, model.data(), dmodel.data());

        switch(operation) {
            case ImGuizmo::TRANSLATE: transform.set_translation_matrix(dmodel * model); break;
            case ImGuizmo::ROTATE:    transform.set_rotation_matrix(dmodel * model); break;
            case ImGuizmo::SCALE:     transform.set_scale_matrix(dmodel * model); break;
        }


    }

};

//=============================================================================
} // OpenGP::
//=============================================================================
