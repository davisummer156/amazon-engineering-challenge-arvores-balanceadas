CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic
PYTHON = python
HEADERS = src/avl.hpp src/rubro_negra.hpp
OUT = results/atual

.PHONY: all test compare demo menu-test verify clean
all: bin/encomendas.exe bin/tests.exe bin/compare.exe bin/demo.exe

bin $(OUT):
	mkdir -p $@

bin/encomendas.exe: src/main.cpp $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) $< -o $@

bin/tests.exe: tests/tests.cpp tests/test_support.hpp $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) $< -o $@

bin/compare.exe: tests/compare.cpp src/main.cpp $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) $< -o $@

bin/demo.exe: tests/demo.cpp tests/test_support.hpp $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) $< -o $@

test: bin/tests.exe | $(OUT)
	./bin/tests.exe > $(OUT)/tests.log
	cat $(OUT)/tests.log

compare: bin/compare.exe | $(OUT)
	./bin/compare.exe > $(OUT)/comparison.log
	cat $(OUT)/comparison.log

demo: bin/demo.exe | $(OUT)
	./bin/demo.exe > $(OUT)/demo.log
	cat $(OUT)/demo.log

menu-test: bin/encomendas.exe | $(OUT)
	$(PYTHON) tests/menu_test.py

verify: test compare demo menu-test

clean:
	rm -f bin/encomendas.exe bin/tests.exe bin/compare.exe bin/demo.exe
