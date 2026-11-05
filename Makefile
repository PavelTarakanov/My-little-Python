CXX = g++
CXXFLAGS = -Wall -Wextra -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -D _DEBUG -D _EJUDGE_CLIENT_SIDE -Iinclude

TARGETS = front_end.out back_end.out simplification.out asm.out processor.out

all: $(TARGETS)

front_end.out: src/common/Taras_Bulba.cpp src/common/file_using.cpp src/common/dump.cpp src/front_end/infix_read.cpp src/front_end/front_end_main.cpp src/common/output.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

back_end.out: src/common/Taras_Bulba.cpp src/back_end/make_asm.cpp src/common/dump.cpp src/back_end/back_end_main.cpp src/common/file_using.cpp src/common/input.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

simplification.out: src/common/Taras_Bulba.cpp src/common/input.cpp src/common/output.cpp src/common/file_using.cpp src/common/dump.cpp src/middle_end/tree_simplification_main.cpp src/middle_end/equation_simplification.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

asm.out: src/asm/asm_main.cpp src/asm/asm.cpp src/asm/command_codes.cpp src/common/file_using.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

processor.out: src/processor/processor.cpp src/processor/processor_main.cpp src/processor/stack.cpp src/common/file_using.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

clean:
	rm -f $(TARGETS)
