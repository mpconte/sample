add_compile_options(-D COMPILING_LIBS)

add_library(main SHARED
  main.cpp
  )
target_link_libraries(main boost::boost ${Python3_LIBRARIES})       

install(TARGETS main 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib)
