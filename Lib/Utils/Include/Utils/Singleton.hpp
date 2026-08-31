#pragma once

#include <Utils/Assert.hpp>

namespace MK::Utils
{
   template<typename T>
   class ISingleton
   {
   public:
      ISingleton( const ISingleton & )     = delete;
      void operator=( const ISingleton & ) = delete;

   public:
      [[nodiscard]] static T &Get()
      {
         static T s_Instance{};
         return s_Instance;
      }

   protected:
      ISingleton() = default;
      ~ISingleton() = default;
   };
}
