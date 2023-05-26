#include <cassert>

// Struct and CTAD guide implementing the overload pattern
// https://www.cppstories.com/2019/02/2lines3featuresoverload.html/
template <typename... Ts> struct Overload : Ts... { using Ts::operator()...; };
template <typename... Ts> Overload(Ts...) -> Overload<Ts...>;

// Simulates a unit test for that pattern
static void test()
{
  const auto func = Overload{
    [](int) { return 1; },
    [](double, bool) { return 3.0; },
  };

  assert(func(0) == 1);
  assert(func(0.0, false) == 3.0);

  // Uncomment the following line in to provoke the failure on any compiler version
  // *((char*)(&func) - 1) = 1;
}

static void setupRuntimeCheckHandling();
static int exitCode = 0;

// Main
int main()
{
  setupRuntimeCheckHandling();

  test();

  return exitCode;
}

// Boilerplate code to properly report runtime check issues

#include <crtdbg.h>
#include <initializer_list>

static int reportHook(int reportType, char *message, int *returnValue)
{
  exitCode = 1;
  return 0;
}

static void setupRuntimeCheckHandling()
{
  // Report run-time check issues on the command-line
  for (const auto reportType : {_CRT_WARN, _CRT_ERROR, _CRT_ASSERT})
  {
    _CrtSetReportMode(reportType, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(reportType, _CRTDBG_FILE_STDERR);
  }

  // Let the program fail on runtime check issues
  _CrtSetReportHook(reportHook);
}
