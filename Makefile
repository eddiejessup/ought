# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/include/python$(PYTHON_VERSION)
PYTHON_CONFIG = /usr/local/Cellar/python/2.7.10_2/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/local/Cellar/boost/1.58.0/include
BOOST_LIB = /usr/local/Cellar/boost/1.58.0/lib
BOOST_PYTHON_LIB = /usr/local/Cellar/boost-python/1.58.0/lib
 
# compile mesh classes
TARGET = ought_ext
 
$(TARGET).so: $(TARGET).o ought.o
	g++ -std=c++14 -Wfatal-errors -shared -Wl $(TARGET).o -L$(BOOST_LIB) -L$(BOOST_PYTHON_LIB) -lboost_python -L$(PYTHON_CONFIG) -lpython$(PYTHON_VERSION) ought.o -o $(TARGET).so
 
ought.o:  ought.cpp
	g++ -std=c++14 -Wfatal-errors -c ought.cpp

$(TARGET).o: $(TARGET).cpp ought.hpp
	g++ -std=c++14 -Wfatal-errors -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).cpp
