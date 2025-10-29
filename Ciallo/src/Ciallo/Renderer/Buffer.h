#pragma once

namespace Ciallo
{
	enum class ShaderDateType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t getShaderDataTypeSize(ShaderDateType type)
	{
		switch (type)
		{
		case ShaderDateType::Float:
			return 4;
		case ShaderDateType::Float2:
			return 4 * 2;
		case ShaderDateType::Float3:
			return 4 * 3;
		case ShaderDateType::Float4:
			return 4 * 4;
		case ShaderDateType::Mat3:
			return 4 * 3 * 3;
		case ShaderDateType::Mat4:
			return 4 * 4 * 4;
		case ShaderDateType::Int:
			return 4;
		case ShaderDateType::Int2:
			return 4 * 2;
		case ShaderDateType::Int3:
			return 4 * 3;
		case ShaderDateType::Int4:
			return 4 * 4;
		case ShaderDateType::Bool:
			return 1;
		}

		HZ_CORE_ERROR("UnKnown ShaderDateType!!!");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDateType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement() {}
		BufferElement(const std::string& name, ShaderDateType type) :name(name), type(type), size(getShaderDataTypeSize(type)), offset(0),normalized(false)
		{
		}

		uint32_t GetComponentCount()const
		{
			switch (type)
			{
			case ShaderDateType::Float:
				return 3;
			case ShaderDateType::Float2:
				return  2;
			case ShaderDateType::Float3:
				return  3;	
			case ShaderDateType::Float4:
				return  4;
			case ShaderDateType::Mat3:
				return  3 * 3;
			case ShaderDateType::Mat4:
				return 4 * 4 ;
			case ShaderDateType::Int:
				return 1;
			case ShaderDateType::Int2:
				return  2;
			case ShaderDateType::Int3:
				return  3;
			case ShaderDateType::Int4:
				return  4;
			case ShaderDateType::Bool:
				return 1;
			}
		}
	};

	class BufferLayout
	{
	public:


		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) :m_Elements(elements),m_Stride(0)
		{
			CaculateOffsetAndStride();
		}

		inline const uint32_t GetStride() const { return m_Stride; };
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		std::vector<BufferElement>::const_iterator begin() const{ return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end()const { return   m_Elements.end(); }
		  
	private:
		void CaculateOffsetAndStride()
		{
			uint32_t offset = 0;

			for (auto& element : m_Elements)
			{
				element.offset= offset;
				offset += element.size;
				m_Stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void bind()const = 0;
		virtual void Unbind()const = 0;
		virtual const BufferLayout& GetLayout()const = 0;
		virtual void  SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* create(float* vertices, uint32_t size);
	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void bind()const = 0;
		virtual void Unbind()const = 0;

		virtual uint32_t GetCount()const = 0;

		static IndexBuffer* create(uint32_t* vertices, uint32_t count);

	};
}