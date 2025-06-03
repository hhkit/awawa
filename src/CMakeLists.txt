
set(AWAWA_BIN awawa)

set(AWAWA_SOURCES
  app/application.cpp
  app/diligent.cpp
  app/glfw.cpp
  app/os.cpp

  gfx/pipeline.cpp
  gfx/utils.cpp

  main.cpp
)

set(AWAWA_DEPENDENCIES
  glfw
  # glm::glm
)

if(PLATFORM_MACOS)
  list(APPEND AWAWA_SOURCES 
    app/surfacehelper.mm
  )
endif()

add_executable(${AWAWA_BIN} 
  ${AWAWA_SOURCES}
)

  # handle shaders
add_slang_shader(tutorial-vs
  SOURCE shaders/tutorial.slang
  STAGE vertex)

add_slang_shader(tutorial-fs
    SOURCE shaders/tutorial.slang
    STAGE fragment)

set(AWAWA_SHADERS 
  tutorial-vs
  tutorial-fs
)

foreach(SHADER IN LISTS AWAWA_SHADERS)
  add_dependencies(
    ${AWAWA_BIN}
    ${SHADER}
  )
endforeach()

target_include_directories(${AWAWA_BIN}
PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

get_supported_backends(ENGINE_LIBRARIES)

target_link_libraries(${AWAWA_BIN}
PRIVATE
  Diligent-Common
  Diligent-GraphicsTools
  ${ENGINE_LIBRARIES}
  ${AWAWA_DEPENDENCIES}
)