#ifndef _VECTOR__H
#define _VECTOR__H
#include "hbnr_alloc.h"






namespace hbnr {
    template<typename T,typename _Alloc=allocator<T>>
     class vector{
public:
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef T* const const_pointer;
        size_t curSize=0;
        size_t Capasity=0;
        std::ptrdiff_t difference_type;
        pointer row_ptr;
        _Alloc al;
///////////////////////////////////////////////////
        size_t size(){
            return this->curSize;
        }
////////////////////////////////////////////////////
//1) default constructor
        vector()=default;
////////////////////////////////////////////////////
//3) Constructor, _VecSize instances of T
        template<typename Size>//2)
         vector(Size VecSize){
            row_ptr=al.allocate(VecSize);
             curSize=VecSize;
              Capasity=VecSize;
         }
///////////////////////////////////////////////////
//2) Constructor,  _VecSize instances of T, each is _ValForEach
        template<typename Size>
         vector(Size VecSize,T ValForEach){
             row_ptr=al.allocate(VecSize);
              curSize=VecSize;
               Capasity=VecSize;

              for(size_t i=0;i<curSize;i++){
                    al.construct(row_ptr+i,ValForEach);
                }
         }
///////////////////////////////////////////////////
//8)initializer list constructor
        vector(std::initializer_list<T> list){
            row_ptr=al.allocate(list.size());
             curSize=list.size();
              Capasity=list.size();

             for(size_t i=0;i<list.size();i++){
               al.construct((row_ptr+i),*(list.begin()+i));
              }
        }
////////////////////////////////////////////////////
//5)copy constructor
        vector(vector<T>& AnotherVec){
            this->curSize=AnotherVec.curSize;
             this->Capasity=curSize;
              row_ptr=al.allocate(AnotherVec.curSize);

             for(size_t i=0;i<AnotherVec.curSize;i++){
                 al.construct((this->row_ptr+i),
                  *(AnotherVec.row_ptr+i));
                }
        }
////////////////////////////////////////////////////
// move constructor
////////////////////////////////////////////////////
        void operator =(hbnr::vector<T>& AnotherVec){
           if(this->curSize()!=0){
               for(size_t i=0;i<this->curSize;i++){
                   al.destroy((this->row_ptr+i));
               }al.deallocate(this->row_ptr);
           }
            row_ptr=al.allocate(AnotherVec.curSize);

           for(size_t i=0;i<AnotherVec.curSize;i++){
                al.construct((this->row_ptr+i),
                                    *(AnotherVec.row_ptr+i));
                 this->curSize=AnotherVec.curSize;
                  this->_Capasite=AnotherVec.curSize;
             }
        }
////////////////////////////////////////////////////
    //assign overloading for vec.assign(Size,Values);
        template<typename Size>
         void assign(Size NewSize,T ValForEach){
            for(size_t i=0;i<curSize;i++){
                al.destroy(row_ptr+i);
            }
             al.deallocate(row_ptr);
              row_ptr=al.allocate(NewSize);
               this->curSize=NewSize;

               for(int i=0;i<NewSize;i++){
                   al.construct((row_ptr+i),ValForEach);
               }
         }
////////////////////////////////////////////////////
// assign overloading for vec.assign(initializer_list)
        void assign(std::initializer_list<T> list){
            for(size_t i=0;i<curSize;i++){
                al.destroy(row_ptr+i);
            }
             al.deallocate(row_ptr);
              row_ptr=al.allocate(list.size());
               this->curSize=list.size();
                this->Capasity=curSize;

              for(size_t i=0;i<list.size();i++){
                  al.construct((row_ptr+i),*(list.begin()+i));
              }
        }
////////////////////////////////////////////////////
    //get_allocator
        _Alloc get_allocator(){
            return al;
        }
////////////////////////////////////////////////////
         reference operator[](int value){
           if(value<0||value>=(int)this->Capasity)
               throw std::out_of_range("NE");
            return *(row_ptr+value);
         }
///////////////////////////////////////////////////
    //data
        pointer data(){
            return row_ptr;
        }

///////////////////////////////////////////////////
        pointer begin(){
            return row_ptr;
        }

///////////////////////////////////////////////////
        pointer end(){
            return (row_ptr+curSize);
        }
///////////////////////////////////////////////////
        bool empty(){
            return (curSize==0);
        }
///////////////////////////////////////////////////
 // size max
        size_t max_size(){
            return (SIZE_MAX/sizeof(T));
        }


///////////////////////////////////////////////////
    //reserve
        template<typename NewCapacity>
         void reserve(NewCapacity newCap){
            if(newCap<0)
                throw std::length_error("vec reserve");
            if((size_t)newCap<curSize)
                return;

            auto ptr=row_ptr;
             row_ptr=al.allocate(newCap);

             for(size_t i=0;i<curSize;i++){
                al.construct((row_ptr+i),*(ptr+i));
                 al.destroy(ptr+i);
             } al.deallocate(ptr);
            this->Capasity=newCap;
         }

///////////////////////////////////////////////////
    //capacity
        size_t capacity(){
            return Capasity;
        }

//////////////////////////////////////////////////
    //shrink_to_fit
        void shrink_to_fit(){
            if(Capasity==curSize)
                return;
            auto ptr=row_ptr;
             row_ptr=al.allocate(curSize);

             for(size_t i=0;i<curSize;i++){
                al.construct((row_ptr+i),*(ptr+i));
                 al.destroy(ptr+i);
             } al.deallocate(ptr);
            this->Capasity=curSize;
        }

//////////////////////////////////////////////////
    //clear
        void clear(){
            for(size_t i=0;i<curSize;i++){
               al.destroy(row_ptr+i);
            }
           curSize=0;
        }
//////////////////////////////////////////////////
    //insert
        pointer insert(pointer pos, const T& value){
            auto ptr=al.allocate(curSize*2);
            Capasity=curSize*2;
            curSize+=1;
            size_t i=0;

            for(i=0;(row_ptr+i)<pos;i++){
                al.construct(ptr+i,*(row_ptr+i));
                al.destroy(row_ptr+i);
            }
            al.construct(ptr+i,value);
            i++;

            for(;i<curSize;i++){
                al.construct(ptr+i,*(row_ptr+i-1));
                al.destroy(row_ptr+i-1);
            }   al.deallocate(row_ptr);

            row_ptr=ptr;
           return pos-1;
        }

/////////////////////////////////////////////////
    //push  back
        pointer push_back(T&& value){
            if(Capasity==0){
               row_ptr=al.allocate(3);
                Capasity=3;
            }
       else if(Capasity>0&&Capasity==curSize){
                this->reserve(curSize*3);
                    Capasity=curSize*3;
            };

            al.construct((row_ptr+curSize),value);
             curSize++;
           return (row_ptr+curSize-1);
        }
////////////////////////////////////////////////
    //pop_back
        void pop_back(){
            al.destroy(row_ptr+curSize-1);
            curSize--;
        }
/////////////////////////////////////////////////
        void resize(size_t count,const value_type& value){
          reserve(count);
            for(size_t i=curSize;i<Capasity;i++){
               row_ptr[i]=value;
            }
            curSize=count;
        }
////////////////////////////////////////////////
    //swap
////////////////////////////////////////////////////
    //operator ==
///////////////////////////////////////////////////
    //operator !=
/////////////////////////////////////////////////////
    //operator <
//////////////////////////////////////////////////////
    //operator <=
//////////////////////////////////////////////////////
    //operator >
//////////////////////////////////////////////////////
    //operator >=
//////////////////////////////////////////////////////

     };


}

#endif // _VECTOR__H
