#pragma once

#include <cstdarg>
#include <sead/types.h>

#ifndef SEAD_DEBUG
#define SEAD_ASSERT(condition, message, ...)                                                       \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
            sead::system::HaltWithDetail(__FILE__, __LINE__, message, ##__VA_ARGS__);              \
    } while (0)
#define SEAD_ASSERT_NOFMT(condition)                                                               \
    do                                                                                             \
    {                                                                                              \
        if (!(condition))                                                                          \
            sead::system::HaltWithDetailNoFormat(__FILE__, __LINE__, #condition);                  \
    } while (0)
#define SEAD_WARN(message, ...)                                                                    \
    do                                                                                             \
        sead::system::Warning(__FILE__, __LINE__, message, ##__VA_ARGS__);                         \
    while (0)
#else
#define SEAD_ASSERT(condition, message, ...)
#define SEAD_ASSERT_NOFMT(condition)
#define SEAD_WARN(message, ...)
#endif

namespace sead
{
class Heap;

namespace system
{
void Halt();
void HaltWithDetail(const char* file, int line, const char* msg, ...);
void HaltWithDetailNoFormat(const char* file, int line, const char* msg);
void DebugBreak();

void Print(const char* format, ...);
void PrintV(const char* format, std::va_list);
void PrintString(const char* format, s32);

void Warning(const char* file, int line, const char* msg, ...);
void SetWarningEnable(bool enable);
}  // namespace system

}  // namespace sead
