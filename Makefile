CC      := i686-w64-mingw32-gcc
WINDRES := i686-w64-mingw32-windres
CFLAGS  := -O2 -Wall -Wextra -Wno-unused-parameter -static-libgcc -std=gnu99 -fno-omit-frame-pointer
LDFLAGS := -shared -Wl,--kill-at

VERSION := $(shell cat VERSION)
# "1.1.0" -> "1,1,0,0" for Windows VS_VERSION_INFO FILEVERSION (always 4
# components).  Embedded PE metadata is the main lever we have against the
# Wacatac.B!ml / Ti!*-class false-positive flags that hit unsigned MinGW
# binaries with empty version info.
comma   := ,
VER_NUM := $(subst .,$(comma),$(VERSION)),0
# windres re-invokes gcc -E -xc on the .rc, so the inner quotes need to
# survive a *second* shell-quoting pass.  '\"...\"' is what reaches cpp.
WINDRESFLAGS := -DPAWNITEMS_VER_NUM=$(VER_NUM) '-DPAWNITEMS_VER_STR=\"$(VERSION)\"' -O coff

SRC_DIR := src
TARGET  := pawnitems.dll
SRC     := $(addprefix $(SRC_DIR)/,pawnitems.c patch.c unequip.c restrict.c)
RES     := pawnitems.res.o

# pawnitems.dll sits next to DDDA.exe / dinput8.dll and is loaded via
# dinput8's [main] addons= line.
DDDA_DIR := /home/istvan/.wine/drive_c/DDDA
DEPLOY   := $(DDDA_DIR)/$(TARGET)

.PHONY: all clean deploy deploy-ini

all: deploy

$(RES): $(SRC_DIR)/pawnitems.rc VERSION
	$(WINDRES) $(WINDRESFLAGS) -i $(SRC_DIR)/pawnitems.rc -o $@

$(TARGET): $(SRC) $(wildcard $(SRC_DIR)/*.h) $(RES) VERSION
	$(CC) $(CFLAGS) -DPAWNITEMS_VERSION='"$(VERSION)"' $(LDFLAGS) -o $@ $(SRC) $(RES) -luser32 -lkernel32 -lversion

deploy: $(TARGET)
	cp $(TARGET) "$(DEPLOY)"
	@echo "== deploy hash check =="
	@md5sum $(TARGET) "$(DEPLOY)"

# Copy the default pawnitems.ini next to the DLL.  Separate from `deploy` so a
# user-customised ini isn't clobbered on every build.  Run once on install,
# or any time you want to reset the config to defaults.
deploy-ini: $(SRC_DIR)/pawnitems.ini
	cp $(SRC_DIR)/pawnitems.ini "$(DDDA_DIR)/pawnitems.ini"
	@echo "== ini deployed to $(DDDA_DIR)/pawnitems.ini =="

clean:
	rm -f $(TARGET) $(RES)
