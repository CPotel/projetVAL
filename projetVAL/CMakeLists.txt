# CMakeList.txt : projet CMake pour projetVAL, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (projetVAL "projetVAL.cpp" "projetVAL.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET projetVAL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib)
target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib)
target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-main.lib)
target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib)
target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib)
target_link_libraries(projetVAL ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)