#define BOOST_SYSTEM_NO_DEPRECATED
#include <openrave/openrave.h>

namespace OpenRAVE
{
    int RaveInitializeLOCAL(bool bLoadAllPlugins=true, int level=Level_Info);
    void RaveInitializeFromStateLOCAL(UserDataPtr);
    UserDataPtr RaveGlobalStateLOCAL();
}

#define RaveGlobalState RaveGlobalStateLOCAL
#define RaveInitialize RaveInitializeLOCAL
#define RaveInitializeFromState RaveInitializeFromStateLOCAL
#include "../openrave/src/libopenrave/libopenrave.cpp"
#undef RaveGlobalState
#undef RaveInitialize
#undef RaveInitializeFromState

class __attribute__((visibility("hidden"))) RaveGlobal_UpdateDataDirs: public OpenRAVE::RaveGlobal
{
public:
    using OpenRAVE::RaveGlobal::_create;
    using OpenRAVE::RaveGlobal::_UpdateDataDirs;
};

__attribute__((visibility("default"))) void RaveUpdateDataDirs()
{
    OpenRAVE::UserDataPtr gsuser = RaveGlobalState();
printf("%016llx\n", gsuser.get());
    boost::shared_ptr<RaveGlobal> gs = boost::dynamic_pointer_cast<RaveGlobal>(gsuser);
printf("%016llx\n", gs.get());
    ((RaveGlobal_UpdateDataDirs*)gs.get())->_UpdateDataDirs();
}

__attribute__((visibility("default"))) OpenRAVE::UserDataPtr RaveCreateGlobal()
{
    RaveGlobal_UpdateDataDirs::_create();
    RaveInitializeLOCAL();
    OpenRAVE::UserDataPtr gsuser = RaveGlobalStateLOCAL();
    RaveInitializeFromStateLOCAL(OpenRAVE::UserDataPtr());
    return gsuser;
}
