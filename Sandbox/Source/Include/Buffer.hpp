#pragma once

#include <glad/glad.h>
#include <vector>

#include "Common.hpp"
#include "Vertex.hpp"

enum class EBufferType : int
{
   VERTEX,
   INDEX,
};

template<EBufferType BufferType, typename TDataType>
class Buffer
{
public:
   explicit Buffer( const std::vector<TDataType>& a_aData, bool a_bDynamic = false )
   {
      GLCHECK( glGenBuffers( 1, &m_iID ) );
      GLCHECK( glBindBuffer( GetBufferTarget(), m_iID ) );
      GLCHECK( glBufferData( GetBufferTarget(), a_aData.size() * sizeof( TDataType ), a_aData.data(), a_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
   }

   void Bind();
   void Unbind();
   void Delete();
   [[nodiscard]] GLuint GetID() const;

private:
   [[nodiscard]] static constexpr
   int GetBufferTarget()
   {
      if constexpr( BufferType == EBufferType::VERTEX )
         return GL_ARRAY_BUFFER;
      else if constexpr( BufferType == EBufferType::INDEX )
         return GL_ELEMENT_ARRAY_BUFFER;
      else
         return GL_ARRAY_BUFFER;
   }

private:
   GLuint m_iID = 0;
};

template<EBufferType BufferType, typename TDataType>
void Buffer<BufferType, TDataType>::Bind()
{
   GLCHECK( glBindBuffer( GetBufferTarget(), m_iID ) );
}

template<EBufferType BufferType, typename TDataType>
void Buffer<BufferType, TDataType>::Unbind()
{
   GLCHECK( glBindBuffer( GetBufferTarget(), 0 ) );
}

template<EBufferType BufferType, typename TDataType>
void Buffer<BufferType, TDataType>::Delete()
{
   GLCHECK( glDeleteBuffers( 1, &m_iID ) );
}

template<EBufferType BufferType, typename TDataType>
GLuint Buffer<BufferType, TDataType>::GetID() const
{
   return m_iID;
}

using VertexBuffer = Buffer<EBufferType::VERTEX, Vertex>;
using IndexBuffer = Buffer<EBufferType::INDEX, GLuint>;