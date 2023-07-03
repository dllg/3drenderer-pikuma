
message(STATUS "find_package(SDL2 CONFIG REQUIRED)")

# Add find_package calls here
find_package(SDL2 CONFIG REQUIRED)

if (NOT TARGET SDL2::SDL2)
  message(FATAL_ERROR "SDL2::SDL2 not found")
endif()

# Add dependencies here
set(dependencies
    SDL2::SDL2
    m
)

# Add include directories here
set (include_dirs
  ""
)

# Add library directories here
set (library_dirs
  ""
)
