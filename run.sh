#!/bin/bash

./create_points -V 10.0 100K points.txt
./build_rgraph -T points.txt graph.cover_tree.txt 0.2
./build_rgraph -F points.txt graph.brute_force.txt 0.2
./graph_diff graph.cover_tree.txt graph.brute_force.txt
