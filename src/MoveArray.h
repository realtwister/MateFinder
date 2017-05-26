#ifndef MOVEARRAY_HEADER_DEFINED
#define MOVEARRAY_HEADER_DEFINED

struct moveArray
{
  //Member variables
  int num;
  int ctr;
  move * moves;
  
  //Constructors
  moveArray() : num(0), ctr(0), moves(NULL) {}
  moveArray(const int n) : num(n), ctr(0), moves(new move[n]) {}
  moveArray(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
  }
  
  //Destructor
  ~moveArray() {delete[] moves;}
  
  //Copy assignment operator
  moveArray& operator=(const moveArray& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = new move[num];
    for (int i = 0; i < ctr; i++)
      moves[i] = other.moves[i];
    return *this;
  }
  
  //Move assignment operator
  moveArray& operator=(moveArray&& other)
  {
    num = other.num;
    ctr = other.ctr;
    moves = other.moves;
    other.moves = NULL;
    return *this;
  }
  
  //Array dereference operator
  inline move& operator[](const int n) const {return moves[n];}
  
  //Public methods
  inline int size() const {return ctr;}
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
  }

  inline moveArray shrink_to_fit()
  {
    moveArray result(ctr);
    result.ctr = ctr;
    for (int i = 0; i < ctr; i++)
      result.moves[i] = moves[i];
    return result;
  }
};

#endif
