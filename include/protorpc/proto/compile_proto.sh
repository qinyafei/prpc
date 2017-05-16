#!/bin/bash

./protoc -I=./proto --cpp_out=./ ./jobdispatch.proto