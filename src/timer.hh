#pragma once

#include "hw.hh"
#include "os.hh"

BaseAddr constexpr TimerBase (int i) {
    return i == 0 ? MDR_TIMER1_BASE :  i == 1 ? MDR_TIMER2_BASE : MDR_TIMER3_BASE;
}

class Timer_ {
public:
    struct Cntrl{
        static constexpr auto bfEventSel = BitAttr(8,4);
        struct EventSel {
            BITDEF(Front, bfEventSel, 0);
            BITDEF(ARR1, bfEventSel, 1);
            BITDEF(ARR2, bfEventSel, 2);
            BITDEF(ARR3, bfEventSel, 3);
            BITDEF(CHN1, bfEventSel, 4);
            BITDEF(CHN2, bfEventSel, 5);
            BITDEF(CHN3, bfEventSel, 6);
            BITDEF(CHN4, bfEventSel, 7);
            BITDEF(ETR, bfEventSel, 8);
        };
        static constexpr auto bfCntMode = BitAttr(6,2);
        class CntMode {
            BITDEF(ClkFixedDir, bfCntMode, 0);
            BITDEF(ClkAltDir, bfCntMode, 1);
            BITDEF(EvtFixedDir, bfCntMode, 2);
            BITDEF(EvtAltDir, bfCntMode, 3);
        };
        static constexpr auto bfFDTS = BitAttr(4,2);
        struct FDTS {
            BITDEF(Div1, bfFDTS, 0);
            BITDEF(Div2, bfFDTS, 1);
            BITDEF(Div3, bfFDTS, 2);
            BITDEF(Div4, bfFDTS, 3);
        };
        static constexpr auto bfDir = BitAttr(3,1);
        struct Dir {
            BITDEF(Up, bfDir, 0);
            BITDEF(Down, bfDir, 1);
        };
        static constexpr auto bfWrCmpl = BitAttr(2,1);
        struct WrCmpl {
            BITDEF(Ready, bfWrCmpl, 0);
            BITDEF(Busy, bfWrCmpl, 1);
        };
        static constexpr auto bfARRBEn = BitAttr(1,1);
        struct ARRBEn {
            BITDEF(Instant, bfARRBEn, 0);
            BITDEF(Delayed, bfARRBEn, 1);
        };
        static constexpr auto bfCntEn = BitAttr(0,1);
        struct CntEn {
            BITDEF(Disable, bfCntEn, 0);
            BITDEF(Enable, bfCntEn, 1);
        };
    };
    struct ChCntrl{
        static constexpr auto bfCapnPWM = BitAttr(15,1);
        struct CapnPWM {
            BITDEF(PWM, bfCapnPWM, 0);
            BITDEF(CAP, bfCapnPWM, 1);
        };
        static constexpr auto bfWRCmpl = BitAttr(14,1);
        struct WRCmpl {
            BITDEF(Busy, bfWRCmpl, 0);
            BITDEF(Ready, bfWRCmpl, 1);
        };
        static constexpr auto bfETREn = BitAttr(13,1);
        struct ETREn {
            BITDEF(Disable, bfETREn, 0);
            BITDEF(Enable, bfETREn, 1);
        };
        static constexpr auto bfBRKEn = BitAttr(12,1);
        struct BRKEn {
            BITDEF(Disable, bfBRKEn, 0);
            BITDEF(Enable, bfBRKEn, 1);
        };
        static constexpr auto bfOCCM = BitAttr(9,3);
        struct OCCM {
            BITDEF(Null, bfOCCM, 0);
            BITDEF(Eq1, bfOCCM, 1);
            BITDEF(Eq0, bfOCCM, 2);
            BITDEF(EqToggle, bfOCCM, 3);
            BITDEF(Zero, bfOCCM, 4);
            BITDEF(One, bfOCCM, 5);
            BITDEF(PosPWM, bfOCCM, 6);
            BITDEF(NegPWM, bfOCCM, 7);
        };
        static constexpr auto bfOCCE = BitAttr(8,1);
        struct OCCE {
            BITDEF(Disable, bfOCCE, 0);
            BITDEF(Enable, bfOCCE, 1);
        };
        static constexpr auto bfChPsc = BitAttr(6,2);
        struct ChPsc {
            BITDEF(Div1, bfChPsc, 0);
            BITDEF(Div2, bfChPsc, 1);
            BITDEF(Div4, bfChPsc, 2);
            BITDEF(Div8, bfChPsc, 3);
        };
        static constexpr auto bfChSel = BitAttr(4,2);
        struct ChSel {
            BITDEF(Pos, bfChSel, 0);
            BITDEF(Neg, bfChSel, 1);
            BITDEF(PosShift1, bfChSel, 2);
            BITDEF(PosShift2, bfChSel, 3);
        };
        static constexpr auto bfChFltr = BitAttr(0,4);
        struct ChFltr {
            BITDEF(T1TClk, bfChFltr, 0);
            BITDEF(T2TClk, bfChFltr, 1);
            BITDEF(T4TClk, bfChFltr, 2);
            BITDEF(T8TClk, bfChFltr, 3);
            BITDEF(T6FDTS2, bfChFltr, 4);
            BITDEF(T8FDTS2, bfChFltr, 5);
            BITDEF(T6FDTS4, bfChFltr, 6);
            BITDEF(T8FDTS4, bfChFltr, 7);
            BITDEF(T6FDTS8, bfChFltr, 8);
            BITDEF(T8FDTS8, bfChFltr, 9);
            BITDEF(T5FDTS16, bfChFltr, 10);
            BITDEF(T6FDTS16, bfChFltr, 11);
            BITDEF(T8FDTS16, bfChFltr, 12);
            BITDEF(T5FDTS32, bfChFltr, 13);
            BITDEF(T6FDTS32, bfChFltr, 14);
            BITDEF(T8FDTS32, bfChFltr, 15);
        };
    };
    struct ChCntrl1{
        static constexpr auto bfNInv = BitAttr(12,1);
        struct NInv {
            BITDEF(Enable, bfNInv, 0);
            BITDEF(Disable, bfNInv, 1);
        };
        static constexpr auto bfNSELO = BitAttr(10,2);
        struct NSELO {
            BITDEF(Zero, bfNSELO, 0);
            BITDEF(One, bfNSELO, 1);
            BITDEF(Ref, bfNSELO, 2);
            BITDEF(Dtg, bfNSELO, 3);
        };
        static constexpr auto bfNSELOE = BitAttr(8,2);
        struct NSELOE {
            BITDEF(Zero, bfNSELOE, 0);
            BITDEF(One, bfNSELOE, 1);
            BITDEF(Ref, bfNSELOE, 2);
            BITDEF(Dtg, bfNSELOE, 3);
        };
        static constexpr auto bfInv = BitAttr(4,1);
        struct Inv {
            BITDEF(Enable, bfInv, 0);
            BITDEF(Disable, bfInv, 1);
        };
        static constexpr auto bfSELO = BitAttr(2,2);
        struct SELO {
            BITDEF(Zero, bfSELO, 0);
            BITDEF(One, bfSELO, 1);
            BITDEF(Ref, bfSELO, 2);
            BITDEF(Dtg, bfSELO, 3);
        };
        static constexpr auto bfSELOE = BitAttr(0,2);
        struct SELOE {
            BITDEF(Zero, bfSELOE, 0);
            BITDEF(One, bfSELOE, 1);
            BITDEF(Ref, bfSELOE, 2);
            BITDEF(Dtg, bfSELOE, 3);
        };
    };
    struct ChCntrl2{
        static constexpr auto bfCCRRld = BitAttr(3,1);
        struct CCRRld {
            BITDEF(Always, bfCCRRld, 0);
            BITDEF(OnCntEq0, bfCCRRld, 1);
        };
        static constexpr auto bfCCR1En = BitAttr(2,1);
        struct CCR1En {
            BITDEF(Disable, bfCCR1En, 0);
            BITDEF(Enable, bfCCR1En, 1);
        };
        static constexpr auto bfChSel1 = BitAttr(0,2);
        struct ChSel1 {
            BITDEF(Pos, bfChSel1, 0);
            BITDEF(Neg, bfChSel1, 1);
            BITDEF(PosShift1, bfChSel1, 2);
            BITDEF(PosShift2, bfChSel1, 3);
        };
    };
    struct ChDTG{
        static constexpr auto bfDTG = BitAttr(8,8);
        BITDEF(DTG, bfDTG, 0);
        static constexpr auto bfEDTS = BitAttr(4,1);
        struct EDTS {
            BITDEF(TIMCLK, bfEDTS, 0);
            BITDEF(FDTS, bfEDTS, 1);
        };
        static constexpr auto bfDTGx = BitAttr(0,3);
        BITDEF(DTGx, bfDTGx, 0);
    };
    struct BrkEtrCntrl{
        static constexpr auto bfEtrFilter = BitAttr(4,4);
        struct EtrFilter {
            BITDEF(T1TClk, bfEtrFilter, 0);
            BITDEF(T2TClk, bfEtrFilter, 1);
            BITDEF(T4TClk, bfEtrFilter, 2);
            BITDEF(T8TClk, bfEtrFilter, 3);
            BITDEF(T6FDTS2, bfEtrFilter, 4);
            BITDEF(T8FDTS2, bfEtrFilter, 5);
            BITDEF(T6FDTS4, bfEtrFilter, 6);
            BITDEF(T8FDTS4, bfEtrFilter, 7);
            BITDEF(T6FDTS8, bfEtrFilter, 8);
            BITDEF(T8FDTS8, bfEtrFilter, 9);
            BITDEF(T5FDTS16, bfEtrFilter, 10);
            BITDEF(T6FDTS16, bfEtrFilter, 11);
            BITDEF(T8FDTS16, bfEtrFilter, 12);
            BITDEF(T5FDTS32, bfEtrFilter, 13);
            BITDEF(T6FDTS32, bfEtrFilter, 14);
            BITDEF(T8FDTS32, bfEtrFilter, 15);
        };
        static constexpr auto bfEtrPsc = BitAttr(2,2);
        struct EtrPsc {
            BITDEF(Div1, bfEtrPsc, 0);
            BITDEF(Div2, bfEtrPsc, 1);
            BITDEF(Div4, bfEtrPsc, 2);
            BITDEF(Div8, bfEtrPsc, 3);
        };
        static constexpr auto bfEtrInv = BitAttr(1,1);
        struct EtrInv {
            BITDEF(Disable, bfEtrInv, 0);
            BITDEF(Enable, bfEtrInv, 1);
        };
        static constexpr auto bfBrkInv = BitAttr(0,1);
        struct BrkInv {
            BITDEF(Disable, bfBrkInv, 0);
            BITDEF(Enable, bfBrkInv, 1);
        };
    };
    struct Status{
        static constexpr auto bfCCRCap1 = BitAttr(13,4);
        BITDEF(CCRCap1, bfCCRCap1, 0b1111);
        static constexpr auto bfCCRRef = BitAttr(9,4);
        BITDEF(CCRRef, bfCCRRef, 0b1111);
        static constexpr auto bfCCRCap = BitAttr(5,4);
        BITDEF(CCRCap, bfCCRCap, 0b1111);
        static constexpr auto bfBrk = BitAttr(4,1);
        BITDEF(Brk, bfBrk, 1);
        static constexpr auto bfETRFalling = BitAttr(3,1);
        BITDEF(ETRFalling, bfETRFalling, 1);
        static constexpr auto bfETRRising = BitAttr(2,1);
        BITDEF(ETRRising, bfETRRising, 1);
        static constexpr auto bfCntARR = BitAttr(1,1);
        BITDEF(CntARR, bfCntARR, 1);
        static constexpr auto bfCntZero = BitAttr(0,1);
        BITDEF(CntZero, bfCntZero, 1);
    };
    using IE = Status;
    using DMARE = Status;
};

