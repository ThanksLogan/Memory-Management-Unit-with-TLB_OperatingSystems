cmake_minimum_required(VERSION 3.20)
project(AS32)

set(CMAKE_CXX_STANDARD 14)

add_executable(AS32 main.cpp vaddr_tracereader.c vaddr_tracereader.h TLB.h TLB.cpp pagetable.h pagetable.cpp print_helpers.c print_helpers.h)
