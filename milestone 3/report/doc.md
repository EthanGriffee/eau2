# Introduction

The eau2 system is a distributed key/value store for big data analysis that can run machine learning algorithms to query for specific chunks of data from a distributed array. 

# Architecture

Eau2 has three layers to it:

* The bottom layer (network layer), consist of the distributed KV stores running on multiple nodes such that each of them have access to some part of the data and can communicate with each other to request and transfer data as and when needed.
* The middle layer provides abstraction in the form of distributed arrays and data frames that are used to store the data and represent them in a structured form if and when quered for.
* The top layer is the application layer where users write code and can query for data.

# Implementation

We implemented the following classes as we believe they are needed for the development of the eau2 system:

* **networkifc.h** - we use this for sending and recieving messages.
* **pseudonetwork.h** - we use this for testing as it is a network that uses threads.
* **kvstore.h** - we use this to hold the serialized dataframes.
* **dataframe.h** - we use this to store and display the data after it parsed from a file.
* **parser.h** (the code is borrowed from team 4200NE) - we use this to parse SoR data from a file into our dataframe.
* **arraytemplate.h** - we use this to store any type of data, a template was decided to be used instead of array classes to reduce clutter. 
* **string.h** - we use it to manupilate strings in our code.
* **object.h** - we use it to manupilate different datatypes in our code.
* **wrapper.h** - wrapper classes for ints, doubles, and bools. Used to create an array of Objects in a row.
* **helper.h** - Just used to make debugging / testing easier, supplying print methods.

# Use cases

One of the use cases of the eau2 system is that it can be trained to behave like a generic database wherein we can parse and store data from a file such that users can request for queries and the system will respond back with the appropiate data.

# Open questions


# Status

* We cleaned our code and implemented the classes mentioned above.
* We ran valgrind on our old code to test for memory leaks, for milestone one and two code. 
* We ran into trouble for using valgrind on milestone three code, as we had trouble deleting the 
KVStore as it has a thread running a while true loop, that we cannot join.
* We created a MakeFile that will run our tests for milestone1, milestone2, or defualt milestone 3.
* We created a static Method in DataFrame that creates a dataframe from a file location input.
* Serialization / Deserialization was added for dataframes, columns, and certain arrays.
* We created a pseudo network for testing the KVStore, and ran the DEMO application code on it.