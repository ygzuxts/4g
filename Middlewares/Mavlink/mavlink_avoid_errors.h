#ifndef MAVLINK_AVOID_ERRORS_H
#define MAVLINK_AVOID_ERRORS_H
#include "stdio.h"
#include "stdint.h"
/*解决..\MAVLINK\common\../mavlink_types.h(53): error:  #20: identifier "pack" is undefined*/
#define MAVPACKED( __Declaration__ )  __Declaration__ 
/*解决..\MAVLINK\common\../mavlink_types.h(53): error:  #3092: anonymous unions are only supported in --gnu mode, or when enabled with #pragma anon_unions*/
#pragma anon_unions
#define inline __inline
#include "mavlink_types.h"
#include "mavlink.h"
#include "mavlink_helpers.h"

#endif //AVLINK_AVOID_ERRORS_H
