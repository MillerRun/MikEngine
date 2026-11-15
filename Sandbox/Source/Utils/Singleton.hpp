#pragma once

#include <type_traits>

namespace MK::Utils
{
   template<typename T>
   class ISingleton
   {
   public:
      ISingleton( const ISingleton & )     = delete;
      void operator=( const ISingleton & ) = delete;

   public:
      [[nodiscard]] static T *Get()
      {
         static T s_Instance{};
         return std::addressof( s_Instance ); // idk why, guess I just like how looks ::Get()->Method()
      }

   protected:
      ISingleton() = default;
      ~ISingleton() = default;
   };
}
