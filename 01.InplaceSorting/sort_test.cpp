#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "sorting.h"

// Тест для пузырьковой сортировки
TEST(BubbleSortTest, SortsRandomArray) {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    std::vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    bubble_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(BubbleSortTest, SortsWithDuplicates) {
    std::vector<int> arr = {5, 2, 8, 2, 9, 1, 5};
    std::vector<int> expected = {1, 2, 2, 5, 5, 8, 9};
    
    bubble_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(BubbleSortTest, SortsWithNegatives) {
    std::vector<int> arr = {-5, 12, -8, 0, -3, 7, -1};
    std::vector<int> expected = {-8, -5, -3, -1, 0, 7, 12};
    
    bubble_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(BubbleSortTest, HandlesEmptyArray) {
    std::vector<int> arr;
    
    bubble_sort(arr.begin(), arr.end());
    
    EXPECT_TRUE(arr.empty());
}

TEST(BubbleSortTest, HandlesSingleElement) {
    std::vector<int> arr = {42};
    
    bubble_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr[0], 42);
}

// Тест для быстрой сортировки
TEST(QuickSortTest, SortsRandomArray) {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    std::vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    quick_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(QuickSortTest, SortsWithDuplicates) {
    std::vector<int> arr = {5, 2, 8, 2, 9, 1, 5};
    std::vector<int> expected = {1, 2, 2, 5, 5, 8, 9};
    
    quick_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(QuickSortTest, SortsWithNegatives) {
    std::vector<int> arr = {-5, 12, -8, 0, -3, 7, -1};
    std::vector<int> expected = {-8, -5, -3, -1, 0, 7, 12};
    
    quick_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr, expected);
}

TEST(QuickSortTest, HandlesEmptyArray) {
    std::vector<int> arr;
    
    quick_sort(arr.begin(), arr.end());
    
    EXPECT_TRUE(arr.empty());
}

TEST(QuickSortTest, HandlesSingleElement) {
    std::vector<int> arr = {42};
    
    quick_sort(arr.begin(), arr.end());
    
    EXPECT_EQ(arr[0], 42);
}