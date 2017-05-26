#ifndef MOVEARRAY_HEADER_DEFINED
#define MOVEARRAY_HEADER_DEFINED

/**
 * The moveArray struct stores an array of moves.
 * Its footprint is similar to std::vector's. We decided not to go with std::vector, though, as our own implementation saved about 15% runtime.
 * The difference probably comes from the less flexible implementation we give here.
 */
struct moveArray
{
  //Member variables
  int num;      //!< The number of moves the moveArray has space for.
  int ctr;      //!< The number of elements that are currently stored in the moveArray.
  move * moves; //!< A pointer to the array of moves.
  
  //Constructors
  moveArray() : num(0), ctr(0), moves(NULL) {}                    //!< The default constructor.
  moveArray(const int n) : num(n), ctr(0), moves(new move[n]) {}  //!< A constructor reserving space for \a n moves.
  moveArray(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
  } //!< The copy constructor.
  
  //Destructor
  ~moveArray() {delete[] moves;}  //!< The destructor.
  
  //Copy assignment operator
  moveArray& operator=(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
    return *this;
  } //!< The copy assignment operator.
  
  //Move assignment operator
  moveArray& operator=(moveArray&& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = other.moves;
    other.moves = NULL;
    return *this;
  } //!< The move assignment operator.
  
  //Array dereference operator
  inline move& operator[](const int n) const {return moves[n];} //!< The array dereference operator
  
  //Public methods
  inline int size() const {return ctr;} //!< Method that returns the size of the array.
  inline void push_back(const move toAdd)
  {
    #ifdef DEBUG
    if (ctr >= num)
    {
      std::cout << "Overflow in the moveArray." << std::endl;
      return;
    }
    #endif
    moves[ctr++] = toAdd;
  } //!< Method that adds a move to the back of the array.

  inline moveArray shrink_to_fit() const
  {
    moveArray result(ctr);
    result.ctr = ctr;
    for (int i = 0; i < ctr; i++)
      result.moves[i] = moves[i];
    return result;
  } //!< This method returns a new moveArray, where the total size is shrunk such that all the information fits precisely in the occupied space.
};

#endif
