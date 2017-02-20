#ifndef ADC_HH
#define ADC_HH

#include "hw.hh"
#include "os.hh"

BaseAddr constexpr ADCBase (int i) {
    return i == 0 ? MDR_ADC_BASE :  MDR_ADC_BASE;
}

class ADC_ {
public:
    struct CFG1{
        BITDEF(DelayAdc, BitAttr(28,4), 0);
        BITDEF(DelayGo, BitAttr(25,3), 0);
        BITDEF(TR, BitAttr(21,4), 0);
        BITDEF(SelVRef, BitAttr(20,1), 1);
        BITDEF(SelTS, BitAttr(19,1), 1);
        BITDEF(TSBufEn, BitAttr(18,1), 1);
        BITDEF(TSEn, BitAttr(17,1), 1);
        BITDEF(SyncConver, BitAttr(16,1), 1);
        static constexpr auto bfDivClk = BitAttr(12,4);
        struct DivClk {
            BITDEF(HCLK, bfDivClk, 0b0000);
            BITDEF(HCLK2, bfDivClk, 0b0001);
            BITDEF(HCLK4, bfDivClk, 0b0010);
            BITDEF(HCLK8, bfDivClk, 0b0011);
            BITDEF(HCLK16, bfDivClk, 0b0100);
            BITDEF(HCLK32, bfDivClk, 0b0101);
            BITDEF(HCLK64, bfDivClk, 0b0110);
            BITDEF(HCLK128, bfDivClk, 0b0111);
            BITDEF(HCLK256, bfDivClk, 0b1000);
            BITDEF(HCLK512, bfDivClk, 0b1001);
            BITDEF(HCLK1024, bfDivClk, 0b1010);
            BITDEF(HCLK2048, bfDivClk, 0b1011);
        };
        BITDEF(MRef, BitAttr(11,1), 1);
        BITDEF(RngC, BitAttr(10,1), 1);
        BITDEF(ChCh, BitAttr(9,1), 1);
        BITDEF(ChS, BitAttr(4,5), 0);
        BITDEF(Sample, BitAttr(3,1), 1);
        BITDEF(ClkS, BitAttr(2,1), 1);
        BITDEF(Go, BitAttr(1,1), 1);
        BITDEF(AdOn, BitAttr(0,1), 1);
    };
    struct CFG2{
        BITDEF(DelayGo, BitAttr(25,3), 0);
        BITDEF(ADC2OP, BitAttr(18,1), 1);
        BITDEF(ADC1OP, BitAttr(17,1), 1);
        static constexpr auto bfDivClk = BitAttr(12,4);
        struct DivClk {
            BITDEF(HCLK, bfDivClk, 0b0000);
            BITDEF(HCLK2, bfDivClk, 0b0001);
            BITDEF(HCLK4, bfDivClk, 0b0010);
            BITDEF(HCLK8, bfDivClk, 0b0011);
            BITDEF(HCLK16, bfDivClk, 0b0100);
            BITDEF(HCLK32, bfDivClk, 0b0101);
            BITDEF(HCLK64, bfDivClk, 0b0110);
            BITDEF(HCLK128, bfDivClk, 0b0111);
            BITDEF(HCLK256, bfDivClk, 0b1000);
            BITDEF(HCLK512, bfDivClk, 0b1001);
            BITDEF(HCLK1024, bfDivClk, 0b1010);
            BITDEF(HCLK2048, bfDivClk, 0b1011);
        };
        BITDEF(MRef, BitAttr(11,1), 1);
        BITDEF(RngC, BitAttr(10,1), 1);
        BITDEF(ChCh, BitAttr(9,1), 1);
        BITDEF(ChS, BitAttr(4,5), 1);
        BITDEF(Sample, BitAttr(3,1), 1);
        BITDEF(ClkS, BitAttr(2,1), 1);
        BITDEF(Go, BitAttr(1,1), 1);
        BITDEF(AdOn, BitAttr(0,1), 1);
    };
    struct Result{
        BITDEF(channel, BitAttr(16,5), 0);
        BITDEF(result, BitAttr(0,12), 0);
    };
    struct Status{
        BITDEF(EOCIE, BitAttr(4,5), 1);
        BITDEF(AWOIE, BitAttr(3,1), 1);
        BITDEF(EOCIF, BitAttr(2,1), 1);
        BITDEF(AWOIF, BitAttr(1,1), 1);
        BITDEF(Overwrite, BitAttr(0,1), 1);
    };
};

class ADC : public ADC_ {
public:
    uint32_t baseAddr;
    ADC(BaseAddr base) : baseAddr(base) {
        cfg = {RegMap(baseAddr + 0x00), RegMap(baseAddr + 0x04)};
        hLevel = {RegMap(baseAddr + 0x08), RegMap(baseAddr + 0x0c)};
        lLevel = {RegMap(baseAddr + 0x10), RegMap(baseAddr + 0x14)};
        result = {RegMap(baseAddr + 0x18), RegMap(baseAddr + 0x1c)};
        status = {RegMap(baseAddr + 0x20), RegMap(baseAddr + 0x24)};
        chSel = {RegMap(baseAddr + 0x28), RegMap(baseAddr + 0x2c)};
    }
    ADC(int base) : ADC(ADCBase(base)) {}
    array<RegMap, 2> cfg;
    array<RegMap, 2> hLevel;
    array<RegMap, 2> lLevel;
    array<RegMap, 2> result;
    array<RegMap, 2> status;
    array<RegMap, 2> chSel;
};

#define ADC12 ADC(0)

#endif // ADC_HH
