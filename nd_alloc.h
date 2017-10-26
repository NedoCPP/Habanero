#ifndef NDVEC_H
#define NDVEC_H
#include<iostream>
#include<tuple>

namespace nedo {

    template<typename T>
     struct allocator{
         T* ptr;
         const T* cptr;

         T* allocate(size_t);
         void destroy();
         void deallocate();

        template<typename...Tp>
         void construct(Tp...tp){
            auto tuple=std::tuple<Tp...>(tp...);//to-do: make_tuple requires int&&. Solve
             auto obj=std::make_from_tuple<T>(tuple);
              new(ptr) T(obj);
         }

     };

    template<typename T>
     T* allocator<T>::allocate(size_t N){
        return static_cast<T*>(malloc(N*sizeof(T)));
     }

    template<typename T>
     void allocator<T>::destroy(){
        ptr->~T();
         ptr=nullptr;
     }

    template<typename T>
     void allocator<T>::deallocate(){
         free(ptr);
     }

};







#endif // NDVEC_H
