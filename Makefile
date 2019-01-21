ifeq ($(strip $(DEVKITPRO)),)
$(error Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

include $(DEVKITARM)/base_tools

#-------------------------------------------------------------------------------
export BUILD := build
export SRC := src
export BINARY := $(BUILD)/linked.o
export ARMIPS ?= armips
export ROM_CODE := BPRE
export LD := $(PREFIX)ld
export PREPROC := deps/pokeruby/tools/preproc/preproc
export CHARMAP := charmap.txt
export ASFLAGS := -mthumb
export PAGB_INCLUDE := deps/g3headers/build

export INCLUDE := -I $(PAGB_INCLUDE)/include -I $(SRC) -I .
export LDFLAGS := -T layout.ld -T $(PAGB_INCLUDE)/linker/$(ROM_CODE).ld -r

export CFLAGS := -g -O2 -Wall -mthumb -std=c11 $(INCLUDE) -mcpu=arm7tdmi \
	-march=armv4t -mno-thumb-interwork -fno-inline -fno-builtin -mlong-calls -DROM_$(ROM_CODE) \
	-fdiagnostics-color
export DEPDIR = .d
export DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

#-------------------------------------------------------------------------------

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Generated
IMAGES=$(call rwildcard,images,*.png)

# Sources
C_SRC=$(call rwildcard,$(SRC),*.c)
S_SRC=$(call rwildcard,$(SRC),*.s)
GEN_SRC=$(IMAGES:images/%.png=generated/images/%.c)

# Binaries
C_OBJ=$(C_SRC:%=%.o)
S_OBJ=$(S_SRC:%=%.o)
GEN_OBJ=$(GEN_SRC:%=%.o)
OBJECTS=$(addprefix $(BUILD)/,$(GEN_OBJ) $(C_OBJ) $(S_OBJ))

#-------------------------------------------------------------------------------

.PHONY: all clean test generated images patch

all: main.s $(BINARY) $(call rwildcard,patches,*.s)
	@echo -e "\e[1;32mCreating ROM\e[0m"
	$(ARMIPS) main.s -sym output.txt
	@echo -e "\e[1;32mBuild Complete\e[0m"

clean:
	rm -rf build
	rm -rf generated

test:
	$(MAKE) -f test/Makefile

$(BINARY): $(OBJECTS)
	@echo -e "\e[1;32mLinking ELF binary $@\e[0m"
	@$(LD) $(LDFLAGS) -o $@ $^

$(BUILD)/%.c.o: %.c $(DEPDIR)/%.d
	@echo -e "\e[32mCompiling $<\e[0m"
	@mkdir -p $(@D)
	@mkdir -p $(DEPDIR)/$<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -E -c $< -o $*.i
	@$(PREPROC) $*.i $(CHARMAP) | $(CC) $(CFLAGS) -x c -o $@ -c -
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(BUILD)/%.s.o: %.s
	@echo -e "\e[32mAssembling $<\e[0m"
	@mkdir -p $(@D)
	@$(PREPROC) $< $(CHARMAP) | $(AS) $(ASFLAGS) -o $@

generated/images/%.c: images/%.png images/%.grit
	@echo -e "\e[34mProcessing image $<\e[0m"
	@mkdir -p $(@D)
	@grit $< -o $@ -ff$(<:%.png=%.grit)
	@python scripts/grithack.py $@


$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(C_SRC)))
