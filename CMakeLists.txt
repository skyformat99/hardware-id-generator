cmake_minimum_required (VERSION 3.5) 

project( HardwareIdGenerator )

set( HID_GEN_SOURCES
    "hardware_gen.hpp" "hardware_gen.cpp"
    "implementation/md5.hpp" "implementation/md5.cpp"
    "implementation/version.hpp" "implementation/version.cpp"
    "implementation/util.hpp" "implementation/util.cpp"
    "implementation/log.hpp" "implementation/log.cpp" )

# Для каждой ОС загружаем специфичный ей сервис для работы с ситемой
# и по мере надобности загружаем / определяем глобальные переменные
# и библиотеки для работы с системой
if( WIN32 )
   set( NATIVE_SOURCES
       "implementation/windows/windows_manager.hpp" "implementation/windows/windows_manager.cpp" )
   add_definitions( -DSYSTEM_WINDOWS )

   add_definitions( -D_WIN32_LEAN_AND_MEAN )
   add_definitions( -D_WIN32_WINNT=0x0600 )

elseif( UNIX AND NOT APPLE )
   set( NATIVE_SOURCES
       "implementation/linux/linux_manager.hpp" "implementation/linux/linux_manager.cpp" )
   add_definitions( -DSYSTEM_LINUX )

elseif( APPLE )
   set( NATIVE_SOURCES
     "implementation/mac/mac_manager.hpp" "implementation/mac/mac_manager.cpp" )
   add_definitions( -DSYSTEM_MAC )
endif()

add_library( ${PROJECT_NAME} SHARED ${HID_GEN_SOURCES} ${NATIVE_SOURCES} )

set( CMAKE_CXX_FLAGS "-std=c++11" )

if( APPLE )
   include_directories( "Developer/Headers/FlatCarbon" )
   find_library( CARBON_LIBRARY Carbon )
   find_library( IOKIT_LIBRARY IOKit )
   mark_as_advanced( CARBON_LIBRARY IOKIT_LIBRARY )
   set( EXTRA_LIBS ${CARBON_LIBRARY} ${IOKIT_LIBRARY} )
   target_link_libraries( ${PROJECT_NAME} ${EXTRA_LIBS} )
endif()
