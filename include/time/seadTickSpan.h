#ifndef SEAD_TICKSPAN_H_
#define SEAD_TICKSPAN_H_

#include <basis/seadTypes.h>

namespace sead {

class TickSpan
{
public:
    TickSpan(s64 span=0)
        : mSpan(span)
    {
    }

    s64 mSpan;
};

} // namespace sead

#endif // SEAD_TICKSPAN_H_
