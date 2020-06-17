#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.hpp>
#include <time/seadCalendarTime.h>

namespace sead
{
class CalendarSpan;

class DateUtil
{
public:
    static bool isLeapYear(u32 year);

    static CalendarTime::Week calcWeekDay(const CalendarTime::Year&, const CalendarTime::Month&,
                                          const CalendarTime::Day&);

    static void calcSecondToCalendarSpan(CalendarSpan* out_span, u64 seconds);

    static bool parseW3CDTFString(CalendarTime* out_time, CalendarSpan* out_span,
                                  const SafeString& string);
};
}  // namespace sead
