#include <absl/random/random.h>

#include <fmt/format.h>

int main()
{
   fmt::print("Hello, World!\n");

   absl::BitGen bitgen;
   const auto number = absl::Uniform<unsigned>(bitgen);

   fmt::print("Your lucky number is: {}\n", number);

   return 0;
}