class Timer : public Timer_ {
    uint32_t baseAddr;
public:
    Timer(BaseAddr base) : baseAddr(base) {
        cnt = RegMap(baseAddr + 0x00);
        psg = RegMap(baseAddr + 0x04);
        arr = RegMap(baseAddr + 0x08);
        cntrl = RegMap(baseAddr + 0x0C);
        ccr = {RegMap(baseAddr + 0x10), RegMap(baseAddr + 0x70),
               RegMap(baseAddr + 0x14), RegMap(baseAddr + 0x74),
               RegMap(baseAddr + 0x18), RegMap(baseAddr + 0x78),
               RegMap(baseAddr + 0x1c), RegMap(baseAddr + 0x7c)
              };
        chCntrl = {
            RegMap(baseAddr + 0x20), RegMap(baseAddr + 0x30), RegMap(baseAddr + 0x60),
            RegMap(baseAddr + 0x24), RegMap(baseAddr + 0x34), RegMap(baseAddr + 0x64),
            RegMap(baseAddr + 0x28), RegMap(baseAddr + 0x38), RegMap(baseAddr + 0x68),
            RegMap(baseAddr + 0x2c), RegMap(baseAddr + 0x3c), RegMap(baseAddr + 0x6c)
        };
        dtg = {RegMap(baseAddr + 0x40), RegMap(baseAddr + 0x44),
               RegMap(baseAddr + 0x48), RegMap(baseAddr + 0x4c)
              };
        brkEtrCntrl = RegMap(baseAddr + 0x50);
        status = RegMap(baseAddr + 0x54);
        ie = RegMap(baseAddr + 0x58);
        dmaRe = RegMap(baseAddr + 0x5c);
    }
    Timer(int base) : Timer(TimerBase(base)) {}
    RegMap cnt;
    RegMap psg;
    RegMap arr;
    RegMap cntrl;
    RegMap brkEtrCntrl;
    RegMap status;
    RegMap ie;
    RegMap dmaRe;
    array<RegMap, 4> dtg;
    array<array<RegMap, 2>, 4> ccr;
    array<array<RegMap, 3>, 4> chCntrl;
};
