#define BOOST_SYSTEM_NO_DEPRECATED
#include <openrave/openrave.h>

#define RaveGlobalState RaveGlobalStateXXX
#include "../openrave/src/libopenrave/libopenrave.cpp"
#undef RaveGlobalState

class __attribute__((visibility("hidden"))) RaveGlobal_UpdateDataDirs: public OpenRAVE::RaveGlobal
{
public:
    using OpenRAVE::RaveGlobal::_UpdateDataDirs;
};

void RaveUpdateDataDirs()
{
    OpenRAVE::UserDataPtr gsuser = RaveGlobalState();
printf("%016llx\n", gsuser.get());
    boost::shared_ptr<RaveGlobal> gs = boost::dynamic_pointer_cast<RaveGlobal>(gsuser);
printf("%016llx\n", gs.get());
    ((RaveGlobal_UpdateDataDirs*)gs.get())->_UpdateDataDirs();
}
