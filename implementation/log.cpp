#include "log.hpp"
#include "util.hpp"

#include <fstream>
#include <iomanip>

namespace system_info
{

std::string HIDLog::fileLogPath = "";

void HIDLog::SetLogPath( const std::string &path )
{
   if( path.empty() )
      return;

   fileLogPath = path;

   // Если папка указана без разделителя в конце, вставляем сами
   if( fileLogPath.at( fileLogPath.size() - 1 ) != '/' ||
       fileLogPath.at( fileLogPath.size() - 1 ) != '\\' )
      fileLogPath += "/";

   // Имя файла-лога
   fileLogPath += "hid.log";
}

void HIDLog::Write( const std::string& message, LogLevel level )
{
   if( fileLogPath.empty() )
      return;

   const int timeColumnWidth = 15;
   const int logLevelWidth = 7;

   std::ofstream file( fileLogPath, std::ios::in | std::ios::out | std::ios::app );
   if( file.is_open() )
   {
      file << std::setw( timeColumnWidth ) << std::left << Util::GetTime()
           << std::setw( logLevelWidth ) << std::left << LogLevelToString( level )
           << " " << message << std::endl;
      file.close();
   }
}

void HIDLog::Write( const std::wstring& message, LogLevel level )
{
   Write( Util::WstringToString( message ), level );
}

std::string HIDLog::LogLevelToString( LogLevel level )
{
   switch( level )
   {
   case LogLevel::Error:
      return "[Error]";
   case LogLevel::Debug:
      return "[Debug]";
   case LogLevel::Info:
   default:
      return "[Info]";
   }
}

} // end namespace system_info
