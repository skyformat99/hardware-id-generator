#include "linux_manager.hpp"
#include "../log.hpp"
#include "../util.hpp"
#include "../version.hpp"

// Linux include specific
#include <dirent.h>

// Standart include
#include <fstream>
#include <map>

namespace system_info
{

std::string NativeOSManager::GetHardwareProperties( const Version& hidVersion )
{
   HIDLog::Write( "Получаем данные о железе в среде Linux", LogLevel::Debug );
   std::string result = "";

   result += GetDmiProperty( "product", "uuid" );
   result += GetDmiProperty( "product", "serial" );
   result += GetDmiProperty( "board", "serial" );
   result += GetHDDSerialNumber();
   result += GetMACAddress();

   return result;
}

std::string NativeOSManager::GetDmiProperty( const std::string& service, const std::string& property )
{
   std::string result = "";
   std::ifstream file( "/sys/class/dmi/id/" + service + "_" + property );

   if( file.is_open() )
   {
      std::getline( file, result );
      file.close();
   }

   HIDLog::Write( "Значение свойства \'" + property + "\' для \'" + service + "\'"
                  " = \'" + result + "\'", LogLevel::Debug );
   return result;
}

std::string NativeOSManager::GetHDDSerialNumber()
{
   // Открываем директорию с идентификаторами жестких дисков
   const std::string uuidFolder = "/dev/disk/by-uuid/";
   std::vector< std::string > uuids;

   if( !GetItemsFromFolder( uuidFolder, uuids, DT_LNK ) )
   {
      HIDLog::Write( "Не смогли получить список идентификаторов"
                     "в папке /dev/disk/by-uuid/", LogLevel::Debug );
      return "";
   }

   // Формируем словарь "служебное имя жесткого диска - UUID"
   // Коллекция упорядочена по ключу
   std::map< std::string, std::string > devices;
   char buffer[ 200 ];
   for( const std::string& uuid : uuids )
   {
      std::string uuidPath = uuidFolder + uuid;
      realpath( uuidPath.c_str(), buffer );
      // Вставка происходит с сортировкой по ключу
      devices[ buffer ] = uuid;
   }

   // Находим самое раннее имя sda или hda и возвращаем его UUID
   for( const auto& item : devices )
   {
      if ( item.first.find( "sda" ) != std::string::npos ||
           item.first.find( "hda" ) != std::string::npos )
      {
         HIDLog::Write( "Серийный номер жесткого диска: " + item.second, LogLevel::Debug );
         return item.second;
      }
   }

   HIDLog::Write( "Не смогли получить серийный номер жесткого диска", LogLevel::Debug );
   return "";
}

std::string NativeOSManager::GetMACAddress()
{
   // Открываем директорию с именами сетевых интерфейсов
   const std::string netFolder = "/sys/class/net/";
   std::vector< std::string > netNames;

   if( !GetItemsFromFolder( netFolder, netNames, DT_LNK ) )
   {
      HIDLog::Write( "Не смогли получить список идентификаторов"
                     "в папке /sys/class/net/", LogLevel::Debug );
      return "";
   }

   // Формируем словарь "служебное имя сетевого устройства - сетевой адрес"
   // Коллекция упорядочена по ключу
   std::map< std::string, std::string > netDevices;
   std::string tempAddress = "";
   for( const std::string& netName : netNames )
   {
      std::string addressPath = netFolder + netName + "/address";
      std::ifstream addressFile( addressPath );
      tempAddress = "";

      if( addressFile.is_open() )
      {
         std::getline( addressFile, tempAddress );
         addressFile.close();
      }

      netDevices[ netName ] = tempAddress;
   }

   // Находим самое раннее имя eth или enp и возвращаем его сетевой адрес
   for( const auto& item : netDevices )
   {
      if ( item.first.find( "eth" ) != std::string::npos ||
           item.first.find( "enp" ) != std::string::npos )
      {
         HIDLog::Write( "MAC-адрес: " + item.second, LogLevel::Debug );
         return item.second;
      }
   }

   HIDLog::Write( "Не смогли получить MAC-адрес", LogLevel::Debug );
   return "";
}

bool NativeOSManager::GetItemsFromFolder( const std::string& folder, std::vector< std::string >& itemList , int type )
{
   DIR* dir = nullptr;
   if( !( dir = opendir( folder.c_str() ) ) )
      return false;

   dirent *dirp = nullptr;
   while( ( dirp = readdir( dir ) ) )
   {
      if( dirp->d_type == type )
         itemList.push_back( std::string( dirp->d_name ) );
   }

   closedir( dir );
   return true;
}

} // end namespace system_info
