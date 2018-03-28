#pragma once

#include <string>

namespace system_info
{

// Предварительное объявление
struct Version;

/**
 * @brief Класс-помощник и различными
 * вспомогательными функциями
 */
class Util
{
public:
   /**
    * @brief Получить текущее время
    * @return Время в строковом формате типа [час:мин:сек:мс]
    */
   static std::string GetTime();

   /**
    * @brief Сформировать из 32-символьной строки (хеша)
    * полноценный UUID с префиксом-версией алгоритма
    * @param hash 32 символьная строка
    * @param version Версия алгоритма
    * @return UUID с префиксом-версией
    */
   static std::string HashToUUID( const std::string& hash, const Version& version );

   /**
    * @brief Преобразует строку из широких символов в
    * соответствующую ей строку многобайтовых символов
    * @param ws Широкая строка
    * @return Сооветствующая ей узкая строка
    * @warning Поскольку является оберткой для wcstombs,
    * может не распознавать некоторые символы, и таким образом
    * возвращать пустую строку
    */
   static std::string WstringToString( const std::wstring& ws );
private:
   /**
    * @brief Поменять местами байты в строковом представлении
    * @param uuid 32 символьная строка
    * @param begin Начало секции
    * @param end Конец секции
    */
   static void SwapUUIDBlocks( std::string& uuid, const int begin, const int end );
};

} // end namespace system_info
