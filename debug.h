#if defined(DEBUG)
#define logInfo(...) { printf( __VA_ARGS__ ); printf("\n"); }                       
#define logWarning(...) { printf( "WRN " __VA_ARGS__ ); printf("\n"); }                
#define logError(...) { printf( "ERR " __VA_ARGS__ ); printf("\n"); }
#else
#define logInfo(...) { }                       
#define logWarning(...) { }                
#define logError(...) { }
#endif
