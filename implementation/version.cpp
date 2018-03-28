#include "version.hpp"
#include "log.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

namespace system_info
{

Version::Version( const std::string& versionStr )
{
   *this = GetVersionFromString( versionStr );
}

std::string Version::ToString() const
{
   return std::to_string( MajorVersion ) + "." + std::to_string( MinorVersion );
}

Version Version::GetVersionFromString( const std::string& versionStr )
{
   Version version;

   std::string versionWithSpaces = versionStr;
   std::replace( versionWithSpaces.begin(), versionWithSpaces.end(), '.', ' ' );

   std::istringstream iss( versionWithSpaces );
   const std::vector< std::string > results( ( std::istream_iterator< std::string >( iss ) ),
                                               std::istream_iterator< std::string>() );

   if( results.size() != 2 )
   {
      HIDLog::Write( "Строка с номером версии задана некорретно."
                     "Значение по умолчанию 1.0", LogLevel::Error );
      version.MajorVersion = 1;
      version.MinorVersion = 0;
      return version;
   }

   version.MajorVersion = std::stoi( results[ 0 ] );
   version.MinorVersion = std::stoi( results[ 1 ] );

   return version;
}

bool operator<( const Version& version1, const Version& version2 )
{
   if( version1.MajorVersion < version2.MajorVersion )
      return true;
   else if( version1.MajorVersion == version2.MajorVersion )
      if( version1.MinorVersion < version2.MinorVersion )
         return true;

   return false;
}

bool operator==( const Version& version1, const Version& version2 )
{
   if( version1.MajorVersion == version2.MajorVersion &&
       version1.MinorVersion == version2.MinorVersion )
      return true;

   return false;
}

bool operator!=( const Version& version1, const Version& version2 )
{
   return !( version1 == version2 );
}

bool operator>=( const Version& version1, const Version& version2 )
{
   return !( version1 < version2 );
}

bool operator<=( const Version& version1, const Version& version2 )
{
   return ( version1 == version2 ) || ( version1 < version2 );
}

bool operator>( const Version& version1, const Version& version2 )
{
   return !( ( version1 == version2 ) || ( version1 < version2 ) );
}

} // end namespace system_info
