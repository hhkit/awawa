
set(APP_NAME awawa-bin)

set(SOURCE 
  app/Application.cpp
)

# ...
add_executable(${APP_NAME}
  ${SOURCE})

target_include_directories(${APP_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )