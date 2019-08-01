
function(sample name)
add_executable(${name}
  ${name}.cpp)

target_include_directories(${name}
  PUBLIC
  ../include)
  
endfunction(sample)

sample(read_and_print_svd)

target_link_libraries(read_and_print_svd
  PUBLIC
  expat
  tinyxml2)

sample(visitor_sample)
sample(normalize_sample)

