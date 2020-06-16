#include <basis/seadRawPrint.h>
#include <mc/seadCoreInfo.h>

namespace sead
{
SEAD_ENUM_IMPL(CoreId)

u32 CoreInfo::sNumCores = 1;
u32 CoreInfo::sPlatformCoreId[32]{};
CoreId CoreInfo::sCoreIdFromPlatformCoreIdTable[32]{};
#ifdef SWITCH
nn::os::TlsSlot CoreInfo::sCoreNumberTlsSlot{};
#endif

namespace
{
// Force a static constructor to be emitted to initialize CoreInfo.
struct CoreInfoInitializer
{
    CoreInfoInitializer() { CoreInfo::configure(); }
};
// IMPORTANT: this must be located after sCoreIdFromPlatformCoreIdTable to get the correct
// initialization order.
static CoreInfoInitializer sInitializer;
}  // namespace

u32 CoreIdMask::countOnBits() const
{
    u32 x = mMask;
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x07070707) + ((x >> 4) & 0x07070707);
    x = (x & 0x000F000F) + ((x >> 8) & 0x000F000F);
    x = (x & 0x1F) + (x >> 16);
    return x;
}

void CoreInfo::configure()
{
#ifdef SWITCH
    sNumCores = 3;
    sPlatformCoreId[0] = 0;
    sPlatformCoreId[1] = 1;
    sPlatformCoreId[2] = 2;

    SEAD_ASSERT_NOFMT(nn::os::GetCurrentCoreNumber() == 0);
    const auto alloc_result = nn::os::AllocateTlsSlot(&sCoreNumberTlsSlot, nullptr);
    SEAD_ASSERT_NOFMT(alloc_result.IsSuccess());

    for (u32 i = 0; i != sNumCores; ++i)
    {
        const u32 id = sPlatformCoreId[i];
        sCoreIdFromPlatformCoreIdTable[id] = i;
    }
#else
#error "Unknown platform"
#endif
}

void CoreInfo::dump()
{
    system::Print("* num cores %d\n", sNumCores);
    for (u32 i = 0; i < sNumCores; ++i)
    {
        system::Print("  [%d] : %s : PlatformCoreId=%d\n", i, i == 0 ? "Main" : "Sub ",
                      sPlatformCoreId[i]);
    }
    system::Print("all mask : %p\n", u32(getMaskAll()));
    system::Print("all sub mask : %p\n", u32(getMaskSubAll()));
}

u32 CoreInfo::getPlatformMask(CoreId id)
{
    return 1 << getPlatformCoreId(id);
}

}  // namespace sead
