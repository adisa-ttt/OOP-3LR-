#define BOOST_TEST_MODULE LR3_OOP
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


/*
cd ..
cd ~/PROGA/3LR/c++
cmake -B build && cmake --build build
./build/boost_tests
./build/gtest_tests
./build/catch2_tests  

cd ~/PROGA/3LR/c++/build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ..
make -j$(nproc)

./boost_tests --run_test=cuckoo_test_roadtrip



gcovr --root .. --object-directory . --html --html-details --output coverage_report/tests.html 

--exclude '.*benchmarks.*' --exclude '.*tests.*' 


xdg-open coverage_report/tests.html


--exclude '.*tests.*'
--exclude '.*benchmarks.*'
*/