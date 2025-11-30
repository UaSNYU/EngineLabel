
#include "clpch.h"
#include "SceneSerializer.h"
#include "yaml-cpp/yaml.h"
#include "Components.h"
#include "Entity.h"

namespace YAML 
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();

			return true;
		}
	};
}

namespace Ciallo 
{

	static std::string RigidTypeToString(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Ciallo::Rigidbody2DComponent::BodyType::Static:
			return "Static";
		case Ciallo::Rigidbody2DComponent::BodyType::Dynamic:
			return "Dynamic";
		case Ciallo::Rigidbody2DComponent::BodyType::Kinematic:
			return "Kinematic";
	
		}
	}

	static Rigidbody2DComponent::BodyType StringToRigidType(const std::string& type)
	{
		if (type == "Static")return Rigidbody2DComponent::BodyType::Static;
		if (type == "Dynamic")return Rigidbody2DComponent::BodyType::Dynamic;
		if (type == "Kinematic")return Rigidbody2DComponent::BodyType::Kinematic;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out<<YAML::Flow;
		out<<YAML::BeginSeq<<v.x<<v.y<<v.z<<YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z <<v.w<< YAML::EndSeq;
		return out;
	}

	Ciallo::SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& emitter, Entity& entity)
	{
		HZ_CORE_ASSERT(entity.HasComponent<UUID>(),"Entity DONT have id!!");

		emitter <<YAML::BeginMap;
		emitter << YAML::Key << "Entity" << YAML::Value << entity.GetID();

		if(entity.HasComponent<TagComponent>())
		{
			emitter <<YAML::Key << "TagComponent";
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			emitter << YAML::BeginMap;
			emitter<<YAML::Key << "Tag" << YAML::Value << tag;
			emitter << YAML::EndMap;
		}


		if (entity.HasComponent<TransformComponent>())
		{
			emitter << YAML::Key << "TransformComponent";
			emitter << YAML::BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			emitter << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			emitter << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			emitter << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			emitter << YAML::EndMap;
		}


		if (entity.HasComponent<SpriteComponent>())
		{
			emitter << YAML::Key << "SpriteComponent";
			emitter << YAML::BeginMap;
			auto& Sprite = entity.GetComponent<SpriteComponent>();
			emitter << YAML::Key << "Color" << YAML::Value << Sprite.Color;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CircleSpriteComponent>())
		{
			emitter << YAML::Key << "CircleSpriteComponent";
			emitter << YAML::BeginMap;
			auto& Sprite = entity.GetComponent<CircleSpriteComponent>();
			emitter << YAML::Key << "Color" << YAML::Value << Sprite.Color;
			emitter << YAML::Key << "Radius" << YAML::Value << Sprite.Radius;
			emitter << YAML::Key << "Thickness" << YAML::Value << Sprite.Thickness;
			emitter << YAML::Key << "fade" << YAML::Value << Sprite.fade;
			emitter << YAML::EndMap;
		}


		if (entity.HasComponent<CameraComponent>())
		{
			emitter << YAML::Key << "CameraComponent";
			emitter << YAML::BeginMap;
			auto& CameraComp = entity.GetComponent<CameraComponent>();
			auto& Camera = CameraComp.camera;
			
			emitter << YAML::Key << "Camera" << YAML::Value;
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "CameraType" << YAML::Value<<(int)Camera.GetCameraType();
			emitter << YAML::Key << "OrthographicCameraSize" << YAML::Value << Camera.GetOrthographicCameraSize();
			emitter << YAML::Key << "OrthographicNearClip" << YAML::Value << Camera.GetOrthographicNearClip();
			emitter << YAML::Key << "OrthographicFarClip" << YAML::Value << Camera.GetOrthographicFarClip();
			emitter << YAML::Key << "PerpectiveCameraFov" << YAML::Value << Camera.GetPerpectiveCameraFov();
			emitter << YAML::Key << "PerpectiveNearClip" << YAML::Value << Camera.GetPerpectiveNearClip();
			emitter << YAML::Key << "PerpectiveFarClip" << YAML::Value << Camera.GetPerpectiveFarClip();
			emitter << YAML::EndMap;
            emitter << YAML::Key << "Primary" << YAML::Value << CameraComp.Primary;
			emitter<< YAML::Key << "FixedAspectRadio" << YAML::Value << CameraComp.FixedAspectRadio;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			emitter << YAML::Key << "Rigidbody2DComponent";
			emitter << YAML::BeginMap;
			auto& RigidBody = entity.GetComponent<Rigidbody2DComponent>();
			emitter << YAML::Key << "Type" << YAML::Value << RigidTypeToString(RigidBody.Type);
			emitter << YAML::Key << "FixedRotation" << YAML::Value <<RigidBody.FixedRotation;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			emitter << YAML::Key << "BoxCollider2DComponent";
			emitter << YAML::BeginMap;
			auto& BodyCollider = entity.GetComponent<BoxCollider2DComponent>();
			emitter << YAML::Key << "Offset" << YAML::Value <<BodyCollider.offset ;
			emitter << YAML::Key << "Size" << YAML::Value << BodyCollider.size;
			emitter << YAML::Key << "Density" << YAML::Value << BodyCollider.Density;
			emitter << YAML::Key << "Friction" << YAML::Value << BodyCollider.Friction;
			emitter << YAML::Key << "Restitution" << YAML::Value << BodyCollider.Restitution;
			emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << BodyCollider.RestitutionThreshold;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			emitter << YAML::Key << "CircleCollider2DComponent";
			emitter << YAML::BeginMap;
			auto& CircleCollider = entity.GetComponent<CircleCollider2DComponent>();
			emitter << YAML::Key << "Offset" << YAML::Value << CircleCollider.offset;
			emitter << YAML::Key << "Size" << YAML::Value << CircleCollider.size;
			emitter << YAML::Key << "Density" << YAML::Value << CircleCollider.Density;
			emitter << YAML::Key << "Friction" << YAML::Value << CircleCollider.Friction;
			emitter << YAML::Key << "Restitution" << YAML::Value << CircleCollider.Restitution;
			emitter << YAML::Key << "RestitutionThreshold" << YAML::Value << CircleCollider.RestitutionThreshold;
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;

	}
	 
	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out<<YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key <<"Entities"<<YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityId) 
			{
				Entity entity{ entityId, m_Scene.get() };
				if (!entity) return;

				SerializeEntity(out, entity);
			});


		out<<YAML::EndSeq;
        out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout<<out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data=YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			return false;
		}

		std::string SceneName = data["Scene"].as<std::string>();
		HZ_CORE_TRACE("Deserializing scene {0}", SceneName);

		auto& Entities=data["Entities"];
		if (Entities)
		{
			for (auto entity: Entities)
			{
				//TODO---CHANGE TO UUID
				uint32_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto& tagcomp = entity["TagComponent"];
				if (tagcomp)
				{
					name = tagcomp["Tag"].as<std::string>();
				}
				
				HZ_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid,name);

                auto& transformcomp = entity["TransformComponent"];
				if (transformcomp)
				{
					auto& tc=deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformcomp["Translation"].as<glm::vec3>();
					tc.Rotation = transformcomp["Rotation"].as<glm::vec3>();
					tc.Scale = transformcomp["Scale"].as<glm::vec3>();
				}

				auto& spritecomp = entity["SpriteComponent"];
				if (spritecomp)
				{
					deserializedEntity.AddComponent<SpriteComponent>(spritecomp["Color"].as<glm::vec4>());
				}

				auto& circlespritecomp = entity["CircleSpriteComponent"];
				if (circlespritecomp)
				{
					auto& circle = deserializedEntity.AddComponent<CircleSpriteComponent>();
					circle.Color = circlespritecomp["Color"].as<glm::vec4>();
					circle.Radius = circlespritecomp["Radius"].as<float>();
					circle.Thickness = circlespritecomp["Thickness"].as<float>();
					circle.fade= circlespritecomp["fade"].as<float>();
				}

				auto& cameraComp = entity["CameraComponent"];
				if (cameraComp)
				{ 
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProps=cameraComp["Camera"];

					cc.camera.SetCameraType((CameraType)cameraProps["CameraType"].as<int>());
					cc.camera.SetOrthographicCameraSize(cameraProps["OrthographicCameraSize"].as<float>());
                    cc.camera.SetOrthographicFarClip(cameraProps["OrthographicFarClip"].as<float>());
                    cc.camera.SetOrthographicNearClip(cameraProps["OrthographicNearClip"].as<float>());

					cc.camera.SetPerpectiveCameraFov(cameraProps["PerpectiveCameraFov"].as<float>());
                    cc.camera.SetPerpectiveFarClip(cameraProps["PerpectiveFarClip"].as<float>());
                    cc.camera.SetPerpectiveNearClip(cameraProps["PerpectiveNearClip"].as<float>());

					cc.FixedAspectRadio=cameraComp["FixedAspectRadio"].as<bool>();
					cc.Primary=cameraComp["Primary"].as<bool>();
				}

				auto& rigidcomp = entity["Rigidbody2DComponent"];
				if (rigidcomp)
				{
					auto& rigid = deserializedEntity.AddComponent<Rigidbody2DComponent>();

					rigid.Type = StringToRigidType(rigidcomp["Type"].as<std::string>());
					rigid.FixedRotation = rigidcomp["FixedRotation"].as<bool>();
				}

				auto& bodycollidercomp = entity["BoxCollider2DComponent"];
				if (bodycollidercomp)
				{
					auto& bodycollider = deserializedEntity.AddComponent<BoxCollider2DComponent>();

					bodycollider.offset = bodycollidercomp["Offset"].as<glm::vec2>();
					bodycollider.size = bodycollidercomp["Size"].as<glm::vec2>();

					bodycollider.Density = bodycollidercomp["Density"].as<float>();
					bodycollider.Friction = bodycollidercomp["Friction"].as<float>();
					bodycollider.Restitution = bodycollidercomp["Restitution"].as<float>();
					bodycollider.RestitutionThreshold = bodycollidercomp["RestitutionThreshold"].as<float>();
					
				}

				auto& circlecollidercomp = entity["CircleCollider2DComponent"];
				if (circlecollidercomp)
				{
					auto& circlecollider = deserializedEntity.AddComponent<CircleCollider2DComponent>();

					circlecollider.offset = circlecollidercomp["Offset"].as<glm::vec2>();
					circlecollider.size = circlecollidercomp["Size"].as<float>();

					circlecollider.Density = circlecollidercomp["Density"].as<float>();
					circlecollider.Friction = circlecollidercomp["Friction"].as<float>();
					circlecollider.Restitution = circlecollidercomp["Restitution"].as<float>();
					circlecollider.RestitutionThreshold = circlecollidercomp["RestitutionThreshold"].as<float>();

				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}

}
