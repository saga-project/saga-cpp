#if _MSC_VER < 1400

// VC7.1
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Include

#elif _MSC_VER < 1500

// VC8.0 and up
#if PLATFORM_IS_X64 == 1

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 8\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft Visual Studio 8\VC\PlatformSDK\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 8\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft Visual Studio 8\VC\PlatformSDK\Include
#endif

#else

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 8\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 8\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include
#endif

#endif  // PLATFORM_IS_X64 == 1

#elif _MSC_VER < 1600

// VC9.0 and up
#if PLATFORM_IS_X64 == 1

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\PlatformSDK\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\PlatformSDK\Include
#endif

#else

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 9.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft Visual Studio 9.0\VC\PlatformSDK\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 9.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft Visual Studio 9.0\VC\PlatformSDK\Include
#endif

#endif  // PLATFORM_IS_X64 == 1

#else

// VS2010 and up
#if PLATFORM_IS_X64 == 1

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include
#endif

#else

#if defined(WIN64)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 10.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include
#elif defined(WIN32)
#define _DOWNHILL_INCLUDE_DIR <C:\Program Files\Microsoft Visual Studio 10.0\VC\include
#define _DOWNHILL_INCLUDE_DIR2 <C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include
#endif

#endif  // PLATFORM_IS_X64 == 1

#endif  // _MSC_VER < 1400
