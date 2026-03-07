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

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<OpenRAVE::Readable> ReadablePtr;

std::vector<OpenRAVE::UserDataPtr> vRegisteredReaders;

#include <openravepy/bindings.h>
#include <openravepy/openravepy_int.h>
#ifdef USE_PYBIND11_PYTHON_BINDINGS
#define PYDEF(fndef) m.fndef
#else
#define PYDEF(fndef) py::fndef
#endif
//namespace py = openravepy::py;

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
        gs = OpenRAVE::RaveGlobalState();
        OpenRAVE::RaveInitializeFromState(OpenRAVE::UserDataPtr());
        return gs;
    }
    void Destroy()
    {
        if(gs) {
            if(!!OpenRAVE::RaveGlobalState()) {
                OpenRAVE::RaveDestroy();
            }
            OpenRAVE::RaveInitializeFromState(gs);
            gs.reset();
        }
    }

    OpenRAVE::UserDataPtr gs;
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
