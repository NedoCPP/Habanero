#ifndef NDVEC_H
#define NDVEC_H
#include<iostream>
#include<tuple>

namespace hbnr {

    template<typename T>
     struct allocator{
\
    template<typename _SizeAlloc>
     T* allocate(_SizeAlloc N){
        if(N<0)
            throw std::bad_alloc();
    auto ptr= static_cast<T*>(calloc(N,sizeof(T)));
        if(ptr)
            return ptr;
        else
            throw std::bad_alloc();
     }


     void destroy(T* ptr);
     void deallocate(T* ptr);
     allocator()=default;


      void construct(T* ptr,const T& value){
          new(ptr)T(value);
      }

     };
//construct overload version with (T*, initializer_list)
    template<typename T>
     void allocator<T>::destroy(T* ptr){
        ptr->~T();
     }

    template<typename T>
     void allocator<T>::deallocate(T* ptr){
         free(ptr);
     }
}
#endif // NDVEC_H
