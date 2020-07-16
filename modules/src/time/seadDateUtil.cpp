#include <time/seadCalendarSpan.h>
#include <time/seadDateUtil.h>

namespace sead
{
namespace DateUtil
{
bool isLeapYear(u32 year)
{
#ifdef NON_MATCHING
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
#else
    bool div100, div4;
    return (div100 = year % 100 == 0, div4 = year % 4 == 0, !div100 & div4) | (year % 400 == 0);
#endif
}

CalendarTime::Week calcWeekDay(const CalendarTime::Year& year, const CalendarTime::Month& month,
                               const CalendarTime::Day& day)
{
    int y = year.getValue();
    int m = month.getValueOneOrigin();
    int d = day.getValue();
    if (m < 3)
    {
        y -= 1;
        m += 12;
    }
    d += y + (y / 4) - (y / 100) + (y / 400);
    d += (26 * m + 16) / 10;
    return CalendarTime::Week(d % 7);
}

void calcSecondToCalendarSpan(CalendarSpan* out_span, u64 sec)
{
    if (!out_span)
        return;
    out_span->setDays(sec / (3600 * 24));
    out_span->setHours((sec % (3600 * 24)) / 3600);
    out_span->setMinutes((sec % 3600) / 60);
    out_span->setSeconds(sec % 60);
}

bool parseW3CDTFString(CalendarTime* out_time, CalendarSpan* out_span, const SafeString& string) {}
}  // namespace DateUtil
}  // namespace sead
