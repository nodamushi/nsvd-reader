find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()



message( ${GTEST_INCLUDE_DIRS})
function(def_test name)

  add_executable(${name} ${name}.cpp)

  target_include_directories(${name}
    PUBLIC
    ../include
    ${GTEST_INCLUDE_DIRS})

  target_link_libraries(${name}
    PRIVATE
    GTest::GTest
    GTest::Main
    # Shlwapi
    )

  set_target_properties(${name}
    PROPERTIES POSITION_INDEPENDENT_CODE ON)

  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:${name}>
    )

endfunction()


def_test(pathTest)
def_test(dim_type_test)
def_test(EnumeratedNumberTest)
def_test(clusterTest)
def_test(sample_readTest)
def_test(sample_normTest)
def_test(dimname_test)


add_executable(link_test
  link_test_bar.cpp
  link_test_foo.cpp
  link_test_main.cpp
  )

target_include_directories(link_test
  PUBLIC
  ../include)
