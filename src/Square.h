#ifndef SQUARE_HEADER_DEFINED
#define SQUARE_HEADER_DEFINED

#include <type_traits>

template <typename T>
struct square
{
  //Member variables
  T x;
  T y;
  
  //Constructors
  square<T>() {}
  square<T>(const T x, const T y) : x(x), y(y) {}
  
  template<typename newT>
  square<T>(const square<newT>& other) : x(other.x), y(other.y) {}
  
  //Addition operators
  template<typename newT>
  square<typename std::common_type<T,newT>::type> operator+(const square<newT> &other) const {return square<typename std::common_type<T,newT>::type>(x+other.x,y+other.y);}
  square<T>& operator+=(const square<T> &other) {x+=other.x; y+=other.y; return *this;}
  
  //Forward declaration as square<void> is not yet defined.
  square<T> operator+(const square<void> &other) const;
  square<T>& operator+=(const square<void> &other);
};

template<>
struct square<void>
{
  //Member variables
  signed char x:4;
  signed char y:4;
  
  //Constructors
  square<void>() {}
  square<void>(const int x, const int y) : x(x), y(y) {}
  template<typename newT>
  square<void>(const square<newT>& other) : x(other.x), y(other.y) {}

  //Addition operators
  template<typename newT>
  square<newT> operator+(const square<newT>& other) {return square<newT>(x+other.x,y+other.y);}
  template<typename newT>
  square<void>& operator+=(const square<newT>& other) {x += other.x; y += other.y; return *this;}
};

template<typename T>
square<T> square<T>::operator+(const square<void> &other) const {return square<T>(x + other.x,y + other.y);}
template<typename T>
square<T>& square<T>::operator+=(const square<void> &other) {x += other.x; y+= other.y; return *this;}

#endif
