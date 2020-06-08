#ifndef SEAD_PATH_H_
#define SEAD_PATH_H_

#include <sead/seadSafeString.hpp>

namespace sead
{
namespace Path
{
bool getDriveName(BufferedSafeString* driveName, const SafeString& path);
void getPathExceptDrive(BufferedSafeString* pathNoDrive, const SafeString& path);

}  // namespace Path
}  // namespace sead

#endif  // SEAD_PATH_H_
