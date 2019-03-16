#!/bin/bash

FLAGS="-Wall -std=c++11"
LIBS=("libcrypto++")
NAME="AESFiles"

SRCS=(".")

for i in ${LIBS[@]}
do
	LIBS_+="$(pkg-config --libs "${i}") "
done

for i in ${SRCS[@]}
do
	SRCS_+="$(find "${i}" -maxdepth 1 | grep -E ".cpp|.hpp") "
done

g++ ${SRCS_} ${FLAGS} ${LIBS_[@]} -o ${NAME}

echo "AESFiles built! Install it by moving it to your PATH."
