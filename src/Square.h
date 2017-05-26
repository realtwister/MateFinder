#ifndef SQUARE_HEADER_DEFINED
#define SQUARE_HEADER_DEFINED

#include <type_traits>

/**
 * The square struct stores one square on the chess board. The template parameter specifies the type in which the two coordinates are stored.
 * A void template parameter indicates that the coordinates are stored in one byte, where one nibble is dedicated to the x coordinate, and the other to the y coordinate.
 * This form of storing the coordinates is chosen to save some memory, and to reduce memory copying overhead.
 */
template <typename T>
struct square
{
  //Member variables
  T x;  //!< x coordinate.
  T y;  //!< y coordinate.
  
  //Constructors
  square<T>() {}  //!< Empty constructor.
  square<T>(const T x, const T y) : x(x), y(y) {} //!< Normal constructor.
  
  template<typename newT>
  square<T>(const square<newT>& other) : x(other.x), y(other.y) {}  //!< Copy constructor
  
  //Addition operators
  template<typename newT>
  square<typename std::common_type<T,newT>::type> operator+(const square<newT> &other) const {return square<typename std::common_type<T,newT>::type>(x+other.x,y+other.y);} //!< Addition operator
  square<T>& operator+=(const square<T> &other) {x+=other.x; y+=other.y; return *this;} //!< Addition assignment operator
  
  //Forward declaration as square<void> is not yet defined.
  square<T> operator+(const square<void> &other) const; //!< Specialized addition operator to interact with the <void> case.
  square<T>& operator+=(const square<void> &other); //!< Specialized addition assignment operator to interact with the <void> case.
};

/**
 * Specialized version of the square struct, storing both coordinates in one byte.
 */
template<>
struct square<void>
{
  //Member variables
  signed char x:4;  //!< x coordinate.
  signed char y:4;  //!< y coordinate.
  
  //Constructors
  square<void>() {} //!< Empty constructor.
  square<void>(const int x, const int y) : x(x), y(y) {}  //!< Normal constructor.
  template<typename newT>
  square<void>(const square<newT>& other) : x(other.x), y(other.y) {} //!< Copy constructor.

  //Addition operators
  template<typename newT>
  square<newT> operator+(const square<newT>& other) {return square<newT>(x+other.x,y+other.y);} //!< Additoin operator
  template<typename newT>
  square<void>& operator+=(const square<newT>& other) {x += other.x; y += other.y; return *this;} //!< Addition assignment operator
};

template<typename T>
square<T> square<T>::operator+(const square<void> &other) const {return square<T>(x + other.x,y + other.y);}
template<typename T>
square<T>& square<T>::operator+=(const square<void> &other) {x += other.x; y+= other.y; return *this;}

#endif
