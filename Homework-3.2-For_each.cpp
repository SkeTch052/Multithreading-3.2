#include <future>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

template <typename Iterator, typename Func>
void parallel_for_each(Iterator begin, Iterator end, Func func) {
    int size = std::distance(begin, end);

    if (size <= 20) {
        std::for_each(begin, end, func);
    }
    else {
        Iterator mid = begin;
        std::advance(mid, size / 2);

        std::future<void> first_half = std::async(std::launch::async, parallel_for_each<Iterator, Func>, begin, mid, func);
        std::future<void> second_half = std::async(std::launch::async, parallel_for_each<Iterator, Func>, mid, end, func);

        first_half.get();
        second_half.get();
    }
}

int main() {
    std::vector<int> vec(100, 2);

    std::cout << "Before:\t";
    for (auto num : vec) {
        std::cout << num;
    }
    std::cout << std::endl;

    parallel_for_each(vec.begin(), vec.end(), [](int& n) { n *= 4; });

    std::cout << "\nAfter:\t";
    for (auto num : vec) {
        std::cout << num;
    }
    std::cout << std::endl;

    return 0;
}