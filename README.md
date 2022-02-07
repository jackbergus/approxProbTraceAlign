# (Approximated) Probabilistic Trace Alignment

This free and open software program implements the (Approximate) Probabilistic Trace Alignment.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

This project has been mainly tested with Ubuntu 20.04 LTS. For GNU/Linux-based systems, you can run the bash script ```./install_deps.sh``` to both install the required dependencies and packages, pull all the required submodules, and compile the Antlr4 Parser/Lexer. For the other operating systems, please remember to install the following packages:

 * [GraphViz 2.42.2-3](https://graphviz.org/download/) (*libcdt5 libcgraph6 libgraphviz-dev libgvc6 libxdot4 libgvpr2*)
 * [Qt5 5.12.8](https://download.qt.io/official_releases/qt/5.12/5.12.8/) (*qtdeclarative5-dev qtdeclarative5-dev-tools*)
 * [OpenMPI 4.0.3](https://www.open-mpi.org/software/ompi/v4.0/) (*libopenmpi-dev*)
 * [Boost 1.71](https://www.boost.org/users/history/version_1_71_0.html) (*libboost1.71-all-dev*)

### Installing

After doing so, you can create a new folder, where all the generated binaries will be located:

```
mkdir build 
cd build
cmake .. 
make -j8
```





## Built With

* [Maven](https://maven.apache.org/) - Dependency Management for Antlr4
* [CMake](https://cmake.org/download/) - Used as an build automation tool for 

## Contributing

Please e-mail Giacomo Bergami for contributing to the project (or, simply fork it!)

## Versioning

We use [GitHub](http://github.com/) for versioning. For the versions available, see the [tags on this repository](https://github.com/jackbergus/approximateTraceAlignment/tags). 

## Authors

* **Giacomo Bergami** - *Initial work* - [JackBergus](https://github.com/jackbergus), [Gyankos](https://github.com/gyankos)

## License

This project is licensed under the GPLv2 License - see the [LICENSE](LICENSE.) file for details

## Acknowledgments

* This work was part of a paper co-authored with Fabrizio Maggi, Marco Montali & Rafael Peñaloza, and Ramon Farga Pereira


