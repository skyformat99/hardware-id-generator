#pragma once

#include <string>

// Предварительное объявление
struct IWbemServices;
using BSTR = wchar_t*;
using GUID = struct _GUID;
using CLSID = GUID;
using IID = GUID;

namespace system_info
{

// Предварительное объявление
struct Version;

/**
 * @brief Класс для работы с железом
 * в ОС Windows
 */
class NativeOSManager
{
public:
   /**
    * @brief Получить строку с идентификаторами железа (обертка)
    * @param version Версия алгоритма
    * @return Строка с идентификаторами железа, следующими
    * друг за другом
    */
   static std::string GetHardwareProperties( const Version& version );
private:
   /**
    * @brief Перевести строку в нижний регистр
    * @param string Результат с символами в нижнем регистре
    */
   static void ToLower( std::wstring& string );

   /**
    * @brief Получить строку с идентификаторами железа
    * @param hidVeriosn Версия алгоритма
    * @return Строка с идентификаторами железа, следующими
    * друг за другом
    */
   static std::wstring GetWmiProperties( const Version& hidVeriosn );

   /**
    * @brief Получить значение свойства класса через WMI
    * @param services Интерфейс для взаимодействия с WMI
    * @param classname Целевой класс
    * @param property Целевое свойство класса
    * @param check Проверка на пустое значение
    * @return Значение указанного свойства класса
    */
   static std::wstring GetWmiProperty( IWbemServices* services, const wchar_t* classname, const wchar_t* property, bool check = true );

   /**
    * @brief Получить идентификатор раздела жесткого диска
    * @param services Интерфейс для взаимодействия с WMI
    * @param check Алгоритм старше версии 1.2?
    * @return Значение идентификатора раздела жесткого диска
    */
   static std::wstring GetWmiPropertyForHdd( IWbemServices* services, bool check );

   /**
    * @brief Получить MAC-адрес
    * @param services Интерфейс для взаимодействия с WMI
    * @return Значние MAC-адреса
    */
   static std::wstring GetWmiPropertyForNetworkAdapter( IWbemServices* services );

   /**
    * @brief Обработать системное значение
    * @param prop Строка в формате BSTR
    * @param check Проверка на пустое значение. Если
    * включена, то пустое системное значние будет трактовано
    * как пустая строка
    * @return Значение в формате std::wstring
    */
   static std::wstring ProcessWmiProperty( BSTR prop, bool check );
private:
   const static CLSID local_CLSID_WbemLocator;
   const static IID local_IID_IWbemLocator;
};

} // end namespace system_info
