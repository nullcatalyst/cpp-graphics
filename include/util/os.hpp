#ifndef util_os_hpp
#define util_os_hpp

#ifdef __APPLE__
    #include <TargetConditionals.h>

    #if TARGET_OS_IOS
        /// TODO: Add more information regarding simulator vs device
        #define OS_ios 1
    #endif

    #if TARGET_OS_OSX
        #define OS_mac 1
    #endif
#endif

// Example usage:
//      #if OS(mac)
//      #if OS(ios)
#define OS(test) OS_##test

#endif /* util_os_hpp */
