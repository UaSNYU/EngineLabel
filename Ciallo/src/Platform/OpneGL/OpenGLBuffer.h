#pragma once


#include "Ciallo/Renderer/Buffer.h"


namespace Ciallo
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        virtual void bind()const;
        virtual void Unbind()const;

        virtual const BufferLayout& GetLayout()const override { return m_layout; }
        virtual void  SetLayout(const BufferLayout& layout)override{m_layout = layout;};

    private:
        uint32_t m_RendererID;
        BufferLayout m_layout;
    };


    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();

        virtual void bind()const;
        virtual void Unbind()const;
        virtual uint32_t GetCount()const { return m_count; };
    private:
        uint32_t m_RendererID;
        uint32_t m_count;
    };
}