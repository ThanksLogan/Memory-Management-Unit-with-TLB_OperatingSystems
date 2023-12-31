# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = TLB.o pagetable.o vaddr_tracereader.o print_helpers.o main.o 

# Program name
PROGRAM = mmuwithtlb

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp
	
TLB.o : TLB.cpp TLB.h
	$(CC) $(CCFLAGS) TLB.cpp

pagetable.o : pagetable.cpp pagetable.h
	$(CC) $(CCFLAGS) pagetable.cpp

vaddr_tracereader.o: vaddr_tracereader.c vaddr_tracereader.h
	$(CC) $(CCFLAGS) vaddr_tracereader.c
    
print_helpers.o: print_helpers.c print_helpers.h
	$(CC) $(CCFLAGS) print_helpers.c

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)

