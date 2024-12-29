#include <gtest/gtest.h>
#include <filesystem>
#include <dlfcn.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace fs = std::filesystem;

#ifndef LIBIMPL1_PATH
#define LIBIMPL1_PATH ""
#endif

#ifndef LIBIMPL2_PATH
#define LIBIMPL2_PATH ""
#endif


const fs::path libimpl1_path = fs::absolute(fs::path(LIBIMPL1_PATH));
const fs::path libimpl2_path = fs::absolute(fs::path(LIBIMPL2_PATH));

TEST(TranslationTest_Impl1, BinaryTranslation) {
    void* handle = dlopen(libimpl1_path.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Не удалось загрузить " << libimpl1_path << ": " << dlerror();

    typedef char* (*translation_func)(long);
    translation_func translation_impl1 = (translation_func)dlsym(handle, "translation");
    ASSERT_NE(translation_impl1, nullptr) << "Не удалось найти функцию translation: " << dlerror();

    char* result = translation_impl1(10);
    EXPECT_STREQ(result, "1010");
    delete[] result;

    dlclose(handle);
}

TEST(SortTest_Impl1, BubbleSort) {
    void* handle = dlopen(libimpl1_path.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Не удалось загрузить " << libimpl1_path << ": " << dlerror();

    typedef int* (*sort_func)(int*, int);
    sort_func Sort_impl1 = (sort_func)dlsym(handle, "Sort");
    ASSERT_NE(Sort_impl1, nullptr) << "Не удалось найти функцию Sort: " << dlerror();

    int array[] = {5, 3, 2, 4, 1};
    int expected[] = {1, 2, 3, 4, 5};
    int* sorted = Sort_impl1(array, 5);
    for(size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(sorted[i], expected[i]);
    }

    dlclose(handle);
}

TEST(TranslationTest_Impl2, TernaryTranslation) {
    void* handle = dlopen(libimpl2_path.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Не удалось загрузить " << libimpl2_path << ": " << dlerror();

    typedef char* (*translation_func)(long);
    translation_func translation_impl2 = (translation_func)dlsym(handle, "translation");
    ASSERT_NE(translation_impl2, nullptr) << "Не удалось найти функцию translation: " << dlerror();

    char* result = translation_impl2(10);
    EXPECT_STREQ(result, "101");
    delete[] result;

    dlclose(handle);
}

TEST(SortTest_Impl2, QuickSort) {
    void* handle = dlopen(libimpl2_path.c_str(), RTLD_LAZY);
    ASSERT_NE(handle, nullptr) << "Не удалось загрузить " << libimpl2_path << ": " << dlerror();

    typedef int* (*sort_func)(int*, int);
    sort_func Sort_impl2 = (sort_func)dlsym(handle, "Sort");
    ASSERT_NE(Sort_impl2, nullptr) << "Не удалось найти функцию Sort: " << dlerror();

    int array[] = {5, 3, 2, 4, 1};
    int expected[] = {1, 2, 3, 4, 5};
    int* sorted = Sort_impl2(array, 5);
    for(size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(sorted[i], expected[i]);
    }

    dlclose(handle);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
