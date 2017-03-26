#include <ctime>
int knightLoop(){
  
}



int main(int argc, char const *argv[]) {
  std::clock_t start;
  double duration;

  start = std::clock();

  /* Your algorithm here */

  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout<<"printf: "<< duration <<'\n';
}
