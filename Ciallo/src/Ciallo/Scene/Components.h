#pragma once

#include "glm/glm.hpp"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Ciallo
{
    struct TransformComponent {
        glm::mat4 Transform{ 1.0f };
        TransformComponent() = default;
        TransformComponent(const glm::mat4& transform) : Transform(transform) {}
        TransformComponent(TransformComponent&&)  = default;
        operator glm::mat4() const { return Transform; }
    };

    struct TagComponent {
        std::string Tag{ "Default" };
        TagComponent() = default;
        TagComponent(std::string tag) : Tag(std::move(tag)) {}
        TagComponent(TagComponent&&)  = default;
    };

    struct SpriteComponent {
        glm::vec4 Color{ 1.0f };
        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color) : Color(color) {}
        SpriteComponent(SpriteComponent&&)  = default;
    };

    struct CameraComponent {
        SceneCamera camera{};
        bool Primary{ true };
        bool FixedAspectRadio=false;
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) { std::cout << "copy ctor" << std::endl; }
        CameraComponent(CameraComponent&&)  { std::cout << "move ctor" << std::endl; }
    };

}