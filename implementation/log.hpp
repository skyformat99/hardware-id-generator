#pragma once

#include <string>

namespace system_info
{

/**
 * @brief Уровень логирования
 */
enum LogLevel
{
   // Информационные сообщения (по умолчанию)
   Info,
   // Для отладки
   Debug,
   // Ошибка
   Error
};

/**
 * @brief Класс для логирования информационных
 * сообщений
 */
class HIDLog
{
public:
   /**
    * @brief Указать папку для логирования
    * информационных сообщений
    * @param path Путь до папки, в которой будем логировать
    */
   static void SetLogPath( const std::string& path );

   /**
    * @brief Залогировать информационное сообщение
    * @param message Сообщение
    * @param level Уровень логирования
    */
   static void Write( const std::string& message, LogLevel level = LogLevel::Info );

   /**
    * @brief Залогировать информационное сообщение
    * @param message Сообщение (из широких символов)
    * @param level Уровень логирования
    */
   static void Write( const std::wstring& message, LogLevel level = LogLevel::Info );
private:
   /**
    * @brief Преобразовать перечисление в стровой вид
    * @param level Уровень логирования
    * @return Соответствующее уровню логирования
    * строковое представление
    * @example LogLevel::Debug <-> "[Debug]"
    */
   static std::string LogLevelToString( LogLevel level );
private:
   // Путь до файла с логированием
   static std::string fileLogPath;
};

} // end namespace system_info
