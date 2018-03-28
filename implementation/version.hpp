#pragma once

#include <string>

namespace system_info
{

/**
 * @brief Структура, описывающая версию
 */
struct Version
{
   /**
    * @brief Конструктор по умолчанию
    */
   Version() { }

   /**
    * @brief Конструктор с параметром
    * @param versionStr Версия в строковом виде
    * @example "1.0", "2.1"
    */
   Version( const std::string& versionStr );

   /**
    * @brief Получить строковое представление версии
    * @return Строковое представление версии
    */
   std::string ToString() const;

   /**
    * @brief Получить версию из строкого представления
    * @param versionStr Версия в строковом формате
    * @return Версия
    */
   static Version GetVersionFromString( const std::string& versionStr );

   // Главный номер версии
   unsigned MajorVersion = 0;

   // Младший номер версии
   unsigned MinorVersion = 0;
};

// Операторы сравнивания версий
bool operator<( const Version& version1, const Version& version2 );
bool operator==( const Version& version1, const Version& version2 );
bool operator!=( const Version& version1, const Version& version2 );
bool operator>=( const Version& version1, const Version& version2 );
bool operator<=( const Version& version1, const Version& version2 );
bool operator>( const Version& version1, const Version& version2 );

} // end namespace system_info
