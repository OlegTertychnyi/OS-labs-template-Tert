#ifndef LIBRARY_H
#define LIBRARY_H

#ifdef _WIN32
    #ifdef LIBRARY_EXPORTS
        #define LIBRARY_API __declspec(dllexport)
    #else
        #define LIBRARY_API __declspec(dllimport)
    #endif
#else
    #define LIBRARY_API
#endif

extern "C" {
    LIBRARY_API char* translation(long x);

    LIBRARY_API int* Sort(int* array, int size);
}

#endif // LIBRARY_H
