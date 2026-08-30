#include <cstdlib>
#include <new>

void* operator new(size_t size) { void* p = std::malloc(size); if (!p) std::abort(); return p; }
void* operator new[](size_t size) { void* p = std::malloc(size); if (!p) std::abort(); return p; }
void operator delete(void* ptr) noexcept { std::free(ptr); }
void operator delete[](void* ptr) noexcept { std::free(ptr); }
void operator delete(void* ptr, size_t) noexcept { std::free(ptr); }
void operator delete[](void* ptr, size_t) noexcept { std::free(ptr); }
