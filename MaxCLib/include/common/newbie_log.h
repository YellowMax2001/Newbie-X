#ifndef _NEWBIE_LOG_H_
#define _NEWBIE_LOG_H_

#include <string>

#include <stdio.h>

#include "basic_type.h"

#define nb_loge printf
#define nb_logw printf
#define nb_logd printf
#define nb_logv(...)

#define GRAPHIC_LOG_GRP "GraphicModule"

#define NEWBIE_ERROR(group, format, ...) \
    printf("\033[0;32;31m" "%s\t" "[NewbieError]:\t" "%s " "%d " format "\033[m\n", \
    group, __FILE__, __LINE__, ##__VA_ARGS__)

#define NEWBIE_INFO(group, format, ...) \
    printf("\033[0;32;32m" "%s\t" "[NewbieInfo]:\t" "%s " "%d " format "\033[m\n", \
    group, __FILE__, __LINE__, ##__VA_ARGS__)


#define RetSuccess      0
#define ErrorNoMemory   0x8000

struct ErrorWarnStringMap {
    INT32 error;
    std::string errorString;
};

ErrorWarnStringMap NewbieErrorWarnStringMap[] = {
    {ErrorNoMemory, "No memory avalible"},
};

#endif // End of _NEWBIE_LOG_H_