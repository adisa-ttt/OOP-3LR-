#include <benchmark/benchmark.h>
#include <string>
#include "Stack.h"
#include "Queue.h"
#include "Array.h"
#include "avlTree.h"
#include "LinkedList.h"
#include "DoubleLinkedList.h"
#include "ChainHash.h"
#include "CuckooHash.h"

using namespace std;

static void Array_push_back(benchmark::State& state) {
    for (auto _ : state) {
        Array a;
        for (int i = 0; i < state.range(0); ++i) a.push_back("item");
    benchmark::DoNotOptimize(a);
    }
    
}
BENCHMARK(Array_push_back)->RangeMultiplier(10)->Range(100, 10000);


static void Array_insert_at(benchmark::State& state) { 
    int n = state.range(0);
    for (auto _ : state) {
        Array a;
        state.PauseTiming();
        for (int i = 0; i < n; ++i) a.push_back("item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            a.insert(0, "new");
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(Array_insert_at)->RangeMultiplier(10)->Range(100, 10000);


static void Array_removeAt(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        Array a;
        state.PauseTiming();
        for (int i = 0; i < n; ++i) a.push_back("item");
        state.ResumeTiming();
        for(int i = 0; i < n; ++i) {
            a.removeAt(0);
        }
        benchmark::DoNotOptimize(a);
    }
}
BENCHMARK(Array_removeAt)->RangeMultiplier(10)->Range(100, 10000);


static void Array_getAt(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        Array a;
        state.PauseTiming();
        for (int i = 0; i < n;++i) a.push_back("item" + to_string(i));
        state.ResumeTiming();
        string found;
        for (int i = 0; i < n; ++i) found = a.getAt(i);
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(Array_getAt)->RangeMultiplier(10)->Range(100, 10000);

static void Array_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        Array a;
        state.PauseTiming();
        for (int i = 0;i < n; ++i) a.push_back("item " + to_string(i));
        string target = "item " + to_string(n-1);
        bool found = false;
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            if (a.getAt(i) == target) {
                found = true;
                break;
            }
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(Array_find)->RangeMultiplier(10)->Range(100, 10000);

//---------------------------------------------------

static void Stack_push(benchmark::State& state) {
for (auto st : state) {
        Stack s;
        for (int i = 0; i < state.range(0); ++i) {
            s.push("item");
        }
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(Stack_push)->RangeMultiplier(10)->Range(100, 10000);



static void Stack_pop(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Stack s;
        for (int i = 0; i < n; ++i) s.push("item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(s.pop()); }
    } 
}
BENCHMARK(Stack_pop)->RangeMultiplier(10)->Range(100, 10000);

static void Stack_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Stack s;
        for (int i = 0; i < n; ++i) s.push("item " + to_string(i));
        string target = "item ";
        bool found = false;
        state.ResumeTiming();
        for (const string& str : s.get()) {
            if (str == target) {
                found = true;
                break; }
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(Stack_find)->RangeMultiplier(10)->Range(100, 10000);

//----------------------------------------------------

static void Queue_push(benchmark::State& state) {
    for (auto _ : state) {
        Queue q;
        for (int i = 0; i < state.range(0); ++i) {
            q.push("item");
        }
        benchmark::DoNotOptimize(q);
    }
}
BENCHMARK(Queue_push)->RangeMultiplier(10)->Range(10, 10000);

static void Queue_pop(benchmark::State& state) {
    int n = state.range();
    for (auto _: state) {
        Queue q;
        state.PauseTiming();
        for (int i = 0; i < n; ++i) q.push("item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(q.pop()); }
    }
}
BENCHMARK(Queue_pop)->RangeMultiplier(10)->Range(100, 10000);

static void Queue_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        Queue q;
        for (int i = 0; i < n; ++i) q.push("item_" + to_string(i));
        string target = "item_" + to_string(n - 1); // худший случай — в конце очереди
        state.ResumeTiming();
        bool found = false;
        for (const string& v : q.get()) {
            if (v == target) { found = true; break; }
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(Queue_find)->RangeMultiplier(10)->Range(100, 10000);

//--------------------------------------------------------

static void list_push_front(benchmark::State& state) {
    for (auto _ : state) {
        LinkList l;
        for (int i = 0; i < state.range(0); ++i) {
            l.push_front("item_" + to_string(i));
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(list_push_front)->RangeMultiplier(10)->Range(100, 10000);

static void list_push_back(benchmark::State& state) {
    for (auto _ : state) {
        LinkList l;
        for (int i = 0; i < state.range(0); ++i) {
            l.push_back("item_" + to_string(i));
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(list_push_back)->RangeMultiplier(10)->Range(100, 10000);

static void list_pop_front(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        LinkList l;
        for (int i = 0; i < n; ++i) l.push_back("item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(l.pop_front());
        }
    }
}
BENCHMARK(list_pop_front)->RangeMultiplier(10)->Range(100, 10000);

static void list_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        LinkList l;
        for (int i = 0; i < n; ++i) l.push_back("item_" + to_string(i));
        string target = "item_" + to_string(n - 1);  // худший случай — в конце
        state.ResumeTiming();
        benchmark::DoNotOptimize(l.find(target));
    }
}
BENCHMARK(list_find)->RangeMultiplier(10)->Range(100, 10000);

static void list_remove(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        LinkList l;
        for (int i = 0; i < n; ++i) l.push_back("item_" + to_string(i));
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(l.removeValue("item_" + to_string(i)));
        }
    }
}
BENCHMARK(list_remove)->RangeMultiplier(10)->Range(100, 10000);

//----------------------------------------------------------

static void double_list_push_front(benchmark::State& state) {
    for (auto _ : state) {
        doubleLinkList l;
        for (int i = 0; i < state.range(0); ++i) {
            l.push_front("item_" + to_string(i));
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(double_list_push_front)->RangeMultiplier(10)->Range(100, 10000);

static void double_list_push_back(benchmark::State& state) {
    for (auto _ : state) {
        doubleLinkList l;
        for (int i = 0; i < state.range(0); ++i) {
            l.push_back("item_" + to_string(i));
        }
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(double_list_push_back)->RangeMultiplier(10)->Range(100, 10000);

static void double_list_pop_front(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        doubleLinkList l;
        for (int i = 0; i < n; ++i) l.push_back("item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(l.pop_front());
        }
    }
}
BENCHMARK(double_list_pop_front)->RangeMultiplier(10)->Range(100, 10000);

static void double_list_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        doubleLinkList l;
        for (int i = 0; i < n; ++i) l.push_back("item_" + to_string(i));
        string target = "item_" + to_string(n - 1);
        state.ResumeTiming();
        benchmark::DoNotOptimize(l.find(target));
    }
}
BENCHMARK(double_list_find)->RangeMultiplier(10)->Range(100, 10000);

//---------------------------------------------------------------------

static void avlTree_insert(benchmark::State& state) {
    for (auto _ : state) {
        avlTree t;
        for (int i= 0; i < state.range(0); ++i) {
            t.insert(i); }
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(avlTree_insert)->RangeMultiplier(10)->Range(100, 10000);

static void avlTree_remove(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        avlTree t;
        for (int i = 0; i < n; ++i) {
            t.insert(i); }
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(t.remove(i));
        }
    }
}
BENCHMARK(avlTree_remove)->RangeMultiplier(10)->Range(100, 10000);


static void avlTree_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        avlTree t;
        for (int i = 0; i < n; ++i) t.insert(i);
        state.ResumeTiming();
        bool found = false;
        for (int i = 0; i < n; ++i) {
            found = t.find(i);
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(avlTree_find)->RangeMultiplier(10)->Range(100, 10000);

//-----------------------------------------------------------------------

static void chain_insert(benchmark::State& state) {
    for (auto _ : state) {
        ChainTable t(128);  // начальный размер 128 корзин
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(
                t.ChainInsert(i, "item_" + to_string(i))
            );
        }
    }
}
BENCHMARK(chain_insert)->RangeMultiplier(10)->Range(100, 10000);

static void chain_remove(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        ChainTable t(128);
        for (int i = 0; i < n; ++i) t.ChainInsert(i, "item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(t.chainRemove(i));
        }
    }
}
BENCHMARK(chain_remove)->RangeMultiplier(10)->Range(100, 10000);

static void chain_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        ChainTable t(128);
        for (int i = 0; i < n; ++i) t.ChainInsert(i, "item");
        state.ResumeTiming();
        bool found = false;
        for (int i = 0; i < n; ++i) {
            found = t.chainFind(i);
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(chain_find)->RangeMultiplier(10)->Range(100, 10000);

//----------------------------------------------------------------

static void cuckoo_insert(benchmark::State& state) {
    for (auto _ : state) {
        CuckooTable t(128);
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(
                t.CuckooInsert(i, "item_" + to_string(i))
            );
        }
    }
}
BENCHMARK(cuckoo_insert)->RangeMultiplier(10)->Range(100, 10000);

static void cuckoo_remove(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        CuckooTable t(128);
        for (int i = 0; i < n; ++i) t.CuckooInsert(i, "item");
        state.ResumeTiming();
        for (int i = 0; i < n; ++i) {
            benchmark::DoNotOptimize(t.CuckooRemove(i));
        }
    }
}
BENCHMARK(cuckoo_remove)->RangeMultiplier(10)->Range(100, 10000);

static void cuckoo_find(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        CuckooTable t(128);
        for (int i = 0; i < n; ++i) t.CuckooInsert(i, "item");
        state.ResumeTiming();
        bool found = false;
        for (int i = 0; i < n; ++i) {
            found = t.CuckooFind(i);
        }
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(cuckoo_find)->RangeMultiplier(10)->Range(100, 10000);

BENCHMARK_MAIN();

/*
cd build
cmake ..
make benchmarks
./benchmarks
*/