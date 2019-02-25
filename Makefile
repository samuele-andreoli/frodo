### Configuration ###

TARGET := libfrodo

## Frodo Configuration
FRODO_TOY := 1
FRODO_CLASSIC := 2
FRODO_RECOMMENDED := 3
FRODO_PARANOID := 4

FRODO_CONFIGURATION := ${FRODO_RECOMMENDED}

## Optimization configuration
MEMORY := 1
TIME := 2
OPTIMIZATION := ${MEMORY}

## Backend configuration
BACKEND_MILAGRO := 1
BACKEND_OPENSSL := 2

BACKEND := BACKEND_MILAGRO

## Local directories
BASEDIR := $(shell pwd)
SRCDIR := src
OBJDIR := .objects
INCLUDEDIR := include
BUILDDIR := build
TESTDIR := test
BENCHDIR := benchmark
EXAMPLEDIR := examples
INSTALLDIR := install
BINDIR := ${BUILDDIR}/bin

## External dependencies

# Milagro
MILAGRO_URL := https://github.com/milagro-crypto/milagro-crypto-c.git
MILAGRO_SRCDIR := ${BASEDIR}/milagro-crypto-c
MILAGRO_BUILDDIR := ${MILAGRO_SRCDIR}/target/aes
MILAGRO_INSTALLDIR := ${BASEDIR}/${INSTALLDIR}/amcl
MILAGRO_VERSION := 4.12.0

# Library paths
MILAGRO_INCLUDE_PATH := ${MILAGRO_INSTALLDIR}/include
MILAGRO_LIBRARY_PATH := ${MILAGRO_INSTALLDIR}/lib
FRODO_INCLUDE_PATH := ${BASEDIR}/${BUILDDIR}/include
FRODO_LIBRARY_PATH := ${BASEDIR}/${BUILDDIR}/lib
INCLUDE_PATH := $(shell echo ${C_INCLUDE_PATH}):${MILAGRO_INCLUDE_PATH}:${FRODO_INCLUDE_PATH}
LIBRARY_PATH := $(shell echo ${LD_LIBRARY_PATH}):${MILAGRO_LIBRARY_PATH}:${FRODO_LIBRARY_PATH}

## Compilation options
CC := gcc

BASEFLAGS := -Wall -Werror -Wextra -pedantic

ifeq ($(OPTIMIZATION),$(MEMORY))
	OFLAGS := -O2
endif
ifeq ($(OPTIMIZATION),$(TIME))
	OFLAGS := -O3
endif

CFLAGS = ${BASEFLAGS} ${OFLAGS} -DFRODO_OPTIMIZATION=${OPTIMIZATION} -DFRODO_BACKEND=${BACKEND} -DFRODO_CONFIGURATION=${FRODO_CONFIGURATION}

## Linker options
MILAGRO_LINK_FLAGS := -L${MILAGRO_LIBRARY_PATH} -lamcl_core
OPENSSL_LINK_FLAGS := -lcrypto -lssl -lm
FRODO_LINK_FLAGS := -L${FRODO_LIBRARY_PATH} -lfrodo

ifeq ($(OPTIMIZATION),$(MEMORY))
	LINK_FLAGS := ${MILAGRO_LINK_FLAGS}
endif
ifeq ($(OPTIMIZATION),$(TIME))
	LINK_FLAGS := ${MILAGRO_LINK_FLAGS} ${OPENSSL_LINK_FLAGS}
endif

### Prepare env ###
BUILD_SOURCES := $(shell find ${SRCDIR} -type f -name "*.c")
BUILD_OBJECTS := $(addprefix $(OBJDIR)/,$(BUILD_SOURCES:.c=.o))

TEST_SOURCES := $(shell find ${TESTDIR} -type f -name "*.c")
TEST_TARGETS := $(addprefix $(BINDIR)/,$(addsuffix .test,$(TEST_SOURCES:.c=)))

BENCH_SOURCES := $(shell find ${BENCHDIR} -type f -name "*.c")
BENCH_TARGETS := $(addprefix $(BINDIR)/,$(addsuffix .bench,$(BENCH_SOURCES:.c=)))

BINARIES_SOURCES := $(shell find ${EXAMPLEDIR} -type f -name "*.c") ${TEST_SOURCES} ${BENCH_SOURCES}
BINARIES := $(addprefix $(BINDIR)/,$(BINARIES_SOURCES:.c=))

TEST_REPORT := test_report.txt
BENCH_REPORT := benchmark_report.txt

### Rules ###
.PHONY:all help build install uninstall

## Default rule
all: test

## Base rules
help:
	@echo make all: build all targets
	@echo make install: install library to install directory
	@echo make uninstall: uninstall library
	@echo make test: run tests
	@echo make bench: run benchmarks
	@echo make build: build shared library and binaries
	@echo make dep: resolve library dependencies
	@echo make cleanall: clean all library files, binaries and dependencies
	@echo make clean: clean all library files, binaries and dependencies, but the dependencies sources 
	@echo make cleanbuild: clean build files

install: test
	mkdir -p ${INSTALLDIR}/frodo
	cp -rf ${BUILDDIR}/lib ${INSTALLDIR}/frodo
	cp -rf ${BUILDDIR}/include ${INSTALLDIR}/frodo

