#pragma once

#if defined(__cplusplus) && __has_include(<array>)
#else
#error This project requires C++11 or greater
#endif

extern "C" {
    void c_CKSimDriver();
}  // extern "C"