// Caden Roberts CIST004A 9/30/22
#include <iostream>
#include <ctime>

int primeFactor (unsigned int num, unsigned int div=2 ){
  if (num == div){
   std::cout << div << " ";
   return;
    }
  if ( num % div == 0 ){
   std::cout << div << ", " ;
   primeFactor (num/div, div);
} else {
   primeFactor (num, div+1);
}
  return;
}

int main() {
   unsigned int number;
   time_t startTime;
   double deltaTimeMS;
  
   std::cout <<"Enter a number to factor: ";
   std::cin >> number;
   std::cout << "The prime factors are: ";
   startTime = clock();
   primeFactor (number);
   deltaTimeMS = static_cast<double>(clock() - startTime)
*1000.0 / CLOCKS_PER_SEC ;
   std::cout << "\nThis took " << deltaTimeMS << " mSec.";
   std::cout << std::endl;

   return 0;
}