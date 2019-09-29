/*!
  @brief box.std::unique_ptr wrapper
  @file nodamushi/box.hpp
*/
/*
 * These codes are licensed under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef NODAMUSHI_BOX_HPP
#define NODAMUSHI_BOX_HPP

# include <type_traits>
# include <memory>
namespace nodamushi{

/**
 * unique_ptr wrapper.
 * get()const  and operator*()const function return const T
 */
template<typename T,typename D = std::default_delete<T>>
struct box
{
  T* get() noexcept{return ptr.get();}
  const T* get() const noexcept{return ptr.get();}

  typename std::add_lvalue_reference<typename std::add_const<T>::type>::type operator*()const{return *ptr;}
  typename std::add_lvalue_reference<T>::type operator*() {return *ptr;}
  
  const T* operator->() const noexcept{return ptr.operator->();}
  T* operator->() noexcept{return ptr.operator->();}

  constexpr box()noexcept
      :ptr(){}
  explicit box(T* p)noexcept
      :ptr(p) {}
  box(T* p, const D& d1)noexcept
      :ptr(p,d1){}
  box(T* p, D&& d2)noexcept
      :ptr(p,std::move(d2)){}
  box(std::unique_ptr<T,D>&& u)noexcept
      :ptr(std::move(u)){}
  box(box<T,D>&& u)noexcept
      :ptr(std::move(u.ptr)){}
  
  constexpr box(std::nullptr_t n)noexcept
      : ptr(n){}
  box(const box&) = delete;
  
  box& operator=(box&& u) noexcept{
    ptr=std::move(u.ptr);
    return *this;
  }
  template <class U, class E>
  box& operator=(box<U, E>&& u) noexcept{
    ptr=std::move(u.ptr);
    return *this;
  }
  box& operator=(std::nullptr_t p) noexcept{
    ptr = p;
    return *this;
  }
  box& operator=(const box&) = delete; 
  
  T* release()noexcept{return ptr.release();}
  void reset(T* p=nullptr)noexcept{ptr.reset(p);}
  template<class U> void reset(U)=delete;
  template<typename T2,typename D2>
  void swap(std::unique_ptr<T2,D2>& x) noexcept{ptr.swap(x);}
  template<typename T2,typename D2>
  void swap(box<T2,D2>& x) noexcept{ptr.swap(x.ptr);}
  

  D& get_deleter() noexcept{return ptr.get_deleter();}
  const D& get_deleter() const noexcept{return ptr.get_deleter();}
  explicit operator bool() const noexcept{return static_cast<bool>(ptr);}
  
 private:
  std::unique_ptr<T,D> ptr;
};

template<typename T>
const T& unboxing(const T& t){return t;}
template<typename T,typename D>
const T& unboxing(const box<T,D>& t){return *t;}

}// end namespace nodamushi

#endif // NODAMUSHI_BOX_HPP
