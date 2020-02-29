/*
@file: log.h
Copyright (C) 2020 by Alejandro Vicario, Xiaoyu Wang and chipPLAY contributors.
This file is part of the chipPLAY project.
ChipPLAY is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ChipPLAY is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with ChipPlay.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>

#ifndef LOG_ENABLED
#define LOG_ENABLED 0
#endif

#ifndef LOG_MODULE_NAME
#define LOG_MODULE_NAME "app"
#endif

#define LOG_LEVEL_DEBUG_NAME "debug"
#define LOG_LEVEL_INFO_NAME "info"
#define LOG_LEVEL_WARN_NAME "warn"
#define LOG_LEVEL_ERROR_NAME "error"

#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 1

#ifndef LOG_LEVEL_ENABLED
#define LOG_LEVEL_ENABLED LOG_LEVEL_ERROR
#endif

#if LOG_ENABLED
#define __LOG_INTERNAL(level, module, fmt, ...) \
	printf("<" level ">\t" module ": " fmt "\n\r", ##__VA_ARGS__)
#else
#define __LOG_INTERNAL(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_DEBUG_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_INFO_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_WARN_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL_ENABLED >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) \
	__LOG_INTERNAL(LOG_LEVEL_ERROR_NAME, LOG_MODULE_NAME, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#endif // __LOG_H
