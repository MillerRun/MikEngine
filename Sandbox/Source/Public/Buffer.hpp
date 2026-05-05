#pragma once

#include <glad/glad.h> 

enum class EBufferType : int
{
   VERTEX,
   INDEX,
};

template<EBufferType BufferType, typename TDataType>
class Buffer
{
public:
   explicit Buffer( const TDataType *a_pData, GLsizeiptr a_iDataSize, bool a_bDynamic = false )
   {
      glGenBuffers( 1, &m_iID );
      glBindBuffer( GetBufferTarget(), m_iID );
      glBufferData( GetBufferTarget(), a_iDataSize, a_pData, a_bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
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
   glBindBuffer( GetBufferTarget(), m_iID );
}

template<EBufferType BufferType, typename TDataType>
void Buffer<BufferType, TDataType>::Unbind()
{
   glBindBuffer( GetBufferTarget(), 0 );
}

template<EBufferType BufferType, typename TDataType>
void Buffer<BufferType, TDataType>::Delete()
{
   glDeleteBuffers( 1, &m_iID );
}

template<EBufferType BufferType, typename TDataType>
GLuint Buffer<BufferType, TDataType>::GetID() const
{
   return m_iID;
}
