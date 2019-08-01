/*!
  @brief Header file
  @file test_utility.hpp
*/
#ifndef __TEST_UTILITY_HPP__
#define __TEST_UTILITY_HPP__

# include <string>
# if __cplusplus >= 201703

#  include <filesystem>
#  include <string_view>

std::filesystem::path find_directory_path(std::string_view dir)
{
  using namespace std::filesystem;
  path p = dir;
  if(exists(p)){
    return p;
  }
  p = path("..")/p;
  int count = 0;
  while(!exists(p)){
    p =   p = path("..")/p;
    count ++;
    if(count == 10){
      return path();
    }
  }
  return p;
}

std::string find_directory(std::string_view dir)
{
  return find_directory_path(dir).string();
}

std::string find_file(const std::string &dir,const std::string& filename)
{
  auto p = find_directory_path(dir);
  if(!p.empty()){
    p /= filename;
  }
  return p.string();
}
    


# else

#  ifdef _WIN32

#   include <Shlwapi.h>
#   include <windows.h>

std::string find_directory(const std::string &dir)
{
  std::string s = dir;
  if(PathFileExists(s.data())){
    return s;
  }
  
  s = "..\\" + s;
  int count = 0;
  while(!PathFileExists(s.data())){
    s = "..\\" + s;
    count ++;
    if(count == 10){
      return "";
    }
  }
  return s;
}


std::string find_file(const std::string &dir,const std::string& filename)
{
  std::string s = find_directory(dir);
  if(!s.empty()){
    return s + "\\" + filename;
  }
  return "";
}



#  endif

#  ifdef __linux__


#  endif

# endif


#endif // __TEST_UTILITY_HPP__
