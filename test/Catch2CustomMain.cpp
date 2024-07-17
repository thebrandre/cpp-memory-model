#include <catch2/catch_session.hpp>
#include <fmt/format.h>
#include <locale>

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int Argc, char *Argv[]) {
#ifdef _WIN32
  const auto PreviousLocale = std::locale::global(std::locale("en_US.UTF-8"));
  // Stack Overflow: "Properly print utf8 characters in windows console"
  // https://stackoverflow.com/q/10882277
  const UINT PreviousConsoleCodepage = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
#endif
  const int Result = Catch::Session().run(Argc, Argv);
  // just a quick Unicode consistency check 😉
  if (Result == 0)
    fmt::println("Good for you! 😊");
  else
    fmt::println("Oh no! Not again! 🙄🤦‍♀️🤦‍♂️");
#ifdef _WIN32
  SetConsoleOutputCP(PreviousConsoleCodepage);
  std::locale::global(PreviousLocale);
#endif
  return Result;
}