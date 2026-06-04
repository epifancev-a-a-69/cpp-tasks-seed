#ifndef SORTING_H
#define SORTING_H

#include "collvalue.h"
#include <iterator>
#include <algorithm>
#include <vector>

// ----------------------------- Пузырёк O(N²) --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end) return;
    
    for (Iterator i = begin; i != end; ++i) {
        bool swapped = false;
        Iterator j = begin;
        Iterator next = begin;
        ++next;
        
        while (next != end) {
            if (*next < *j) {
                std::swap(*j, *next);
                swapped = true;
            }
            ++j;
            ++next;
        }
        
        if (!swapped) break;
    }
}

// ----------------------------- Быстрая сортировка O(N log N) -----------------------
template <typename Iterator>
Iterator partition(Iterator begin, Iterator end)
{
    auto pivot = *std::prev(end);
    Iterator i = begin;
    
    for (Iterator j = begin; j != std::prev(end); ++j) {
        if (*j <= pivot) {
            std::swap(*i, *j);
            ++i;
        }
    }
    std::swap(*i, *std::prev(end));
    return i;
}

template <typename Iterator>
void quick_sort_helper(Iterator begin, Iterator end)
{
    if (begin == end || std::next(begin) == end) return;
    
    Iterator pivot_it = partition(begin, end);
    quick_sort_helper(begin, pivot_it);
    quick_sort_helper(std::next(pivot_it), end);
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (begin == end) return;
    quick_sort_helper(begin, end);
}

#endif // SORTING_H