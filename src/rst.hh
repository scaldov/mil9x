#ifndef RST_HH
#define RST_HH

#include "hw.hh"
#include "os.hh"

BaseAddr constexpr RSTBase (int i) {
    return i == 0 ? MDR_RST_CLK_BASE :  MDR_RST_CLK_BASE;
}

class RST_ {
public:
    struct ClockStatus {
        BITDEF(HseRdy, BitAttr(2,1), 1);
        BITDEF(PllCpuRdy, BitAttr(1,1), 1);
        BITDEF(PllUsbRdy, BitAttr(0,1), 1);
    };
    struct PllControl {
        BITDEF(PllCpuMul, BitAttr(11,4), 0);
        BITDEF(PllUsbMul, BitAttr(4,4), 0);
        BITDEF(PllCpuRld, BitAttr(3,1), 1);
        BITDEF(PllCpuOn, BitAttr(2,1), 1);
        BITDEF(PllUsbRld, BitAttr(1,1), 1);
        BITDEF(PllUsbOn, BitAttr(0,1), 1);
    };
    struct HsControl {
        BITDEF(HseOn, BitAttr(1,1), 1);
        BITDEF(HseBypass, BitAttr(0,1), 1);
    };
    struct CpuClock {
        static constexpr auto bfHclkSel = BitAttr(8,2);
        struct HclkSel {
            BITDEF(HSI, bfHclkSel, 0);
            BITDEF(CpuC3, bfHclkSel, 1);
            BITDEF(LSE, bfHclkSel, 2);
            BITDEF(LSI, bfHclkSel, 3);
        };
        static constexpr auto bfCpuC3Sel = BitAttr(4,4);
        struct CpuC3Sel {
            BITDEF(CpuC2, bfCpuC3Sel, 0b0000);
            BITDEF(CpuC2div2, bfCpuC3Sel, 0b1000);
            BITDEF(CpuC2div4, bfCpuC3Sel, 0b1001);
            BITDEF(CpuC2div8, bfCpuC3Sel, 0b1010);
            BITDEF(CpuC2div16, bfCpuC3Sel, 0b1011);
            BITDEF(CpuC2div32, bfCpuC3Sel, 0b1100);
            BITDEF(CpuC2div64, bfCpuC3Sel, 0b1101);
            BITDEF(CpuC2div128, bfCpuC3Sel, 0b1110);
            BITDEF(CpuC2div256, bfCpuC3Sel, 0b1111);
        };
        static constexpr auto bfCpuC2Sel = BitAttr(2,1);
        struct CpuC2Sel {
            BITDEF(CpuC1, bfCpuC2Sel, 0);
            BITDEF(PllCpu, bfCpuC2Sel, 1);
        };
        static constexpr auto bfCpuC1Sel = BitAttr(0,2);
        struct CpuC1Sel {
            BITDEF(HSI, bfCpuC1Sel, 0);
            BITDEF(HSIdiv2, bfCpuC1Sel, 1);
            BITDEF(HSE, bfCpuC1Sel, 2);
            BITDEF(HSEdiv2, bfCpuC1Sel, 3);
        };
    };
    struct UsbClock {
        BITDEF(UsbClkEn, BitAttr(8,1), 1);
        static constexpr auto bfUsbC3Sel = BitAttr(4,1);
        struct UsbC3Sel {
            BITDEF(UsbC2, bfUsbC3Sel, 0);
            BITDEF(UsbC2div2, bfUsbC3Sel, 1);
        };
        static constexpr auto bfUsbC2Sel = BitAttr(2,1);
        struct UsbC2Sel {
            BITDEF(UsbC1, bfUsbC2Sel, 0);
            BITDEF(PllUsb, bfUsbC2Sel, 1);
        };
        static constexpr auto bfUsbC1Sel = BitAttr(0,2);
        struct UsbC1Sel {
            BITDEF(HSI, bfUsbC1Sel, 0);
            BITDEF(HSIdiv2, bfUsbC1Sel, 1);
            BITDEF(HSE, bfUsbC1Sel, 2);
            BITDEF(HSEdiv2, bfUsbC1Sel, 3);
        };
    };
    struct AdcMcoClock {
        BITDEF(AdcClkEn, BitAttr(13,1), 1);
        static constexpr auto bfAdcC3Sel = BitAttr(8,4);
        struct AdcC3Sel {
            BITDEF(AdcC2, bfAdcC3Sel, 0b0000);
            BITDEF(AdcC2div2, bfAdcC3Sel, 0b1000);
            BITDEF(AdcC2div4, bfAdcC3Sel, 0b1001);
            BITDEF(AdcC2div8, bfAdcC3Sel, 0b1010);
            BITDEF(AdcC2div16, bfAdcC3Sel, 0b1011);
            BITDEF(AdcC2div32, bfAdcC3Sel, 0b1100);
            BITDEF(AdcC2div64, bfAdcC3Sel, 0b1101);
            BITDEF(AdcC2div128, bfAdcC3Sel, 0b1110);
            BITDEF(AdcC2div256, bfAdcC3Sel, 0b1111);
        };
        static constexpr auto bfAdcC2Sel = BitAttr(4,2);
        struct AdcC2Sel {
            BITDEF(LSE, bfAdcC2Sel, 0);
            BITDEF(LSI, bfAdcC2Sel, 1);
            BITDEF(AdcC1, bfAdcC2Sel, 2);
            BITDEF(HsiC1, bfAdcC2Sel, 3);
        };
        static constexpr auto bfAdcC1Sel = BitAttr(0,2);
        struct AdcC1Sel {
            BITDEF(CpuC1, bfAdcC1Sel, 0);
            BITDEF(UsbC1, bfAdcC1Sel, 1);
            BITDEF(CpuC2, bfAdcC1Sel, 2);
            BITDEF(UsbC2, bfAdcC1Sel, 3);
        };
    };
    struct RtcClock {
        BITDEF(HsiRtcEn, BitAttr(9,1), 1);
        BITDEF(HseRtcEn, BitAttr(9,1), 1);
        static constexpr auto bfHsiSel = BitAttr(4,4);
        struct HsiSel {
            BITDEF(HsiC2, bfHsiSel, 0b0000);
            BITDEF(HsiC2div2, bfHsiSel, 0b1000);
            BITDEF(HsiC2div4, bfHsiSel, 0b1001);
            BITDEF(HsiC2div8, bfHsiSel, 0b1010);
            BITDEF(HsiC2div16, bfHsiSel, 0b1011);
            BITDEF(HsiC2div32, bfHsiSel, 0b1100);
            BITDEF(HsiC2div64, bfHsiSel, 0b1101);
            BITDEF(HsiC2div128, bfHsiSel, 0b1110);
            BITDEF(HsiC2div256, bfHsiSel, 0b1111);
        };
        static constexpr auto bfHseSel = BitAttr(0,4);
        struct HseSel {
            BITDEF(HseC2, bfHseSel, 0b0000);
            BITDEF(HseC2div2, bfHseSel, 0b1000);
            BITDEF(HseC2div4, bfHseSel, 0b1001);
            BITDEF(HseC2div8, bfHseSel, 0b1010);
            BITDEF(HseC2div16, bfHseSel, 0b1011);
            BITDEF(HseC2div32, bfHseSel, 0b1100);
            BITDEF(HseC2div64, bfHseSel, 0b1101);
            BITDEF(HseC2div128, bfHseSel, 0b1110);
            BITDEF(HseC2div256, bfHseSel, 0b1111);
        };
    };
    struct PerClock {
        BITDEF(CAN1, BitAttr(0,1), 1);
        BITDEF(CAN2, BitAttr(1,1), 1);
        BITDEF(USB, BitAttr(2,1), 1);
        BITDEF(EEPROM, BitAttr(3,1), 1);
        BITDEF(RST, BitAttr(4,1), 1);
        BITDEF(DMA, BitAttr(5,1), 1);
        BITDEF(UART1, BitAttr(6,1), 1);
        BITDEF(UART2, BitAttr(7,1), 1);
        BITDEF(SPI1, BitAttr(8,1), 1);
//        BITDEF(, BitAttr(9,1), 1);
        BITDEF(I2C1, BitAttr(10,1), 1);
        BITDEF(Power, BitAttr(11,1), 1);
        BITDEF(WWDT, BitAttr(12,1), 1);
        BITDEF(IWDT, BitAttr(13,1), 1);
        BITDEF(Timer1, BitAttr(14,1), 1);
        BITDEF(Timer2, BitAttr(15,1), 1);
        BITDEF(Timer3, BitAttr(16,1), 1);
        BITDEF(ADC, BitAttr(17,1), 1);
        BITDEF(DAC, BitAttr(18,1), 1);
        BITDEF(Comp, BitAttr(19,1), 1);
        BITDEF(SPI2, BitAttr(20,1), 1);
        BITDEF(PortA, BitAttr(21,1), 1);
        BITDEF(PortB, BitAttr(22,1), 1);
        BITDEF(PortC, BitAttr(23,1), 1);
        BITDEF(PortD, BitAttr(24,1), 1);
        BITDEF(PortE, BitAttr(25,1), 1);
//        BITDEF(, BitAttr(26,1), 1);
        BITDEF(BKP, BitAttr(27,1), 1);
//        BITDEF(, BitAttr(28,1), 1);
        BITDEF(PortF, BitAttr(29,1), 1);
        BITDEF(ExtBus, BitAttr(30,1), 1);
//        BITDEF(, BitAttr(31,1), 1);
    };
    struct CanClock {
        BITDEF(Can2ClkEn, BitAttr(25,1), 1);
        BITDEF(Can1ClkEn, BitAttr(24,1), 1);
        static constexpr auto bfCan2BRG = BitAttr(8,8);
        struct Can2BRG {
            BITDEF(HClk, bfCan2BRG, 0b000);
            BITDEF(HClkDiv2, bfCan2BRG, 0b001);
            BITDEF(HClkDiv4, bfCan2BRG, 0b010);
            BITDEF(HClkDiv8, bfCan2BRG, 0b011);
            BITDEF(HClkDiv16, bfCan2BRG, 0b100);
            BITDEF(HClkDiv32, bfCan2BRG, 0b101);
            BITDEF(HClkDiv64, bfCan2BRG, 0b110);
            BITDEF(HClkDiv128, bfCan2BRG, 0b111);
        };
        static constexpr auto bfCan1BRG = BitAttr(0,8);
        struct Can1BRG {
            BITDEF(HClk, bfCan1BRG, 0b000);
            BITDEF(HClkDiv2, bfCan1BRG, 0b001);
            BITDEF(HClkDiv4, bfCan1BRG, 0b010);
            BITDEF(HClkDiv8, bfCan1BRG, 0b011);
            BITDEF(HClkDiv16, bfCan1BRG, 0b100);
            BITDEF(HClkDiv32, bfCan1BRG, 0b101);
            BITDEF(HClkDiv64, bfCan1BRG, 0b110);
            BITDEF(HClkDiv128, bfCan1BRG, 0b111);
        };
    };
    struct TimClock {
        BITDEF(Tim3ClkEn, BitAttr(26,1), 1);
        BITDEF(Tim2ClkEn, BitAttr(25,1), 1);
        BITDEF(Tim1ClkEn, BitAttr(24,1), 1);
        static constexpr auto bfTim3BRG = BitAttr(16,8);
        struct Tim3BRG {
            BITDEF(HClk, bfTim3BRG, 0b000);
            BITDEF(HClkDiv2, bfTim3BRG, 0b001);
            BITDEF(HClkDiv4, bfTim3BRG, 0b010);
            BITDEF(HClkDiv8, bfTim3BRG, 0b011);
            BITDEF(HClkDiv16, bfTim3BRG, 0b100);
            BITDEF(HClkDiv32, bfTim3BRG, 0b101);
            BITDEF(HClkDiv64, bfTim3BRG, 0b110);
            BITDEF(HClkDiv128, bfTim3BRG, 0b111);
        };
        static constexpr auto bfTim2BRG = BitAttr(8,8);
        struct Tim2BRG {
            BITDEF(HClk, bfTim2BRG, 0b000);
            BITDEF(HClkDiv2, bfTim2BRG, 0b001);
            BITDEF(HClkDiv4, bfTim2BRG, 0b010);
            BITDEF(HClkDiv8, bfTim2BRG, 0b011);
            BITDEF(HClkDiv16, bfTim2BRG, 0b100);
            BITDEF(HClkDiv32, bfTim2BRG, 0b101);
            BITDEF(HClkDiv64, bfTim2BRG, 0b110);
            BITDEF(HClkDiv128, bfTim2BRG, 0b111);
        };
        static constexpr auto bfTim1BRG = BitAttr(0,8);
        struct Tim1BRG {
            BITDEF(HClk, bfTim1BRG, 0b000);
            BITDEF(HClkDiv2, bfTim1BRG, 0b001);
            BITDEF(HClkDiv4, bfTim1BRG, 0b010);
            BITDEF(HClkDiv8, bfTim1BRG, 0b011);
            BITDEF(HClkDiv16, bfTim1BRG, 0b100);
            BITDEF(HClkDiv32, bfTim1BRG, 0b101);
            BITDEF(HClkDiv64, bfTim1BRG, 0b110);
            BITDEF(HClkDiv128, bfTim1BRG, 0b111);
        };
    };
    struct UartClock {
        BITDEF(Uart2ClkEn, BitAttr(25,1), 1);
        BITDEF(Uart1ClkEn, BitAttr(24,1), 1);
        static constexpr auto bfUart2BRG = BitAttr(8,8);
        struct Uart2BRG {
            BITDEF(HClk, bfUart2BRG, 0b000);
            BITDEF(HClkDiv2, bfUart2BRG, 0b001);
            BITDEF(HClkDiv4, bfUart2BRG, 0b010);
            BITDEF(HClkDiv8, bfUart2BRG, 0b011);
            BITDEF(HClkDiv16, bfUart2BRG, 0b100);
            BITDEF(HClkDiv32, bfUart2BRG, 0b101);
            BITDEF(HClkDiv64, bfUart2BRG, 0b110);
            BITDEF(HClkDiv128, bfUart2BRG, 0b111);
        };
        static constexpr auto bfUart1BRG = BitAttr(0,8);
        struct Uart1BRG {
            BITDEF(HClk, bfUart1BRG, 0b000);
            BITDEF(HClkDiv2, bfUart1BRG, 0b001);
            BITDEF(HClkDiv4, bfUart1BRG, 0b010);
            BITDEF(HClkDiv8, bfUart1BRG, 0b011);
            BITDEF(HClkDiv16, bfUart1BRG, 0b100);
            BITDEF(HClkDiv32, bfUart1BRG, 0b101);
            BITDEF(HClkDiv64, bfUart1BRG, 0b110);
            BITDEF(HClkDiv128, bfUart1BRG, 0b111);
        };
    };
    struct SSPClock {
        BITDEF(SSP2ClkEn, BitAttr(25,1), 1);
        BITDEF(SSP1ClkEn, BitAttr(24,1), 1);
        static constexpr auto bfSSP2BRG = BitAttr(8,8);
        struct SSP2BRG {
            BITDEF(HClk, bfSSP2BRG, 0b000);
            BITDEF(HClkDiv2, bfSSP2BRG, 0b001);
            BITDEF(HClkDiv4, bfSSP2BRG, 0b010);
            BITDEF(HClkDiv8, bfSSP2BRG, 0b011);
            BITDEF(HClkDiv16, bfSSP2BRG, 0b100);
            BITDEF(HClkDiv32, bfSSP2BRG, 0b101);
            BITDEF(HClkDiv64, bfSSP2BRG, 0b110);
            BITDEF(HClkDiv128, bfSSP2BRG, 0b111);
        };
        static constexpr auto bfSSP1BRG = BitAttr(0,8);
        struct SSP1BRG {
            BITDEF(HClk, bfSSP1BRG, 0b000);
            BITDEF(HClkDiv2, bfSSP1BRG, 0b001);
            BITDEF(HClkDiv4, bfSSP1BRG, 0b010);
            BITDEF(HClkDiv8, bfSSP1BRG, 0b011);
            BITDEF(HClkDiv16, bfSSP1BRG, 0b100);
            BITDEF(HClkDiv32, bfSSP1BRG, 0b101);
            BITDEF(HClkDiv64, bfSSP1BRG, 0b110);
            BITDEF(HClkDiv128, bfSSP1BRG, 0b111);
        };
    };
};

