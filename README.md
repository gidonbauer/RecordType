# RecordType
A simple C++ header-only library for visualizing (simple) algorithms as graphs.

## Getting started

```Console
$ cmake -Bbuild
$ cd bin/
$ make
$ ./example/example_dot_product
$ dot -Tpdf -o graphs/example_dot_product.pdf graphs/example_dot_product.dot
```

## Testing

```Console
$ cmake -Bbuildtest -DRT_BUILD_TEST=ON -DRT_BUILD_EXAMPLE=OFF
$ cd bin/
$ make tests
```

## Third party dependecies

- Graphviz: Graph is written to dot format, graphviz can be used to visualize the graph
- Eigen: Eigen is only used to check for compatibility
