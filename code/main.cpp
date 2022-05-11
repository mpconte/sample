#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>

using namespace boost::filesystem;

int main(int argc, char *argv[]) {
    path p(argc>1? argv[1] : ".");      
    double array[2][2] = { { 1, 2,}, { 3, 4 }};    

    int type_size = sizeof(int);    
    boost::python::tuple shape = boost::python::make_tuple<int, int>(2, 2);
    boost::python::tuple strides = boost::python::make_tuple(type_size * 2, type_size * 1); 
        
    boost::python::numpy::ndarray np_array = boost::python::numpy::from_data(
        array, // array of data
        boost::python::numpy::dtype::get_builtin<double>(), // type
        shape, // shape
        strides, // strides
        boost::python::object() // owner object
    );
    auto data = np_array.get_data();
    
    if(is_directory(p)) {
        std::cout << p << " is a directory containing:\n";

        for(auto& entry : boost::make_iterator_range(directory_iterator(p), {}))
            std::cout << entry << "\n";
    }
}