class RST : public RST_ {
public:
    uint32_t baseAddr;
    RST(BaseAddr base) : baseAddr(base) {
        clockStatus = RegMap(baseAddr + 0x00);
        pllControl = RegMap(baseAddr + 0x04);
        hsControl = RegMap(baseAddr + 0x08);
        cpuClock = RegMap(baseAddr + 0x0c);
        usbClock = RegMap(baseAddr + 0x10);
        adcMcoClock = RegMap(baseAddr + 0x14);
        rtcClock = RegMap(baseAddr + 0x18);
        perClock = RegMap(baseAddr + 0x1c);
        canClock = RegMap(baseAddr + 0x20);
        timClock = RegMap(baseAddr + 0x24);
        uartClock = RegMap(baseAddr + 0x28);
        sspClock = RegMap(baseAddr + 0x2c);
    }
    RST(int base = 0) : RST(RSTBase(base)) {}
    RegMap clockStatus;
    RegMap pllControl;
    RegMap hsControl;
    RegMap cpuClock;
    RegMap usbClock;
    RegMap adcMcoClock;
    RegMap rtcClock;
    RegMap perClock;
    RegMap canClock;
    RegMap timClock;
    RegMap uartClock;
    RegMap sspClock;
};

#define ADC12 ADC(0)

#endif // RST_HH
