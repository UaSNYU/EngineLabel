#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Ciallo/Scene/Camera.h"
#include "Ciallo/Scene/EditorCamera.h"
#include "Ciallo/Scene/Components.h"

namespace Ciallo
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void EndScene();
		static void Flush();

		static void SetLineWidth(float width);
		static float GetLineWidth();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void StartBatch();
		//primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f });

		static void DrawRotateQuad(const glm::vec2& position,float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position,float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f , glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f});
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f});
		static void DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f });
		static void DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f });


		static void DrawQuad(const glm::mat4& tranform, const glm::vec4& color,int entity_id=-1);
		static void DrawQuad(const glm::mat4& tranform, const Ref<Texture2D>& texture, int entity_id = -1, float TilingFactor = 1.0f, glm::vec4 TilingColor = { 1.0f,1.0f, 1.0f, 1.0f });
		static void DrawCircle (const glm::mat4& tranform, const glm::vec4& color, const float Thickness=1.0f, float fade=0.0005f, int entity_id = -1);
		static void DrawLines(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec4& color, int entity_id = -1);
		static void DrawRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entity_id = -1);
		static void DrawRect(const glm::mat4& tranform, const glm::vec4& color, int entity_id = -1);


		static void DrawSprite(const glm::mat4& tranform, const SpriteComponent& sprite,int entity_id);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics& GetStatistics();
	private:
		static void FlushAndReset();
	};
}