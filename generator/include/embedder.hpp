R"embedder(
#include <cstddef>

#if defined(__clang__) && !defined(__APPLE__)

    #define RESOURCE(name, path)                        \
    __asm__ (                                           \
        ".global " #name "\n"                           \
        ".global " #name "_size\n"                      \
        #name ":\n"                                     \
            ".incbin \"" path "\"\n"                    \
            ".align 8\n"                                \
        #name "_size:\n"                                \
            ".int " #name "_size - " #name " - 1\n"     \
            ".align 8\n"                                \
    )

#elif defined(__clang__) && defined(__APPLE__)

    #define RESOURCE(name, path)                        \
    __asm__ (                                           \
        ".global _" #name "\n"                          \
        ".global _" #name "_size\n"                     \
        "_" #name ":\n"                                 \
            ".incbin \"" path "\"\n"                    \
            ".align 8\n"                                \
        "_" #name "_size:\n"                            \
            ".int _" #name "_size - _" #name " - 1\n"   \
            ".align 8\n"                                \
    )


#elif defined(__GNUC__)

    #if defined(__linux__)
        #define RESOURCE(name, path)                    \
        __asm__ (                                       \
            ".section .rodata\n"                        \
            #name ":\n"                                 \
                ".incbin \"" path "\"\n"                \
                ".type " #name ", @object\n"            \
                ".size " #name "_size, 1\n"             \
            #name "_size:\n"                            \
                ".int " #name "_size - " #name " - 1\n" \
                ".align 8\n"                            \
        )
    #else
        #define RESOURCE(name, path)                    \
        __asm__ (                                       \
            ".section .rodata\n"                        \
            #name ":\n"                                 \
                ".incbin \"" path "\"\n"                \
                ".byte 0\n"                             \
            #name "_size:\n"                            \
                ".int " #name "_size - " #name " - 1\n" \
                ".align 8\n"                            \
        )
    #endif

#else
    #error "Unknown compiler!"
#endif

#define RESOURCE_EXPORT(name)       \
    extern "C" std::byte name[];    \
    extern "C" int name##_size;
)embedder";