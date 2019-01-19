### Configuration ###

TARGET := libfrodo

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

### Prepare env ###
BUILD_SOURCES := $(shell find ${SRCDIR} -type f -name "*.c")
BUILD_OBJECTS := $(addprefix $(OBJDIR)/,$(BUILD_SOURCES:.c=.o))

TEST_SOURCES := $(shell find ${TESTDIR} -type f -name "*.c")
TEST_TARGETS := $(addprefix $(BINDIR)/,$(addsuffix .test,$(TEST_SOURCES:.c=)))

BINARIES_SOURCES := $(shell find ${EXAMPLEDIR} ${BENCHDIR} -type f -name "*.c") ${TEST_SOURCES}
BINARIES := $(addprefix $(BINDIR)/,$(BINARIES_SOURCES:.c=))

TEST_REPORT := test_report.txt

### Rules ###
.PHONY:all help install test build

## Default rule
all: test

## Base rules
help:
	@echo make all: build all targets
	@echo make install: install library to install directory
	@echo make test: run tests
	@echo make build: build shared library and binaries
	@echo make dep: resolve library dependencies
	@echo make cleanall: clean all library files, binaries and dependencies
	@echo make clean: clean all library files, binaries and dependencies, but the dependencies sources 
	@echo make cleanbuild: clean build files

install: test

test: cleantest build test_header $(TEST_TARGETS)
	@echo "------------------------------------------------------------"

test_header:
	@echo "------------------------------------------------------------"
	@echo "  Run tests"

$(TEST_TARGETS): ${BINDIR}/%.test: ${BINDIR}/%
	@echo "------------------------------------------------------------"
	@echo "  Run $<" 
	@echo "  $<" >> ${TEST_REPORT}
	@export LD_LIBRARY_PATH=${FRODO_LIBRARY_PATH}:${MILAGRO_LIBRARY_PATH}; ./$< >> ${TEST_REPORT} &&												\
	(echo "    - Test successful"; echo "    - Test successful" >> ${TEST_REPORT};) || \
	(echo "    - Test failed"; echo "    - Test failed" >> ${TEST_REPORT};)

build: dep ${BUILDDIR}/lib/${TARGET}.so $(BINARIES)
	@echo "Build Done"

## Build rules
${BUILDDIR}/lib/${TARGET}.so: ${BUILD_OBJECTS}
	@mkdir -p ${BUILDDIR}/lib ${BUILDDIR}/include
	@cd ${BUILDDIR}/lib
	@echo "Build shared lib $@"
	@cp ${INCLUDEDIR}/* ${BUILDDIR}/include
	@export C_INCLUDE_PATH=${INCLUDE_PATH}; \
	 $(CC) -fPIC ${BASEFLAGS} $^ -shared -o $@

$(BUILD_OBJECTS): $(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Build object $@"
	@export C_INCLUDE_PATH=${INCLUDE_PATH}:${INCLUDEDIR}; \
	 $(CC) -fPIC ${BASEFLAGS} -c $< -o $@

$(BINARIES): ${BINDIR}/%: %.c
	@mkdir -p $(@D)
	@echo "Build binary $@"
	@export C_INCLUDE_PATH=${INCLUDE_PATH}; \
	 $(CC) ${BASEFLAGS} $< -L${FRODO_LIBRARY_PATH} -lfrodo -L${MILAGRO_LIBRARY_PATH} -lamcl_core -o $@

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
	cmake -DWORD_SIZE=64 -DAMCL_CURVE= -DAMCL_RSA= -DCMAKE_INSTALL_PREFIX=${MILAGRO_INSTALLDIR} ${MILAGRO_SRCDIR}

${MILAGRO_SRCDIR}:
	git clone ${MILAGRO_URL} ${MILAGRO_SRCDIR}

## Cleanup rules
.PHONY: cleanall clean cleanbuild cleantest

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

# Dependency cleanup
.PHONY: cleandepall cleandep cleandepinstall cleandepbuild cleandepsrc

cleandepall: cleandep cleandepsrc cleandepinstall

cleandep: cleandepbuild cleandepinstall
	-rm -rf ${MILAGRO_BUILDDIR}

cleandepinstall: 
	-rm -rf ${INSTALLDIR}

cleandepbuild:
	-rm -rf ${MILAGRO_BUILDDIR}

cleandepsrc:
	-rm -rf ${MILAGRO_DIR}