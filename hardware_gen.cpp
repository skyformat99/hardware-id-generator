#include "hardware_gen.hpp"
#include "implementation/log.hpp"
#include "implementation/md5.hpp"
#include "implementation/util.hpp"
#include "implementation/version.hpp"

#if defined( SYSTEM_WINDOWS )
#include "implementation/windows/windows_manager.hpp"
#elif defined( SYSTEM_LINUX )
#include "implementation/linux/linux_manager.hpp"
#elif defined( SYSTEM_MAC )
#include "implementation/mac/mac_manager.hpp"
#endif

#include <cstring>
#include <ctime>
#include <string>

using namespace system_info;

void GetHardwareId( char* hardwareIdResult, const char* versionStr, const char* logPath )
{
   try
   {
      // Инициализация логгера
      HIDLog::SetLogPath( logPath );
      HIDLog::Write( "Выполняется метод GetHardwareId", LogLevel::Debug );

      // Получаем HID заданной версии
      const Version version( versionStr );
      std::string hid = NativeOSManager::GetHardwareProperties( version );

      // Если не удалось получать никаких данных о железе - генерируем случайную строку
      if( hid.empty() )
      {
         HIDLog::Write( "Не удалось получить ни один из всех идентификаторов железа. "
                        "Будет сгенерирован случайный UUID", LogLevel::Debug );
         std::srand( std::time( nullptr ) );
         int rand = std::rand();
         hid = std::to_string( rand );
      }
      else
         HIDLog::Write( "Идентификаторы получены.", LogLevel::Debug );

      // Получаем MD5 хеш данных о железе - есть наш UUID
      MD5 md5( hid );
      std::string md5hash = md5.HexDigest();
      std::string uuid = Util::HashToUUID( md5hash, version );
      HIDLog::Write( "Получен HID: " + uuid, LogLevel::Debug );

      // Копируем результат в буффер
      memset( hardwareIdResult, '\0', uuid.size() + 1 );
      memcpy( hardwareIdResult, uuid.c_str(), uuid.size() + 1 );
   }
   catch( ... )
   {
      HIDLog::Write( "Непредсказуемая ошибка в GetHardwareId", LogLevel::Error );
   }
}

static const std::string libraryVersion = "1.4";

void GetVersion( char *libVersion )
{
   // Копируем результат в буффер
   memset( libVersion, '\0', libraryVersion.size() + 1 );
   memcpy( libVersion, libraryVersion.c_str(), libraryVersion.size() + 1 );
}
