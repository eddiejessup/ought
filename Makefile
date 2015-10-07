CC=gcc
CXX=g++

# location of the Python header files
PYTHON_VERSION=2.7
PYTHON_INCLUDE=/usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/include/python$(PYTHON_VERSION)
PYTHON_CONFIG=/usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config

# location of the Boost Python include files and library
BOOST_INCLUDE=/usr/local/Cellar/boost/1.58.0/include
BOOST_LIB=/usr/local/Cellar/boost/1.58.0/lib
BOOST_PYTHON_LIB=/usr/local/Cellar/boost-python/1.58.0/lib

CPPFLAGS=-std=c++14 -Wfatal-errors
LDLIBS=-L$(BOOST_LIB) -L$(BOOST_PYTHON_LIB) -lboost_python -L$(PYTHON_CONFIG) -lpython$(PYTHON_VERSION)
LDFLAGS=-shared
BOOST_EXT_INCLUDE=-I$(PYTHON_INCLUDE) -I$(BOOST_INCLUDE)
BOOST_EXT_FLAGS=-fPIC

SRCS=vector_format.cpp ought.cpp ought_ext.cpp strings_format.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

ought_ext.so: $(OBJS)
	$(CXX) $(LDFLAGS) -o ought_ext.so $(OBJS) $(LDLIBS)

ought_ext.o: ought_ext.cpp ought.hpp
	$(CXX) $(CPPFLAGS) $(BOOST_EXT_INCLUDE) $(BOOST_EXT_FLAGS) -c ought_ext.cpp

# ought.o: ought.cpp ought.hpp vector_format.hpp
# 	$(CXX) $(CPPFLAGS) -c ought.cpp

# vector_format.o: vector_format.cpp vector_format.hpp strings_format.hpp
# 	$(CXX) $(CPPFLAGS) -c vector_format.cpp

# strings_format.o: strings_format.cpp strings_format.hpp
# 	$(CXX) $(CPPFLAGS) -c strings_format.cpp
