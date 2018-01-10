SLUG = PvC
VERSION = 0.5.7

# FLAGS will be passed to both the C and C++ compiler
# FLAGS +=
# CFLAGS +=
# CXXFLAGS +=

FLAGS +=
CFLAGS += -O3 -std=c99
CXXFLAGS += -O3

# Careful about linking to libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine.
LDFLAGS +=

# Add .cpp and .c files to the build
SOURCES = $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
DISTRIBUTABLES += $(wildcard LICENSE*) res

# Must include the VCV plugin Makefile framework
include ../../plugin.mk


# Convenience target for including files in the distributable release
#.PHONY: dist
#dist: all
#ifndef VERSION
#	$(error VERSION must be defined when making distributables)
#endif
#	mkdir -p dist/$(SLUG)
#	cp LICENSE* dist/$(SLUG)/
#	cp $(TARGET) dist/$(SLUG)/
#	cp -R res dist/$(SLUG)/
#	cp -R example-patches dist/$(SLUG)/
#	cd dist && zip -5 -r $(SLUG)-$(VERSION)-$(ARCH).zip $(SLUG)

