// This file is part of OpenRAVE.
// OpenRAVE is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#define BOOST_SYSTEM_NO_DEPRECATED
#include <openrave/openrave.h>
#include <openrave/xmlreaders.h>
#include <openrave/openravejson.h>
#include <string>
#include <sstream>
#include <vector>

#include <dlfcn.h>

#include <openravepy/bindings.h>
#include <openravepy/openravepy_int.h>
#ifdef USE_PYBIND11_PYTHON_BINDINGS
#define PYDEF(fndef) m.fndef
#else
#define PYDEF(fndef) py::fndef
#endif
//namespace py = openravepy::py;

typedef int (*tRaveInitialize)(bool, int);
typedef OpenRAVE::UserDataPtr (*tRaveGlobalState)();

class GlobalStateSaver
{
public:
    GlobalStateSaver()
    {
    }
    ~GlobalStateSaver()
    {
        Destroy();
    }
    OpenRAVE::UserDataPtr Init()
    {
        //rave2 = dlmopen(LM_ID_NEWLM, "/usr/local/lib/libopenrave0.169.so", RTLD_NOW);
        //printf("yoooo %016llx\n", rave2);
        //tRaveInitialize pRaveInitialize = (tRaveInitialize)dlsym(rave2, "_ZN8OpenRAVE14RaveInitializeEbi");
        //tRaveGlobalState pRaveGlobalState = (tRaveGlobalState)dlsym(rave2, "_ZN8OpenRAVE15RaveGlobalStateEv");
        //printf("yoooo %016llx\n",pRaveInitialize);

        //pRaveInitialize(true, OpenRAVE::Level_Info);
        //printf("yoooo %016llx\n",pRaveGlobalState);
        //OpenRAVE::UserDataPtr gs2 = pRaveGlobalState();

        gs = OpenRAVE::RaveGlobalState();
        //OpenRAVE::RaveInitializeFromState(gs2);

        void RaveUpdateDataDirs();
        RaveUpdateDataDirs();
        return gs;
    }
    void Destroy()
    {
        //if(gs) {
        //    if(!!OpenRAVE::RaveGlobalState()) {
        //        OpenRAVE::RaveDestroy();
        //    }
        //    OpenRAVE::RaveInitializeFromState(gs);
        //    gs.reset();
        //}
    }

    OpenRAVE::UserDataPtr gs;
    void* rave2;
};

namespace openravepy {
        py::object toPyUserData(OpenRAVE::UserDataPtr p);

	//BOOST_PYTHON_MODULE(openrave_globalstatesaver)
	OPENRAVE_PYTHON_MODULE(openrave_globalstatesaver)
	{
                py::module::import("openravepy"); // declare UserDataPtr

		PYDEF(def("RaveInitializeFromState",[](openravepy::PyUserData globalstate){
                    OpenRAVE::RaveInitializeFromState(globalstate._handle);
                }));
		PYDEF(def("RaveGlobalState",[]{
                    OpenRAVE::UserDataPtr gs = OpenRAVE::RaveGlobalState();
                    return toPyUserData(gs);
                }));

                py::class_<GlobalStateSaver> cGlobalStateSaver(m, "GlobalStateSaver");
                cGlobalStateSaver.def(py::init<>());
                cGlobalStateSaver.def("__enter__", [](GlobalStateSaver &p){
                    return toPyUserData(p.Init());
                });
                cGlobalStateSaver.def("__exit__", [](GlobalStateSaver &p, py::object p1, py::object p2, py::object p3){
                    return p.Destroy();
                });
	}
}
