CC=gcc
CC_FLAGS_COMMON=-Wall
CC_FLAGS_DEBUG=-g -fprofile-arcs -ftest-coverage
CC_FLAGS_RELEASE=

FLEX=flex
BISON=bison

BUILD_FOLDER=build

SRC_FOLDER=src
SRC_MUJ7_FOLDER=$(SRC_FOLDER)/cpu
LIBMUJ7_SOURCE=$(SRC_MUJ7_FOLDER)/muj7cpu.c
LIBMUJ7_TARGET=$(BUILD_FOLDER)/libmuj7.so 

SRC_MUJ7_HELPER_FOLDER=$(SRC_FOLDER)/helper
MUJ7_HELPER_SOURCE=$(SRC_MUJ7_HELPER_FOLDER)/muj7helper.c

SRC_ASSEMBLER_FOLDER=$(SRC_FOLDER)/assembler
ASSEMBLER_LEX_SOURCE=$(SRC_ASSEMBLER_FOLDER)/muj7asm.l
ASSEMBLER_YACC_SOURCE=$(SRC_ASSEMBLER_FOLDER)/muj7asm.y
ASSEMBLER_LEX_TARGET=$(BUILD_FOLDER)/lex.yy.c
ASSEMBLER_YACC_TARGET=$(BUILD_FOLDER)/y.tab.c
ASSEMBLER_TARGET=$(BUILD_FOLDER)/muj7asm.out

SRC_DEBUGGER_FOLDER=$(SRC_FOLDER)/debugger
DEBUGGER_SOURCE=$(SRC_DEBUGGER_FOLDER)/muj7debugger.c
DEBUGGER_TARGET=$(BUILD_FOLDER)/muj7debugger.out

SRC_MACHINE_FOLDER=$(SRC_FOLDER)/machine
MACHINE_SOURCE=$(SRC_MACHINE_FOLDER)/machine.c
MACHINE_TARGET=$(BUILD_FOLDER)/machine.out

TEST_FOLDER=test
BUILD_TEST_FOLDER=$(BUILD_FOLDER)/test
BUILD_COVERAGE_FOLDER=$(BUILD_FOLDER)/coverage
TEST_SOURCE=$(TEST_FOLDER)/cputest.c
TEST_TARGET=$(BUILD_TEST_FOLDER)/cputest.out

.PHONY: all debug release clean

all: debug

debug: CC_FLAGS=$(CC_FLAGS_DEBUG)
debug: cpu assembler debugger machine

release: CC_FLAGS=$(CC_FLAGS_RELEASE)
release: cpu assembler debugger machine

cpu: $(LIBMUJ7_TARGET)

$(LIBMUJ7_TARGET): $(LIBMUJ7_SOURCE) 
	mkdir -p $(BUILD_FOLDER)
	$(CC) $(CC_FLAGS_COMMON) $(CC_FLAGS) -shared $< -o $@

assembler: $(ASSEMBLER_TARGET)
	
$(ASSEMBLER_TARGET): $(ASSEMBLER_LEX_TARGET) $(ASSEMBLER_YACC_TARGET)
	$(CC) -o $(ASSEMBLER_TARGET) $(ASSEMBLER_LEX_TARGET) $(ASSEMBLER_YACC_TARGET) -lfl

$(ASSEMBLER_LEX_TARGET): $(ASSEMBLER_LEX_SOURCE) 
	mkdir -p $(BUILD_FOLDER)
	$(FLEX) -o $(ASSEMBLER_LEX_TARGET) $(ASSEMBLER_LEX_SOURCE)

$(ASSEMBLER_YACC_TARGET): $(ASSEMBLER_YACC_SOURCE) 
	$(BISON) -o $(ASSEMBLER_YACC_TARGET) -dy $(ASSEMBLER_YACC_SOURCE)

debugger: $(DEBUGGER_TARGET)

$(DEBUGGER_TARGET): $(DEBUGGER_SOURCE) $(MUJ7_HELPER_SOURCE) $(LIBMUJ7_TARGET) 
	$(CC) $(DEBUGGER_SOURCE) $(MUJ7_HELPER_SOURCE) -I$(SRC_FOLDER) -L$(BUILD_FOLDER) -lmuj7 -lncurses $(CC_FLAGS) -o $(DEBUGGER_TARGET)

machine: $(MACHINE_TARGET)

$(MACHINE_TARGET): $(MACHINE_SOURCE) $(LIBMUJ7_TARGET)
	$(CC) $< $(MUJ7_HELPER_SOURCE) -I$(SRC_FOLDER) -o $@ -I$(SRC_FOLDER) -L$(BUILD_FOLDER) -lmuj7 -lSDL2 -lm

tests: $(TEST_TARGET)
	LD_LIBRARY_PATH=$(BUILD_FOLDER) $(TEST_TARGET)	

$(TEST_TARGET): $(TEST_SOURCE) $(LIBMUJ7_TARGET) 
	mkdir -p $(BUILD_TEST_FOLDER)
	$(CC) -I$(SRC_FOLDER) -L$(BUILD_FOLDER) $(TEST_SOURCE) -lcunit -lmuj7 -o $(TEST_TARGET)

report-coverage: tests
report-coverage:
	mkdir -p $(BUILD_COVERAGE_FOLDER)
	gcovr --root . --html --html-details --output $(BUILD_COVERAGE_FOLDER)/coverage.html

clean:
	rm -Rf $(BUILD_FOLDER)
