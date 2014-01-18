#if !defined(UTSDEFS_HPP_INCLUDED)
#define UTSDEFS_HPP_INCLUDED

/*
  You should always define one of these symbols when building or using
  the Unit Test System, unless you are statically linking with its
  object files:

  UTS_SHARED_IMPL		Building the shared implementation of UTS
  UTS_SHARED			Using a shared implementatin of UTS

  Under Win32, "shared" implies that UTS is being used/built as a DLL.

  The following preprocessor symbols will be defined:
  UTS_EXPORT    Use this to mark decls of shareable functions & variables.
*/ 

#if defined(WIN32)
#  if defined(UTS_SHARED_IMPL)
#    define UTS_EXPORT __declspec(dllexport)
#  elif defined(UTS_SHARED)
#    define UTS_EXPORT __declspec(dllimport)
#  else
#    define UTS_EXPORT
#  endif
#else
// Do we need to anything special here for shared libraries in UNIX systems?
#  define UTS_EXPORT
#endif


#endif

