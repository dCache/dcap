
#define VER(v)  v " " __DATE__ " " __TIME__ ;

#include "dcap.h"
#include "debug_level.h"

const int dc_getProtocol()
{
	return DCAP_VER_PROTOCOL;
}

const int dc_getMajor()
{
	return DCAP_VER_MAJOR;
}

const int dc_getMinor()
{
	return DCAP_VER_MINOR;
}

const char *dc_getPatch()
{
	return DCAP_VER_PATCH;
}

/*depricated method please use above methods*/
const char *getDcapVersion()
{
	return VER( PACKAGE_VERSION );
}
