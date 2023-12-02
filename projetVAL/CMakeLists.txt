# CMakeList.txt : projet CMake pour projetVAL, incluez la source et définissez
# la logique spécifique au projet ici.
#


if(MSVC)
  set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
  set(SFML_STATIC_LIBRARIES TRUE)
  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
endif()

# Ajoutez une source à l'exécutable de ce projet.
add_executable (projetVAL "projetVAL.cpp" "projetVAL.h")


