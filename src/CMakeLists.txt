
set(APP_NAME awawa)

set(SOURCES
  app/application.cpp
  app/diligent.cpp
  app/glfw.cpp
  main.cpp
)
if(PLATFORM_MACOS)
  list(APPEND SOURCES 
    app/surfacehelper.mm
  )
endif()

add_executable(${APP_NAME} 
  ${SOURCES})

target_include_directories(${APP_NAME}
PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

get_supported_backends(ENGINE_LIBRARIES)

target_link_libraries(${APP_NAME}
PRIVATE
  Diligent-Common
  Diligent-GraphicsTools
  ${ENGINE_LIBRARIES}
  glfw
)