#pragma once

#include "hw.hh"
#include "os.hh"

#ifndef DMA_CHANNELS_NUMBER
#define DMA_CHANNELS_NUMBER 32
#endif // DMA_CHANNELS_NUMBER
#ifndef DMA_ALTERNATE_DATA
#define DMA_ALTERNATE_DATA 1
#endif // DMA_ALTERNATE_DATA

#if (DMA_CHANNELS_NUMBER  > (1 << 4))
#define DMA_ALIGN (1 << (4 + 5))
#elif (DMA_CHANNELS_NUMBER  > (1 << 3))
#define DMA_ALIGN (1 << (3 + 5))
#elif (DMA_CHANNELS_NUMBER  > (1 << 2))
#define DMA_ALIGN (1 << (2 + 5))
#elif (DMA_CHANNELS_NUMBER  > (1 << 1))
#define DMA_ALIGN (1 << (1 + 5))
#elif (DMA_CHANNELS_NUMBER  > (1 << 0))
#define DMA_ALIGN (1 << (0 + 5))
#else
#define DMA_ALIGN (1 << (-1 + 5))
#endif

extern uint32_t *dma_adc1_bfr_lo;
extern uint32_t *dma_adc1_bfr_hi;
extern uint32_t *dma_adc2_bfr_lo;
extern uint32_t *dma_adc2_bfr_hi;

extern uint32_t dma_enable_channel;

BaseAddr constexpr DMABase (int i) {
    return i == 0 ? MDR_DMA_BASE :  MDR_DMA_BASE;
}

class DMA_ {
public:
    struct Status{
        static constexpr auto bfTestStatus = BitAttr(28,4);
        struct TestStatus {
            BITDEF(NoITC, bfTestStatus, 0);
            BITDEF(ITC, bfTestStatus, 1);
            BITDEF(Undef, bfTestStatus, 2);
        };
        static constexpr auto bfChnlsMinus1 = BitAttr(16,5);
        BITDEF(ChnlsMinus1, bfChnlsMinus1, 0);
        static constexpr auto bfState = BitAttr(4,4);
        struct State {
            BITDEF(Idle, bfState, 0b0000);
            BITDEF(ReadCtrl, bfState, 0b0001);
            BITDEF(ReadSrcEndPtr, bfState, 0b0010);
            BITDEF(ReadDstEndPtr, bfState, 0b0011);
            BITDEF(ReadSrcData, bfState, 0b0100);
            BITDEF(WriteSrcData, bfState, 0b0101);
            BITDEF(WaitReq, bfState, 0b0110);
            BITDEF(WriteCtrl, bfState, 0b0111);
            BITDEF(Stop, bfState, 0b1000);
            BITDEF(Ready, bfState, 0b1001);
            BITDEF(SG, bfState, 0b1010);
            BITDEF(Undef, bfState, 0b1011);
        };
        static constexpr auto bfMasterEnable = BitAttr(0,1);
        BITDEF(MasterEnable, bfMasterEnable, 1);
    };
    struct Cfg{
        static constexpr auto bfChnProtCtrl = BitAttr(4,4);
        struct ChnProtCtrl {
            BITDEF(Cached, BitAttr(7,1), 1);
            BITDEF(Bufferized, BitAttr(6,1), 1);
            BITDEF(Privileged, BitAttr(5,1), 1);
        };
        static constexpr auto bfMasterEnable = BitAttr(0,1);
        BITDEF(MasterEnable, bfMasterEnable, 1);
    };
    static constexpr auto bfErrClr = BitAttr(0,1);
    BITDEF(ErrClr, bfErrClr, 1);
    // control structure bits
    struct Ctrl{
        static constexpr auto bfChnProtCtrl = BitAttr(4, 4);
        struct Dst{
            static constexpr auto bfInc = BitAttr(30, 2);
            struct Inc{
                BITDEF(Byte, bfInc, 0);
                BITDEF(HalfWord, bfInc, 1);
                BITDEF(Word, bfInc, 2);
                BITDEF(None, bfInc, 3);
            };
            static constexpr auto bfSize = BitAttr(28, 2);
            struct Size{
                BITDEF(Byte, bfSize, 0);
                BITDEF(HalfWord, bfSize, 1);
                BITDEF(Word, bfSize, 2);
                BITDEF(None, bfSize, 3);
            };
            static constexpr auto bfProt = BitAttr(21, 3);
            struct Prot {
                BITDEF(Cached, BitAttr(23, 1), 1);
                BITDEF(Bufferized, BitAttr(22, 1), 1);
                BITDEF(Privileged, BitAttr(21, 1), 1);
            };
        };
        struct Src{
            static constexpr auto bfInc = BitAttr(26, 2);
            struct Inc{
                BITDEF(Byte, bfInc, 0);
                BITDEF(HalfWord, bfInc, 1);
                BITDEF(Word, bfInc, 2);
                BITDEF(None, bfInc, 3);
            };
            static constexpr auto bfSize = BitAttr(24, 2);
            struct Size{
                BITDEF(Byte, bfSize, 0);
                BITDEF(HalfWord, bfSize, 1);
                BITDEF(Word, bfSize, 2);
                BITDEF(None, bfSize, 3);
            };
            static constexpr auto bfProt = BitAttr(18, 3);
            struct Prot {
                BITDEF(Cached, BitAttr(20, 1), 1);
                BITDEF(Bufferized, BitAttr(19, 1), 1);
                BITDEF(Privileged, BitAttr(18, 1), 1);
            };
        };
        static constexpr auto bfRPower = BitAttr(14,4);
        BITDEF(RPower, bfRPower, 1);
        static constexpr auto bfNMinus1 = BitAttr(4,10);
        BITDEF(NMinus1, bfNMinus1, 0);
        BITDEF(NextUseBurst, BitAttr(3,1), 1);
        static constexpr auto bfCycle = BitAttr(0,3);
        struct Cycle{
            BITDEF(Stop, bfCycle, 0b000);
            BITDEF(Basic, bfCycle, 0b001);
            BITDEF(AutoReq, bfCycle, 0b010);
            BITDEF(PingPong, bfCycle, 0b011);
            BITDEF(MemSGPri, bfCycle, 0b100);
            BITDEF(MemSGAlt, bfCycle, 0b101);
            BITDEF(PeriphSGPri, bfCycle, 0b110);
            BITDEF(PeriphSGAlt, bfCycle, 0b111);
        };
    };
    enum Chn : uint32_t {
        UART1_TX = 0,
        UART1_RX, UART2_TX, UART2_RX,
        SSP1_TX, SSP1_RX, SSP2_TX, SSP2_RX,
        ADC1, ADC2,
        TIM1, TIM2, TIM3
    };
    struct ControlData{
        Reg32 srcEndPtr;
        Reg32 dstEndPtr;
        Reg32 control;
        uint32_t unused;
    };
};

