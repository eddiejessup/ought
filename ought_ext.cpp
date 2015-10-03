#include <boost/python.hpp>
#include "ought.hpp"

BOOST_PYTHON_MODULE(ought_ext) {
    boost::python::class_<Network>("Network", boost::python::init<int, int>())
        .def("iterate", &Network::iterate)
        .def("to_string", &Network::to_string)
        .def("to_json", &Network::to_json)
    ;
}
