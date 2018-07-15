#if defined( _WIN64 ) || defined( _WIN32 )
#else
// POSIX
typedef SOCKET int;
#endif