class DMA : public DMA_{
public:
    uint32_t baseAddr;
    DMA(BaseAddr base) : baseAddr(base) {
        status = RegMap(baseAddr + 0x00);
        cfg = RegMap(baseAddr + 0x04);
        ctrlBasePtr = RegMap(baseAddr + 0x08);
        altCtrlBasePtr = RegMap(baseAddr + 0x0c);
        waitOnReqStatus = RegMap(baseAddr + 0x10);
        chnSwRequest = RegMap(baseAddr + 0x14);
        chnUseBurstSet = RegMap(baseAddr + 0x18);
        chnUseBurstClr = RegMap(baseAddr + 0x1c);
        chnReqMaskSet = RegMap(baseAddr + 0x20);
        chnReqMaskClr = RegMap(baseAddr + 0x24);
        chnEnableSet = RegMap(baseAddr + 0x28);
        chnEnableClr = RegMap(baseAddr + 0x2c);
        chnPriAltSet = RegMap(baseAddr + 0x30);
        chnPriAltClr = RegMap(baseAddr + 0x34);
        chnPrioritySet = RegMap(baseAddr + 0x38);
        chnPriorityClr = RegMap(baseAddr + 0x3c);
        errClr = RegMap(baseAddr + 0x4c);
    }
    DMA(int base = 0) : DMA(DMABase(base)) {}
    RegMap status;
    RegMap cfg;
    RegMap ctrlBasePtr;
    RegMap altCtrlBasePtr;
    RegMap waitOnReqStatus;
    RegMap chnSwRequest;
    RegMap chnUseBurstSet;
    RegMap chnUseBurstClr;
    RegMap chnReqMaskSet;
    RegMap chnReqMaskClr;
    RegMap chnEnableSet;
    RegMap chnEnableClr;
    RegMap chnPriAltSet;
    RegMap chnPriAltClr;
    RegMap chnPrioritySet;
    RegMap chnPriorityClr;
    RegMap errClr;
    static ControlData controlTable[1 + DMA_ALTERNATE_DATA][DMA_ALIGN / sizeof(ControlData)] __attribute__((aligned(DMA_ALIGN * 2)));
};
