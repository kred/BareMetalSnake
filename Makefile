PROJECT = baremetalsnake
INCDIR += inc
SRCDIR += src
OBJDIR += obj
LIBS +=
LINKER = $(SRCDIR)/linker.ld
COPT += -Wall -O2 -mcpu=arm1176jzf-s 
COPT += -ffreestanding -nostdinc -nostdlib -nostartfiles


###############################################################################

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

COPT += $(foreach dir, $(INCDIR), -I$(dir)) 

LDOPT += -T$(LINKER) 
LDOPT += -Wl,-Map=$(OBJDIR)/$(PROJECT).map,--cref,--gc-sections $(COPT) 
LDOPT += -g -ffunction-sections -fdata-sections

CSRC = $(wildcard $(SRCDIR)/*.c)
ASMSRC = $(wildcard $(SRCDIR)/*.s)

#VPATH = $(SRCDIR):$(INCDIR)
OBJECTS += $(addprefix $(OBJDIR)/,$(notdir $(CSRC:%.c=%.o) $(ASMSRC:%.s=%.o)))

all: $(OBJDIR)/$(PROJECT).elf $(OBJDIR)/$(PROJECT).bin $(OBJDIR)/$(PROJECT).lst image

$(OBJDIR)/$(PROJECT).elf: $(OBJECTS)
	@echo "Linking $@..."
	@$(CC) $(OBJECTS) $(LIBS) $(LDOPT) --output $@

$(OBJDIR)/$(PROJECT).bin: $(OBJDIR)/$(PROJECT).elf
	@echo "Converting to $@..."
	@$(OBJCOPY) -O binary $< $@

$(OBJDIR)/$(PROJECT).lst: $(OBJDIR)/$(PROJECT).elf
	@echo "Disassembling to $@..."
	@$(OBJDUMP) -D $< > $(OBJDIR)/$(PROJECT).lst

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $@..."
	@$(CC) -c $(COPT) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	@echo "Compiling $@..."
	@$(CC) -c $(COPT) $< -o $@

image:
	@echo "Creating boot image..."
	@cp -f $(OBJDIR)/$(PROJECT).bin img/kernel.img

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS)
	@rm -f $(OBJDIR)/$(PROJECT).elf
	@rm -f $(OBJDIR)/$(PROJECT).bin
	@rm -f $(OBJDIR)/$(PROJECT).lst
	@rm -f $(OBJDIR)/$(PROJECT).map
