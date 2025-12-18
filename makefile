# Example Makefile
VCPKG := ./vcpkg
TOOLCHAIN := $(PWD)/vcpkg/scripts/buildsystems/vcpkg.cmake
VCPKG_INSTALLED := $(PWD)/vcpkg_installed
TRIPLET := x64-linux
BUILD_DIR := build

.PHONY: install build test lint clean

install:
	# TODO: Install VCPKG
	$(VCPKG) install --manifest

build_web:
	cmake -S . -B $(BUILD_DIR) \
		-DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
		-DVCPKG_INSTALLED_DIR=$(VCPKG_INSTALLED) \
		-DCMAKE_PREFIX_PATH=$(VCPKG_INSTALLED)/wasm32-emscripten/share \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-DCMAKE_BUILD_TYPE=Web \
		-DCMAKE_CXX_COMPILER=em++
	cmake --build $(BUILD_DIR) --config Debug

build_debug:
	cmake -S . -B $(BUILD_DIR) \
		-DVCPKG_TARGET_TRIPLET=$(TRIPLET) \
		-DVCPKG_INSTALLED_DIR=$(VCPKG_INSTALLED) \
		-DCMAKE_PREFIX_PATH=$(VCPKG_INSTALLED)/$(TRIPLET)/share \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_COMPILER=g++
	cmake --build $(BUILD_DIR) --config Debug

build:
	cmake -S . -B $(BUILD_DIR) \
		-DVCPKG_TARGET_TRIPLET=$(TRIPLET) \
		-DVCPKG_INSTALLED_DIR=$(VCPKG_INSTALLED) \
		-DCMAKE_PREFIX_PATH=$(VCPKG_INSTALLED)/$(TRIPLET)/share \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CXX_COMPILER=g++
	cmake --build $(BUILD_DIR) --config Release

# test:
# 	mkdir -p $(BUILD_DIR)
# 	make build_debug
# 	ctest --test-dir $(BUILD_DIR) --output-on-failure

lint:
	# TODO: clang-tidy + clang-format
	$(VCPKG) env -- cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) && \
	$(VCPKG) env -- cppcheck --enable=all src

clean:
	rm -rf $(BUILD_DIR)