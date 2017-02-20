#ifndef UART_HH
#define UART_HH


#include "hw.hh"
#include "os.hh"

BaseAddr constexpr UartBase (int i) {
    return i == 0 ? MDR_UART1_BASE : MDR_UART2_BASE;
}

class UART_ {
public:

    struct DR{
        BITDEF(OE, BitAttr(11,1), 1);
        BITDEF(BE, BitAttr(10,1), 1);
        BITDEF(PE, BitAttr(9,1), 1);
        BITDEF(FE, BitAttr(8,1), 1);
        BITDEF(DATA, BitAttr(0,8), 0);
    };

    struct ESR_ECR{
        BITDEF(OE, BitAttr(3,1), 1);
        BITDEF(BE, BitAttr(2,1), 1);
        BITDEF(PE, BitAttr(1,1), 1);
        BITDEF(FE, BitAttr(0,1), 1);
    };

    struct FR{
        BITDEF(RI, BitAttr(8,1), 1);
        BITDEF(TXFE, BitAttr(7,1), 1);
        BITDEF(RXFF, BitAttr(6,1), 1);
        BITDEF(TXFF, BitAttr(5,1), 1);
        BITDEF(RXFE, BitAttr(4,1), 1);
        BITDEF(BUSY, BitAttr(3,1), 1);
        BITDEF(DCD, BitAttr(2,1), 1);
        BITDEF(DSR, BitAttr(1,1), 1);
        BITDEF(CTS, BitAttr(0,1), 1);
    };

    struct ILPR{
        BITDEF(DVSR, BitAttr(0,8), 1);//not 0
    };


    struct IBRD{
        BITDEF(BAUDDIV_INT, BitAttr(0,16), 0);//целая часть коэффициента деления
    };

    struct FBRD{
        BITDEF(BAUDDIV_FRAC, BitAttr(0,6), 0);//дробная часть коэффициента деления
    };

    struct LCR_H{
        BITDEF(SPS, BitAttr(7,1), 1);

        static constexpr auto bfWLEN = BitAttr(5,2);
        struct WLEN {
            BITDEF(w5bit, bfWLEN, 0b00);
            BITDEF(w6bit, bfWLEN, 0b01);
            BITDEF(w7bit, bfWLEN, 0b10);
            BITDEF(w8bit, bfWLEN, 0b11);
        };

        BITDEF(FEN, BitAttr(4,1), 1);
        BITDEF(STP2, BitAttr(3,1), 1);
        BITDEF(EPS, BitAttr(2,1), 1);
        BITDEF(PEN, BitAttr(1,1), 1);
        BITDEF(BRK, BitAttr(0,1), 0);
    };

    struct CR{
        BITDEF(CTSEn, BitAttr(15,1), 1);
        BITDEF(RTSEn, BitAttr(14,1), 1);
        BITDEF(Out2, BitAttr(13,1), 1);
        BITDEF(Out1, BitAttr(12,1), 1);
        BITDEF(RTS, BitAttr(11,1), 1);
        BITDEF(DTR, BitAttr(10,1), 1);
        BITDEF(RXE, BitAttr(9,1), 1);
        BITDEF(TXE, BitAttr(8,1), 1);
        BITDEF(LBE, BitAttr(7,1), 1);
        BITDEF(SIRLP, BitAttr(2,1), 1);
        BITDEF(SIREn, BitAttr(1,1), 1);
        BITDEF(UARTEn, BitAttr(0,1), 1);
    };

    struct IFLS{
        static constexpr auto bfRXIF = BitAttr(3,3);
        struct RXIF {
            BITDEF(buf_div8, bfRXIF, 0b000);
            BITDEF(buf_div4, bfRXIF, 0b001);
            BITDEF(buf_div2, bfRXIF, 0b010);
            BITDEF(buf_3div4, bfRXF, 0b011);
            BITDEF(buf_7div8, bfRXF, 0b100);
        };
        static constexpr auto bfTXIF = BitAttr(0,3);
        struct TXIF {
            BITDEF(buf_div8, bfTXIF, 0b000);
            BITDEF(buf_div4, bfTXIF, 0b001);
            BITDEF(buf_div2, bfTXIF, 0b010);
            BITDEF(buf_divof3_4, bfTXF, 0b011);
            BITDEF(buf_divof7_8, bfTXF, 0b100);
        };
    };

    struct IMSC{
        BITDEF(OE, BitAttr(10,1), 1);
        BITDEF(BE, BitAttr(9,1), 1);
        BITDEF(PE, BitAttr(8,1), 1);
        BITDEF(FE, BitAttr(7,1), 1);
        BITDEF(RT, BitAttr(6,1), 1);
        BITDEF(TX, BitAttr(5,1), 1);
        BITDEF(RX, BitAttr(4,1), 1);
        BITDEF(DSRM, BitAttr(3,1), 1);
        BITDEF(DCDM, BitAttr(2,1), 1);
        BITDEF(CTSM, BitAttr(1,1), 1);
        BITDEF(RIM, BitAttr(0,1), 1);
    };
    using RIS = IMSC;
    using MIS = IMSC;
    using ICR = IMSC;

    struct DMACR{
        BITDEF(DMAONERR, BitAttr(2,1), 1);
        BITDEF(TXDMAE, BitAttr(1,1), 1);
        BITDEF(RXDMAE, BitAttr(0,1), 1);
    };


};



class UART : public UART_ {
public:
    uint32_t baseAddr;
    UART(BaseAddr base) : baseAddr(base) {
        dr = RegMap(baseAddr + 0x00);
        rsr_ecr = RegMap(baseAddr + 0x04);
        fr = RegMap(baseAddr + 0x18);
        ilpr = RegMap(baseAddr + 0x20);
        ibrd = RegMap(baseAddr + 0x24);
        fbrd = RegMap(baseAddr + 0x28);
        lcr_h = RegMap(baseAddr + 0x2C);
        cr = RegMap(baseAddr + 0x30);
        ifls = RegMap(baseAddr + 0x34);
        imsc = RegMap(baseAddr + 0x38);
        ris = RegMap(baseAddr + 0x3C);
        mis = RegMap(baseAddr + 0x40);
        icr = RegMap(baseAddr + 0x44);
        dmacr = RegMap(baseAddr + 0x48);

    }
    UART(int base) : UART(UARTBase(base)) {}
    RegMap dr;
    RegMap rsr_ecr;
    RegMap fr;
    RegMap ilpr;
    RegMap ibrd;
    RegMap fbrd;
    RegMap lcr_h;
    RegMap cr;
    RegMap ifls;
    RegMap imsc;
    RegMap ris;
    RegMap mis;
    RegMap icr;
    RegMap dmacr;
};


#endif // UART_HH
