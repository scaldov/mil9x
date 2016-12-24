#ifndef SSP_HH
#define SSP_HH

#include "hw.hh"
#include "os.hh"

BaseAddr constexpr SSPBase (int i) {
    return i == 0 ? MDR_SSP1_BASE :  MDR_SSP2_BASE;
}

class SSP_ {
public:

    struct CR0{
        BITDEF(SCR, BitAttr(8,8), 0);
        BITDEF(SPH, BitAttr(7,1), 1);
        BITDEF(SPO, BitAttr(6,1), 1);
        static constexpr auto bfFRF = BitAttr(4,2);
        struct FRF {
            BITDEF(SPI, bfFRF, 0b00);
            BITDEF(SSI, bfFRF, 0b01);
            BITDEF(Microwire, bfFRF, 0b10);
            BITDEF(Reserved, bfFRF, 0b11);
        };
        BITDEF(DSS, BitAttr(0,4), 7);
    };
    struct CR1{
        BITDEF(SOD, BitAttr(3,1), 1);
        BITDEF(MS, BitAttr(2,1), 1);
        BITDEF(SSE, BitAttr(1,1), 1);
        BITDEF(LBM, BitAttr(0,1), 1);
    };
    struct SR{
        BITDEF(BSY, BitAttr(4,1), 1);
        BITDEF(RFF, BitAttr(3,1), 1);
        BITDEF(RNE, BitAttr(2,1), 1);
        BITDEF(TNF, BitAttr(1,1), 1);
        BITDEF(TFE, BitAttr(0,1), 1);
    };
    struct CPSR{
        BITDEF(CPSDVSR, BitAttr(0,8), 2);
    };
    struct IMSC{
        BITDEF(TX, BitAttr(3,1), 1);
        BITDEF(RX, BitAttr(2,1), 1);
        BITDEF(RT, BitAttr(1,1), 1);
        BITDEF(ROR, BitAttr(0,1), 1);
    };
    using RIS = IMSC;
    using MIS = IMSC;
    using ICR = IMSC;
    struct DMACR{
        BITDEF(TXDMAE, BitAttr(1,1), 1);
        BITDEF(RXDMAE, BitAttr(0,1), 1);
    };

};

class SSP : public SSP_ {
public:
    uint32_t baseAddr;
    SSP(BaseAddr base) : baseAddr(base) {
        cr0 = RegMap(baseAddr + 0x00);
        cr1 = RegMap(baseAddr + 0x04);
        dr = RegMap(baseAddr + 0x08);
        sr = RegMap(baseAddr + 0x0c);
        cpsr = RegMap(baseAddr + 0x10);
        imsc = RegMap(baseAddr + 0x14);
        ris = RegMap(baseAddr + 0x18);
        mis = RegMap(baseAddr + 0x1c);
        icr = RegMap(baseAddr + 0x20);
        dmacr = RegMap(baseAddr + 0x24);
    }
    SSP(int base) : SSP(SSPBase(base)) {}
    RegMap cr0;
    RegMap cr1;
    RegMap dr;
    RegMap sr;
    RegMap cpsr;
    RegMap imsc;
    RegMap ris;
    RegMap mis;
    RegMap icr;
    RegMap dmacr;
};

//#define SSP1 SSPX(MDR_SSP1_BASE)
//#define SSP2 SSPX(MDR_SSP2_BASE)

#endif // SSP_HH
