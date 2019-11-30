#ifndef _REGISTERS_H
#define _REGISTERS_H

#define ARM_IO_BASE		0x20000000
#define REG(addr) (*((volatile unsigned int *)(ARM_IO_BASE + addr)))

// GPIO registers offset address
#define GPFSEL0       0x200000
#define GPFSEL1       0x200004
#define GPFSEL2       0x200008
#define GPFSEL3       0x20000C
#define GPFSEL4       0x200010
#define GPFSEL5       0x200014

#define GPSET0        0x20001C
#define GPSET1        0x200020
#define GPCLR0        0x200028
#define GPCLR1        0x20002C
#define GPLEV0        0x200034
#define GPLEV1        0x200038
#define GPPUD         0x200094
#define GPPUDCLK0     0x200098
#define GPPUDCLK1     0x20009C

#define GPFSEL_IN     0x0
#define GPFSEL_OUT    0x1
#define GPFSEL_AF0    0x4
#define GPFSEL_AF1    0x5
#define GPFSEL_AF2    0x6
#define GPFSEL_AF3    0x7
#define GPFSEL_AF4    0x3
#define GPFSEL_AF5    0x2

// set GPIO function, pin is 0-9
#define GPIOFSEL(reg, pin, fun) REG(reg) &= ~(0x7 << (pin*3));REG(reg) |= (fun << (pin*3))
// set GPIO output, pin is 0-31
#define GPIOSET0(pin) REG(GPSET0) |= (1 << pin)
// set GPIO output, pin is 32-53
#define GPIOSET1(pin) REG(GPSET1) |= (1 << (pin-32))
// get GPIO intput, pin is 0-31
#define GPIOGET0(pin) (REG(GPLEV0) & (1 << pin))
// get GPIO intput, pin is 32-53
#define GPIOGET1(pin) (REG(GPLEV1) & (1 << (pin-32)))
// clear GPIO output, pin is 0-31
#define GPIOCLR0(pin) REG(GPCLR0) |= (1 << pin)
// clear GPIO output, pin is 32-53
#define GPIOCLR1(pin) REG(GPCLR1) |= (1 << (pin-32))



// SPI registers offset address
#define SPICS         0x204000
#define SPIFIFO       0x204004
#define SPICLK        0x204008
#define SPIDLEN       0x20400c
#define SPILTOH       0x204010
#define SPIDC         0x204014

#define SPI_LEN_LONG  (1<<25)
#define SPI_DMA_LEN   (1<<24)
#define SPI_CSPOL2    (1<<23)
#define SPI_CSPOL1    (1<<22)
#define SPI_CSPOL0    (1<<21)
#define SPI_RXF       (1<<20)
#define SPI_RXR       (1<<19)
#define SPI_TXD       (1<<18)
#define SPI_RXD       (1<<17)
#define SPI_DONE      (1<<16)
#define SPI_LEN       (1<<13)
#define SPI_REN       (1<<12)
#define SPI_ADCS      (1<<11)
#define SPI_INTR      (1<<10)
#define SPI_INTD      (1<<9)
#define SPI_DMAEN     (1<<8)
#define SPI_TA        (1<<7)
#define SPI_CSPOL     (1<<6)
#define SPI_CLEAR1    (1<<5)
#define SPI_CLEAR0    (1<<4)
#define SPI_CPOL      (1<<3)
#define SPI_CPHA      (1<<2)
#define SPI_CS1       (1<<1)
#define SPI_CS0       (1<<0)

// setup SPI
#define SPI_SETUP(options) REG(SPICS) = options
#define SPI_CLK_DIV(div) REG(SPICLK) = div
#define SPI_WRITE(val) REG(SPIFIFO) = val & 0xff
#define SPI_BUSY !(REG(SPICS) & SPI_DONE)
#define SPI_TXD_READY (REG(SPICS) & SPI_TXD)
#define SPI_ENABLE_CS SPI_SETUP(SPI_TA | SPI_CLEAR1 | SPI_CLEAR0)
#define SPI_DISABLE_CS SPI_SETUP(0)

// System Timer registers
#define SYSTIMCS      0x003000
#define SYSTIMCLO     0x003004
#define SYSTIMCHI     0x003008
#define SYSTIMC0      0x00300C
#define SYSTIMC1      0x003010
#define SYSTIMC2      0x003014
#define SYSTIMC3      0x003018

#define SYSTIMCS_MATCH0 0x1
#define SYSTIMCS_MATCH1 0x2
#define SYSTIMCS_MATCH2 0x4
#define SYSTIMCS_MATCH3 0x8

#define SYSTIMCLO_VAL REG(SYSTIMCLO)
#define SYSTIMC1_THR(val) REG(SYSTIMC1) = val
#define SYSTIMC1_MATCH REG(SYSTIMCS) & SYSTIMCS_MATCH1
#define SYSTIMC1_CLEAR REG(SYSTIMCS) |= SYSTIMCS_MATCH1

// Interrupts
#define IRQ_PEND        0x00B200
#define IRQ_PEND1       0x00B204
#define IRQ_PEND2       0x00B208
#define IRQ_FIQ_CTRL    0x00B20C
#define IRQ_EN_IRQ1     0x00B210
#define IRQ_EN_IRQ2     0x00B214
#define IRQ_EN_BIRQ     0x00B218
#define IRQ_DIS_IRQ1    0x00B21C
#define IRQ_DIS_IRQ2    0x00B220
#define IRQ_DIS_BIRQ    0x00B224

#define IRQ_SYSTIMC1    0x2
#define IRQ_ENABLE_SYSTIMC1 REG(IRQ_EN_IRQ1) |= IRQ_SYSTIMC1

#endif