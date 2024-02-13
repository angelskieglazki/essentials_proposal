#include <benchmark/benchmark.h>
#include <memory>

class CustomMemoryManager : public benchmark::MemoryManager {
 public:
  int64_t num_allocs;
  int64_t max_bytes_used;

  void Start() override {
    num_allocs = 0;
    max_bytes_used = 0;
  }

  void Stop(Result& result) override {
    result.num_allocs = num_allocs;
    result.max_bytes_used = max_bytes_used;
  }
};

std::unique_ptr<CustomMemoryManager> mm(new CustomMemoryManager());

#define MEMORY_PROFILER
#ifdef MEMORY_PROFILER
void* custom_malloc(size_t size) {
  void* p = malloc(size);
  mm.get()->num_allocs += 1;
  mm.get()->max_bytes_used += size;
  return p;
}
#define malloc(size) custom_malloc(size)
#endif

static void BM_memory(benchmark::State& state) {
  for (auto _ : state)
    for (int i = 0; i < 10; i++) {
      benchmark::DoNotOptimize((int*)malloc(10 * sizeof(int*)));
    }
}

BENCHMARK(BM_memory)->Unit(benchmark::kMillisecond)->Iterations(17);

// BENCHMARK_MAIN();
int main(int argc, char** argv) {
  ::benchmark::RegisterMemoryManager(mm.get());
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::RegisterMemoryManager(nullptr);
}

// #include <cstring> // malloc
// #include <gperftools/malloc_hook.h> // link tcmalloc
// #include "benchmark/benchmark.h" // link benchmark

// benchmark::IterationCount g_num_new      = 0;
// benchmark::IterationCount g_sum_size_new = 0;
// benchmark::IterationCount g_max_size_new = 0;
// benchmark::IterationCount g_min_size_new = -1;
// auto new_hook = [](const void*, size_t size){ ++g_num_new; g_sum_size_new +=
// size;
//                                               g_max_size_new =
//                                               std::max(g_max_size_new, size);
//                                               g_min_size_new =
//                                               std::min(g_min_size_new, size);
//                                               };
// #define BEFORE_TEST \
//   benchmark::IterationCount num_new      = g_num_new;\
//   benchmark::IterationCount sum_size_new = g_sum_size_new;\
//   g_max_size_new = 0;\
//   g_min_size_new = -1;\
//   MallocHook::AddNewHook( new_hook );

// #define AFTER_TEST \
//   MallocHook::RemoveNewHook( new_hook );\
//   auto iter = state.iterations();\
//   state.counters["#new"]      = (g_num_new      - num_new)      / iter;\
//   state.counters["sum_new_B"] = (g_sum_size_new - sum_size_new) / iter;\
//   state.counters["avg_new_B"] = (g_sum_size_new - sum_size_new) / (g_num_new - num_new);\
//   state.counters["max_new_B"] = g_max_size_new;\
//   if( ((benchmark::IterationCount)-1) != g_min_size_new ){\
//     state.counters["min_new_B"] = g_min_size_new;\
//   }

// static void BM_demo(benchmark::State& state) {
//   BEFORE_TEST
//   for (auto _ : state) {
//     void* ret1 = malloc(state.range(0));
//     void* ret2 = malloc(state.range(1));
//     void* ret3 = malloc(state.range(2));
//     free(ret1);
//     free(ret2);
//     free(ret3);
//   }
//   AFTER_TEST
// }
// BENCHMARK(BM_demo)->Args({32,128,32});
// BENCHMARK(BM_demo)->Args({320,640,960});
// BENCHMARK_MAIN();

// static void BM_StringCreation(benchmark::State &state)
// {
//   for (auto _ : state)
//     std::string empty_string;
// }
// // Register the function as a benchmark
// BENCHMARK(BM_StringCreation);

// // Define another benchmark
// static void BM_StringCopy(benchmark::State &state)
// {
//   std::string x = "hello";
//   for (auto _ : state)
//     std::string copy(x);
// }
// BENCHMARK(BM_StringCopy);

// int main(int argc, char **argv)
// {
//   char arg0_default[] = "benchmark";
//   char *args_default = arg0_default;
//   if (!argv)
//   {
//     argc = 1;
//     argv = &args_default;
//   }
//   ::benchmark::Initialize(&argc, argv);
//   if (::benchmark::ReportUnrecognizedArguments(argc, argv))
//     return 1;
//   ::benchmark::RunSpecifiedBenchmarks();
//   ::benchmark::Shutdown();
//   return 0;
// }