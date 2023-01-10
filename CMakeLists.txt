cmake_minimum_required (VERSION 3.0)
project ("SDL2Test")

IF(WIN32)
  message(STATUS "Building for windows")

  set(SDL2_INCLUDE_DIRS "./include/SDL2/include")
  set(SDL2IMAGE_INCLUDE_DIRS "./include/SDL2_image/include")

  # Support both 32 and 64 bit builds
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    message(STATUS "Building for 64bit")
    set(SDL2_LINK_DIRS "./include/SDL2/lib/x64")
    set(SDL2IMAGE_LINK_DIRS "./include/SDL2_image/lib/x64")
  else ()
    message(STATUS "Building for 32bit")
    set(SDL2_LINK_DIRS "./include/SDL2/lib/x86")
    set(SDL2IMAGE_LINK_DIRS "./include/SDL2_image/lib/x86")
  endif ()

  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    add_custom_target(SDL2Dependancies
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIRS}/../lib/x64/SDL2.dll          ${CMAKE_BINARY_DIR}/Release/SDL2.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2IMAGE_INCLUDE_DIRS}/../lib/x64/SDL2_image.dll          ${CMAKE_BINARY_DIR}/Release/SDL2_image.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIRS}/../lib/x64/SDL2.dll          ${CMAKE_BINARY_DIR}/Debug/SDL2.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2IMAGE_INCLUDE_DIRS}/../lib/x64/SDL2_image.dll          ${CMAKE_BINARY_DIR}/Debug/SDL2_image.dll
      COMMENT "Copied SDL2 from '${SDL2_INCLUDE_DIRS}/../lib/x64/SDL2.dll' to '${CMAKE_BINARY_DIR}'" VERBATIM
      COMMENT "Copied SDL2 from '${SDL2IMAGE_INCLUDE_DIRS}/../lib/x64/SDL2_Image.dll' to '${CMAKE_BINARY_DIR}'" VERBATIM
    )
  else ()
    add_custom_target(SDL2Dependancies
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIRS}/../lib/x86/SDL2.dll          ${CMAKE_BINARY_DIR}/Release/SDL2.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2IMAGE_INCLUDE_DIRS}/../lib/x86/SDL2.dll          ${CMAKE_BINARY_DIR}/Release/SDL2.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIRS}/../lib/x86/SDL2.dll          ${CMAKE_BINARY_DIR}/Debug/SDL2.dll
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIRS}/../lib/x86/SDL2.dll          ${CMAKE_BINARY_DIR}/Debug/SDL2.dll
      COMMENT "Copied SDL2 from '${SDL2_INCLUDE_DIRS}/../lib/x86/SDL2.dll' to '${CMAKE_BINARY_DIR}'" VERBATIM
    )
  endif ()

  include_directories(${SDL2_INCLUDE_DIRS})
  include_directories(${SDL2IMAGE_INCLUDE_DIRS})
  link_directories(${SDL2_LINK_DIRS}, ${SDL2IMAGE_LINK_DIRS})

  add_executable(SDL2Test src/main.cpp src/ECS/application.cpp src/ECS/window.cpp src/ECS/entity.cpp src/ECS/systems/render.cpp src/ECS/systems/AI.cpp)
  target_link_libraries(SDL2Test PUBLIC SDL2 SDL2main SDL2_image)
  add_dependencies(SDL2Test SDL2Dependancies)
ELSE()
  message(STATUS "Building for Linux or Mac")

  find_package(SDL2 REQUIRED)
  find_package(SDL2_IMAGE REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS})
  include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

  add_executable(SDL2Test src/main.cpp src/ECS/application.cpp src/ECS/window.cpp src/ECS/entity.cpp src/ECS/systems/render.cpp, src/ECS/systems/AI.cpp)
  target_link_libraries(SDL2Test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
ENDIF()