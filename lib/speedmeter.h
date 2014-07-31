#pragma once

#ifdef LIBSPEEDMETER_EXPORTS

#ifdef _WINDOWS
#define SpeedMeterDllExport   __declspec( dllexport )
#pragma warning (disable :4251)
#else
#define SpeedMeterDllExport
#endif

#else
#define SpeedMeterDllExport
#endif
