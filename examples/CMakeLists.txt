project (alGuiExamples)

add_executable(ex_gui ex_gui.cpp)
target_link_libraries(ex_gui ${ALLEGRO_LIBRARIES})
target_link_libraries(ex_gui alGui)
