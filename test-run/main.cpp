#include "../hardware_gen.hpp"
// Чтобы линковщик нашел библиотеку,
// она должна находиться там же, где
// исполняемый файл

#include <cstring>
#include <iostream>

int main( int argc, char* argv[] )
{
   const int buffSize = 200;
   char memory[ buffSize ];
   memset( memory, '\0', buffSize );

   // Получение HardwareID
   GetHardwareId( memory, "1.4", "" );
   std::string hid( memory );

   // Выводим результат
   if( hid.size() > 0 )
      std::cout << hid << std::endl ;
   else
      return -1;

   return 0;
}
