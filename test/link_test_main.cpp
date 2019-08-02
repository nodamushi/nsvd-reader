#include "nodamushi/svd.hpp"
#include "nodamushi/svd/normalized.hpp"

using namespace nodamushi::svd::normalized;
void foo(const std::string& filename);
void bar(const std::string& filename);


int main(int argc, char *argv[])
{
  foo("sample.svd");
  bar("sample.svd");
  return 0;
}

