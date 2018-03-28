#include "windows_manager.hpp"
#include "../log.hpp"
#include "../util.hpp"
#include "../version.hpp"

// Windows-specific include
#include <shlobj.h>
#include <wbemcli.h>

// Standart include
#include <algorithm>

namespace system_info
{

struct BSTRHolder
{
   BSTRHolder( const wchar_t* str )
   {
      bstr = SysAllocString( str );
   }

   ~BSTRHolder()
   {
      if( bstr )
         SysFreeString( bstr );
   }

   BSTR bstr;
};

// В mingw CLSID_WbemLocator и IID_IWbemLocator выдают undefined reference, используем свой "аналог"
const CLSID NativeOSManager::local_CLSID_WbemLocator = { 0x4590F811, 0x1D3A, 0x11D0, { 0x89, 0x1F, 0, 0xAA, 0, 0x4B, 0x2E, 0x24 } };
const IID NativeOSManager::local_IID_IWbemLocator = { 0xdc12a687, 0x737f, 0x11cf, { 0x88, 0x4d, 0, 0xAA, 0, 0x4B, 0x2E, 0x24 } };

// version пока не используем
std::string NativeOSManager::GetHardwareProperties( const Version& version )
{
   HIDLog::Write( "Получаем данные о железе в среде Windows", LogLevel::Debug );
   std::wstring hardwareId = L"";

   if( SUCCEEDED( CoInitialize( nullptr ) ) )
   {
      hardwareId = GetWmiProperties( version );
      CoUninitialize();
   }

   return Util::WstringToString( hardwareId );
}

std::wstring NativeOSManager::GetWmiProperties( const Version& version )
{
   std::wstring result;
   //CoInitializeSecurity

   IWbemLocator* locator;
   if( SUCCEEDED( CoCreateInstance( local_CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER, local_IID_IWbemLocator, reinterpret_cast< void** >( &locator ) ) ) )
   {
      IWbemServices* services;
      BSTRHolder net( L"ROOT\\CIMV2" );
      if( SUCCEEDED( locator->ConnectServer( net.bstr, nullptr, nullptr, nullptr, WBEM_FLAG_CONNECT_USE_MAX_WAIT, nullptr, nullptr, &services ) ) )
      {
         if( SUCCEEDED( CoSetProxyBlanket( services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE ) ) )
         {
            try
            {
               if( version <= Version( "1.3" ) )
               {
                  result += GetWmiProperty( services, L"Win32_ComputerSystemProduct", L"UUID" );
                  result += GetWmiProperty( services, L"Win32_OperatingSystem", L"SerialNumber" );
               }
               result += GetWmiPropertyForHdd( services, version >= Version( "1.2" ) );
               result += GetWmiProperty( services, L"Win32_ComputerSystemProduct", L"IdentifyingNumber" );
               result += GetWmiProperty( services, L"Win32_BaseBoard", L"SerialNumber" );
               if( version > Version( "1.0" ) )
                  result += GetWmiPropertyForNetworkAdapter( services );
            }
            catch( ... )
            {
               result.clear();
            }
         }
         services->Release();
      }
      locator->Release();
   }

   return result;
}

std::wstring NativeOSManager::GetWmiProperty( IWbemServices* services, const wchar_t* classname, const wchar_t* property, bool check )
{
   std::wstring result;
   IEnumWbemClassObject* enumerator = nullptr;
   try
   {
      std::wstring query = L"SELECT * FROM ";
      query += classname;
      BSTRHolder query_holder( query.c_str() );
      BSTRHolder lang( L"WQL" );
      if( SUCCEEDED( services->ExecQuery( lang.bstr, query_holder.bstr, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &enumerator ) ) )
      {
         IWbemClassObject* object;
         ULONG returned = 0;
         if( SUCCEEDED( enumerator->Next( WBEM_INFINITE, 1, &object, &returned ) ) && ( returned > 0 ) )
         {
            VARIANT value;
            CIMTYPE type;
            if( SUCCEEDED( object->Get( property, 0, &value, &type, nullptr ) ) )
            {
               if( ( type == CIM_STRING ) && ( value.vt == VT_BSTR ) )
                  result += ProcessWmiProperty( V_BSTR( &value ), check );

               HIDLog::Write( std::wstring() + L"WMI property \"" + property + L"\" FOR CLASS \"" +
                              classname + L"\" = \"" + result + L"\"", LogLevel::Debug );
               VariantClear( &value );
            }
            object->Release();
         }
      }
   }
   catch( ... )
   {
      result.clear();
   }

   if( enumerator )
      enumerator->Release();

   return result;
}

std::wstring NativeOSManager::GetWmiPropertyForHdd( IWbemServices* services, bool check )
{
   std::wstring from = L"win32_logicaldisk WHERE deviceid=\"";
   PWSTR location;
   if( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_System, KF_FLAG_DONT_UNEXPAND, nullptr, &location ) ) )
   {
      from.append( 1, *location );
      from.append( L":\"" );
      CoTaskMemFree( location );
      return GetWmiProperty( services, from.c_str(), L"VolumeSerialNumber", check );
   }

   HIDLog::Write( "Идентификатор раздела жесткого диска не удалось получить", LogLevel::Debug );
   return std::wstring();
}

std::wstring NativeOSManager::ProcessWmiProperty( BSTR prop, bool check )
{
   std::wstring value( prop, SysStringLen( prop ) );
   ToLower( value );
   // Внимание: все константы в нижнем регистре
   const std::wstring empty_guid( L"00000000-0000-0000-0000-000000000000" );
   const std::wstring broken_guid( L"ffffffff-ffff-ffff-ffff-ffffffffffff" );
   const std::wstring empty_oem( L"to be filled by o.e.m." );

   if( check )
   {
      if( ( value == empty_guid ) || ( value == broken_guid ) || ( value == empty_oem ) )
      {
         return std::wstring();
      }
   }

   return value;
}

std::wstring NativeOSManager::GetWmiPropertyForNetworkAdapter( IWbemServices* services )
{
   return GetWmiProperty( services, L"Win32_NetworkAdapter WHERE Manufacturer != \'Microsoft\' AND NOT PNPDeviceID LIKE \'ROOT%\\\\\'", L"MACAddress", false );
}

void NativeOSManager::ToLower( std::wstring& string )
{
   std::transform( string.begin(), string.end(), string.begin(), tolower );
}

} // end namespace system_info