uninstall:
	-rm -rf ${INSTALLDIR}/frodo

## Build rules
build: dep ${BUILDDIR}/lib/${TARGET}.so $(BINARIES)
	@echo "Build Done"

${BUILDDIR}/lib/${TARGET}.so: ${BUILD_OBJECTS}
	@mkdir -p ${BUILDDIR}/lib ${BUILDDIR}/include
	@cd ${BUILDDIR}/lib
	@echo "Build shared lib $@"
	@cp ${INCLUDEDIR}/* ${BUILDDIR}/include
	@export C_INCLUDE_PATH=${INCLUDE_PATH}; \
	 $(CC) -fPIC ${CFLAGS} ${LINK_FLAGS} $^ -shared -o $@

$(BUILD_OBJECTS): $(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Build object $@"
	export C_INCLUDE_PATH=${INCLUDE_PATH}:${INCLUDEDIR}; \
	 $(CC) -fPIC ${CFLAGS} ${LINK_FLAGS} -c $< -o $@

$(BINARIES): ${BINDIR}/%: %.c
	@mkdir -p $(@D)
	@echo "Build binary $@"
	@export C_INCLUDE_PATH=${INCLUDE_PATH}:${BENCHDIR}; \
	 $(CC) ${CFLAGS} $< ${FRODO_LINK_FLAGS} ${LINK_FLAGS} -o $@


# Testing rules
.PHONY: test test_header

test: cleantest build test_header $(TEST_TARGETS)
	@echo "------------------------------------------------------------"

test_header:
	@echo "------------------------------------------------------------"
	@echo "  Run tests"

$(TEST_TARGETS): ${BINDIR}/%.test: ${BINDIR}/%
	@echo "------------------------------------------------------------"
	@echo "  Run $<"
	@echo "  $<" >> ${TEST_REPORT}
	@export LD_LIBRARY_PATH=${LIBRARY_PATH}; ./$< >> ${TEST_REPORT} &&												\
	(echo "    - Test successful"; echo "    - Test successful" >> ${TEST_REPORT};) || \
	(echo "    - Test failed"; echo "    - Test failed" >> ${TEST_REPORT};)

# Testing
.PHONY: bench bench_header

bench: cleanbench bench_header $(BENCH_TARGETS)
	@echo "------------------------------------------------------------"
	@echo "Benchmarks done."

bench_header:
	@echo "------------------------------------------------------------"
	@echo "  Run benchmarks"
	@echo "------------------------------------------------------------"

$(BENCH_TARGETS): ${BINDIR}/%.bench: ${BINDIR}/%
	@echo "  Run $<"
	@echo "\n  $<\n" >> ${BENCH_REPORT}
	@export LD_LIBRARY_PATH=${LIBRARY_PATH}; ./$< >> ${BENCH_REPORT}

## Dependencies rules
.PHONY: dep

dep: ${MILAGRO_INSTALLDIR}
	@# Display dependencies configuration
	@echo "*** Dependencies resolved ***"
	@echo "Dynamic library path"
	@echo ${FRODO_LIBRARY_PATH}:${MILAGRO_LIBRARY_PATH}
	@echo "Include path"
	@echo ${INCLUDE_PATH}

${MILAGRO_INSTALLDIR}: ${MILAGRO_BUILDDIR}
	${MAKE} -C ${MILAGRO_BUILDDIR}
	${MAKE} -C ${MILAGRO_BUILDDIR} install

${MILAGRO_BUILDDIR}: ${MILAGRO_SRCDIR}
	mkdir -p ${MILAGRO_INSTALLDIR}
	cd ${MILAGRO_SRCDIR};			\
	git checkout ${MILAGRO_VERSION};\
	mkdir -p ${MILAGRO_BUILDDIR};	\
	cd ${MILAGRO_BUILDDIR};			\
	cmake -DCMAKE_C_FLAGS=-O3 -DWORD_SIZE=64 -DAMCL_CURVE= -DAMCL_RSA= -DCMAKE_INSTALL_PREFIX=${MILAGRO_INSTALLDIR} ${MILAGRO_SRCDIR}

${MILAGRO_SRCDIR}:
	git clone ${MILAGRO_URL} ${MILAGRO_SRCDIR}

## Cleanup rules
.PHONY: cleanall clean cleanbuild cleantest cleanbench

# Nuke everything
cleanall: cleandepall clean

# Clean build files, examples and dependencies
clean: cleandep cleanbuild
	-rm -rf ${BUILDDIR}

# Clean build files
cleanbuild: cleantest
	-rm -rf ${OBJDIR}

# Clean test report
cleantest:
	-rm ${TEST_REPORT}

# Clean bench report
cleanbench:
	-rm ${BENCH_REPORT}

# Dependency cleanup
.PHONY: cleandepall cleandep cleandepinstall cleandepbuild cleandepsrc

cleandepall: cleandep cleandepsrc cleandepinstall

cleandep: cleandepbuild cleandepinstall
	-rm -rf ${MILAGRO_BUILDDIR}

cleandepinstall:
	-rm -rf ${MILAGRO_INSTALLDIR}

cleandepbuild:
	-rm -rf ${MILAGRO_BUILDDIR}

cleandepsrc:
	-rm -rf ${MILAGRO_DIR}