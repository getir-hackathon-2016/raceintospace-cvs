#ifndef _LOGGING_H
#define _LOGGING_H

#define LOG_ROOT_CAT  baris
#include "log4c.h"

LOG_EXPORT_CATEGORY(LOG_ROOT_CAT);
LOG_EXPORT_CATEGORY(sdl);
LOG_EXPORT_CATEGORY(mission);
LOG_EXPORT_CATEGORY(utils);
LOG_EXPORT_CATEGORY(filesys);
LOG_EXPORT_CATEGORY(config);
LOG_EXPORT_CATEGORY(multimedia);
LOG_EXPORT_CATEGORY(audio);
LOG_EXPORT_CATEGORY(music);
LOG_EXPORT_CATEGORY(video);

#endif /* _LOGGING_H */