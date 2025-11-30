#pragma once

#include "glm/glm.hpp"
#include "Ciallo/Core/UUID.h"
#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Ciallo/Renderer/Texture.h"

namespace Ciallo
{
    struct TransformComponent {
       
        glm::vec3 Translation{ 0.0f,0.0f,0.0f };
        glm::vec3 Rotation{ 0.0f,0.0f,0.0f };
        glm::vec3 Scale{ 1.0f,1.0f,1.0f };

        glm::mat4 GetTransform()
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));

            return glm::translate(glm::mat4(1.0f), Translation)*rotation*glm::scale(glm::mat4(1.0f), Scale);
        }

        TransformComponent() = default;
        TransformComponent(const glm::vec3& translation) : Translation(translation) {}
        TransformComponent(const TransformComponent&) = default;
    };

    struct UUIDComponent
    {
        UUID ID;

        UUIDComponent() = default;
        UUIDComponent(uint64_t id) : ID(id) {}
        UUIDComponent(const UUIDComponent&) = default;
    };

    struct TagComponent {
        std::string Tag{ "Default" };
        TagComponent() = default;
        TagComponent(std::string tag) : Tag(std::move(tag)) {}
        TagComponent(const TagComponent&) = default;
    };

    struct SpriteComponent {
        glm::vec4 Color{ 1.0f };
        Ref<Texture2D> texture;
        float TillingFactor = 1.0f;
        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color) : Color(color) {}
        SpriteComponent(const SpriteComponent&) = default;
    };

    struct CircleSpriteComponent {
        glm::vec4 Color{ 1.0f };
        float Radius = 0.5f;
        float Thickness = 1.0f;
        float fade = 0.005f;

        CircleSpriteComponent() = default;
        CircleSpriteComponent(const glm::vec4& color) : Color(color) {}
    };

    struct CameraComponent {
        SceneCamera camera{};
        bool Primary{ true };
        bool FixedAspectRadio=false;
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    class ScriptEntity;
    struct NativeScriptComponent
    {
        ScriptEntity* Instance=nullptr;

        ScriptEntity* (*InstanciateScript)();
        void(*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstanciateScript = []() {return static_cast<ScriptEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };

        }

        NativeScriptComponent() = default;
        NativeScriptComponent(const NativeScriptComponent&) = default;
    };

    struct Rigidbody2DComponent
    {
        enum class BodyType {Static=0,Dynamic,Kinematic};
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        void* RuntimeBody = nullptr;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 offset = {0.0f,0.0f};
        glm::vec2 size = { 0.5f,0.5f };

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* Fixture = nullptr;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };

    struct CircleCollider2DComponent
    {
        glm::vec2 offset = { 0.0f,0.0f };
        float size = 0.5f;

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* Fixture = nullptr;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
    };

}