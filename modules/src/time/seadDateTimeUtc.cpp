#ifdef SWITCH
#include <nn/time.h>
#else
#include <chrono>
#include <ctime>
#endif

#include "basis/seadRawPrint.h"
#include "time/seadDateTime.h"
#include "time/seadDateUtil.h"

namespace sead
{
DateTimeUtc::DateTimeUtc(u64 unix_time)
{
    mUnixTime = unix_time;
}

DateTimeUtc::DateTimeUtc(const DateTime& date_time)
{
    CalendarTime time;
    date_time.getCalendarTime(&time);

#ifdef SWITCH
    // Note: time sysmodule is not initialised here.
    nn::time::CalendarTime nn_time;
    nn_time.year = time.getYear();
    nn_time.month = time.getMonth().getValueOneOrigin();
    nn_time.day = time.getDay();
    nn_time.hour = time.getHour();
    nn_time.minute = time.getMinute();
    nn_time.second = time.getSecond();
    nn::time::PosixTime posix_time;
    int dummy = 0;
    nn::time::ToPosixTime(&dummy, &posix_time, 1, nn_time);
    mUnixTime = posix_time.time;
#else
    std::tm tm{};
    tm.tm_sec = time.getSecond();
    tm.tm_min = time.getMinute();
    tm.tm_hour = time.getHour();
    tm.tm_mday = time.getDay();
    tm.tm_mon = time.getMonth().getValueOneOrigin();
    tm.tm_year = time.getYear();
    tm.tm_isdst = 0;
    mUnixTime = std::mktime(&tm);
#endif
}

DateTimeUtc::DateTimeUtc(const CalendarTime& time)
{
    setUnixTime(time);
}

DateTimeUtc::DateTimeUtc(const CalendarTime::Year& year, const CalendarTime::Month& month,
                         const CalendarTime::Day& day, const CalendarTime::Hour& hour,
                         const CalendarTime::Minute& minute, const CalendarTime::Second& second)
{
    setUnixTime(year, month, day, hour, minute, second);
}

u64 DateTimeUtc::setNow()
{
#ifdef SWITCH
    DateTime::initializeSystemTimeModule();
    nn::time::PosixTime now;
    nn::time::StandardUserSystemClock::GetCurrentTime(&now);
    mUnixTime = now.time;
#else
    const auto now = std::chrono::system_clock::now();
    mUnixTime = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
#endif
    return mUnixTime;
}

u64 DateTimeUtc::setUnixTime(const CalendarTime& time)
{
#ifdef SWITCH
    DateTime::initializeSystemTimeModule();
    nn::time::CalendarTime nn_time;
    nn_time.year = time.getYear();
    nn_time.month = time.getMonth().getValueOneOrigin();
    nn_time.day = time.getDay();
    nn_time.hour = time.getHour();
    nn_time.minute = time.getMinute();
    nn_time.second = time.getSecond();
    mUnixTime = nn::time::ToPosixTimeFromUtc(nn_time).time;
#else
#ifdef _WIN32
#define timegm _mkgmtime
#endif
    std::tm tm{};
    tm.tm_sec = time.getSecond();
    tm.tm_min = time.getMinute();
    tm.tm_hour = time.getHour();
    tm.tm_mday = time.getDay();
    tm.tm_mon = time.getMonth().getValueOneOrigin();
    tm.tm_year = time.getYear();
    tm.tm_isdst = 0;
    mUnixTime = timegm(&tm);
#ifdef _WIN32
#undef timegm
#endif
#endif
    return mUnixTime;
}

u64 DateTimeUtc::setUnixTime(const CalendarTime::Year& year, const CalendarTime::Month& month,
                             const CalendarTime::Day& day, const CalendarTime::Hour& hour,
                             const CalendarTime::Minute& minute, const CalendarTime::Second& second)
{
    DateTime::initializeSystemTimeModule();
    CalendarTime time(year, month, day, hour, minute, second);
#ifdef SWITCH
    nn::time::CalendarTime nn_time;
    nn_time.year = time.getYear();
    nn_time.month = time.getMonth().getValueOneOrigin();
    nn_time.day = time.getDay();
    nn_time.hour = time.getHour();
    nn_time.minute = time.getMinute();
    nn_time.second = time.getSecond();
    mUnixTime = nn::time::ToPosixTimeFromUtc(nn_time).time;
#else
#ifdef _WIN32
#define timegm _mkgmtime
#endif
    std::tm tm{};
    tm.tm_sec = time.getSecond();
    tm.tm_min = time.getMinute();
    tm.tm_hour = time.getHour();
    tm.tm_mday = time.getDay();
    tm.tm_mon = time.getMonth().getValueOneOrigin();
    tm.tm_year = time.getYear();
    tm.tm_isdst = 0;
    mUnixTime = timegm(&tm);
#ifdef _WIN32
#undef timegm
#endif
#endif
    return mUnixTime;
}

void DateTimeUtc::getCalendarTime(CalendarTime* out) const
{
#ifdef SWITCH
    DateTime::initializeSystemTimeModule();
    const nn::time::CalendarTime ctime = nn::time::ToCalendarTimeInUtc({mUnixTime});

    CalendarTime::Date date(ctime.year, CalendarTime::Month::makeFromValueOneOrigin(ctime.month),
                            ctime.day);
    CalendarTime::Time time(ctime.hour, ctime.minute, ctime.second);
#else
    std::tm tm{};

    const std::time_t posix_time = mUnixTime;
    auto* ret_gmtime = gmtime_r(&posix_time, &tm);
    SEAD_ASSERT(ret_gmtime != nullptr);

    CalendarTime::Date date(tm.tm_year, CalendarTime::Month::makeFromValueOneOrigin(tm.tm_mon),
                            tm.tm_mday);
    CalendarTime::Time time(tm.tm_hour, tm.tm_min, tm.tm_sec);
#endif

    out->setDate(date);
    out->setTime(time);
}

DateSpan DateTimeUtc::diff(DateTimeUtc time) const
{
    return DateSpan(mUnixTime - time.mUnixTime);
}

DateSpan DateTimeUtc::diffToNow() const
{
    DateTimeUtc now(0);
    now.setNow();
    return now.diff(*this);
}

DateSpan operator-(DateTimeUtc lhs, DateTimeUtc rhs)
{
    return DateSpan(lhs.getUnixTime() - rhs.getUnixTime());
}

DateTimeUtc operator-(DateTimeUtc time, DateSpan span)
{
    return DateTimeUtc(time.getUnixTime() - span.getSpan());
}

DateTimeUtc operator+(DateTimeUtc time, DateSpan span)
{
    return DateTimeUtc(time.getUnixTime() + span.getSpan());
}
}  // namespace sead
