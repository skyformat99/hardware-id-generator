#pragma once

#include <string>

namespace system_info
{

struct Version;

/**
 * @brief Класс для работы с железом
 * в MAC OS X
 */
class NativeOSManager
{
public:
   /**
    * @brief Получить строку с идентификаторами железа
    * @param hidVersion Версия алгоритма
    * @return Строка с идентификаторами железа, следующими
    * друг за другом
    */
   static std::string GetHardwareProperties( const Version& hidVersion );
private:
   /**
    * @brief Получить указанное свойство сервиса IOPlatform
    * @param property Свойство, чье значение нужно получить
    * @return Значение указанного свойства
    */
   static std::string GetIOPlatformProperty( const std::string& property );

   /**
    * @brief Получить указанное свойство сервиса IONetwork
    * @param property Свойство, чье значение нужно получить
    * @return Значение указанного свойства
    */
   static std::string GetIONetworkProperty( const std::string& property );

   /**
    * @brief Получить указанное свойство сервиса IOStorage
    * @param property Свойство, чье значение нужно получить
    * @return Значение указанного свойства
    */
   static std::string GetIOStorageProperty( const std::string& property );
};

} // end namespace system_info
