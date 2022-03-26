# AUTOGENERATED, DON'T CHANGE THIS FILE!

if (NOT yandex-c-ares_FIND_VERSION OR yandex-c-ares_FIND_VERSION VERSION_LESS 1.16.0)
    set(yandex-c-ares_FIND_VERSION 1.16.0)
endif()

if (NOT USERVER_CHECK_PACKAGE_VERSIONS)
  unset(yandex-c-ares_FIND_VERSION)
endif()

if (TARGET yandex-c-ares)
  if (NOT yandex-c-ares_FIND_VERSION)
      set(yandex-c-ares_FOUND ON)
      return()
  endif()

  if (yandex-c-ares_VERSION)
      if (yandex-c-ares_FIND_VERSION VERSION_LESS_EQUAL yandex-c-ares_VERSION)
          set(yandex-c-ares_FOUND ON)
          return()
      else()
          message(FATAL_ERROR
              "Already using version ${yandex-c-ares_VERSION} "
              "of yandex-c-ares when version ${yandex-c-ares_FIND_VERSION} "
              "was requested."
          )
      endif()
  endif()
endif()

set(FULL_ERROR_MESSAGE "Could not find `yandex-c-ares` package.\n\tDebian: sudo apt update && sudo apt install libyandex-taxi-c-ares-dev\n\tMacOS: brew install c-ares")


include(FindPackageHandleStandardArgs)

find_library(yandex-c-ares_LIBRARIES_yandex-taxi-cares_cares
  NAMES yandex-taxi-cares cares
)
list(APPEND yandex-c-ares_LIBRARIES ${yandex-c-ares_LIBRARIES_yandex-taxi-cares_cares})

find_path(yandex-c-ares_INCLUDE_DIRS_ares_h
  NAMES ares.h
  PATH_SUFFIXES yandex-taxi-c-ares
)
list(APPEND yandex-c-ares_INCLUDE_DIRS ${yandex-c-ares_INCLUDE_DIRS_ares_h})



if (yandex-c-ares_VERSION)
  set(yandex-c-ares_VERSION ${yandex-c-ares_VERSION})
endif()

if (yandex-c-ares_FIND_VERSION AND NOT yandex-c-ares_VERSION)
  include(DetectVersion)

  if (UNIX AND NOT APPLE)
    deb_version(yandex-c-ares_VERSION libyandex-taxi-c-ares-dev)
  endif()
  if (APPLE)
    brew_version(yandex-c-ares_VERSION c-ares)
  endif()
endif()

 
find_package_handle_standard_args(
  yandex-c-ares
    REQUIRED_VARS
      yandex-c-ares_LIBRARIES
      yandex-c-ares_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  yandex-c-ares_LIBRARIES
  yandex-c-ares_INCLUDE_DIRS
  
)

if (NOT yandex-c-ares_FOUND)
  if (yandex-c-ares_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}. Required version is at least ${yandex-c-ares_FIND_VERSION}")
  endif()

  return()
endif()

if (yandex-c-ares_FIND_VERSION)
  if (yandex-c-ares_VERSION VERSION_LESS yandex-c-ares_FIND_VERSION)
      message(STATUS
          "Version of yandex-c-ares is '${yandex-c-ares_VERSION}'. "
          "Required version is at least '${yandex-c-ares_FIND_VERSION}'. "
          "Ignoring found yandex-c-ares."
          "Note: Set -DUSERVER_CHECK_PACKAGE_VERSIONS=0 to skip package version checks if the package is fine."
      )
      set(yandex-c-ares_FOUND OFF)
      return()
  endif()
endif()

 
if (NOT TARGET yandex-c-ares)
  add_library(yandex-c-ares INTERFACE IMPORTED GLOBAL)

  target_include_directories(yandex-c-ares INTERFACE ${yandex-c-ares_INCLUDE_DIRS})
  target_link_libraries(yandex-c-ares INTERFACE ${yandex-c-ares_LIBRARIES})
  
  # Target yandex-c-ares is created
endif()

if (yandex-c-ares_VERSION)
  set(yandex-c-ares_VERSION "${yandex-c-ares_VERSION}" CACHE STRING "Version of the yandex-c-ares")
endif()