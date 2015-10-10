#include <boost/python.hpp>
#include "ought.hpp"

BOOST_PYTHON_MODULE(ought_ext) {
    void (Network::*iterate_1)() = &Network::iterate;
    void (Network::*iterate_n)(int) = &Network::iterate;
    boost::python::class_<Network>("Network", boost::python::init<std::string>())
        .def("iterate", iterate_1)
        .def("iterate", iterate_n)
        .def("to_string", &Network::to_string)
        .def("to_json", &Network::to_json)
    ;
}
