#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef DLL_EXPORT
   #ifdef SYSTEM_WINDOWS
      #define DLL_EXPORT __declspec(dllexport)
   #else
      #define DLL_EXPORT extern
   #endif
#endif

/**
 * @brief Получить идентификатор железа (HID)
 * @param hardwareIdResult Результат - буфер для идентификатора
 * @param version Версия алгоритма
 * @param logPath Путь к папке логирования
 */
DLL_EXPORT void GetHardwareId( char* hardwareIdResult, const char* version, const char* logPath );

/**
 * @brief Получить версию библиотеки
 * @param libVersion Результат - буфер для версии
 */
DLL_EXPORT void GetVersion( char* libVersion );

#ifdef __cplusplus
}
#endif
