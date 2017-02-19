#pragma once

#include "hw.hh"
#include "os.hh"

BaseAddr constexpr PortBase (int i) {
    return i == 0 ? MDR_PORTA_BASE :  i == 1 ? MDR_PORTB_BASE :
                                               i == 2 ? MDR_PORTC_BASE : i == 3 ? MDR_PORTD_BASE :
                                                                                  i == 4 ? MDR_PORTE_BASE : MDR_PORTF_BASE;
}

class Port_ {
public:
    enum class Func{
        Port = 0,  Basic = 1, Alternated = 2, Overridden = 3
    };
    static constexpr int A = 0;
    static constexpr int B = 1;
    static constexpr int C = 2;
    static constexpr int D = 3;
    static constexpr int E = 4;
    static constexpr int F = 5;
};

class Port : public Port_ {
public:
    uint32_t baseAddr;
    Port(BaseAddr base) : baseAddr(base) {
        rxtx = RegMap(baseAddr + 0x00);
        oe = RegMap(baseAddr + 0x04);
        func = RegMap(baseAddr + 0x08);
        analog = RegMap(baseAddr + 0x0c);
        pull = RegMap(baseAddr + 0x10);
        pd = RegMap(baseAddr + 0x14);
        pwr = RegMap(baseAddr + 0x18);
        gfEn = RegMap(baseAddr + 0x1c);
    }
    Port(int base) : Port(PortBase(base)) {}
    RegMap rxtx;
    RegMap oe;
    RegMap func;
    RegMap analog;
    RegMap pull;
    RegMap pd;
    RegMap pwr;
    RegMap gfEn;
};

#define PortA Port(Port::A)
#define PortB Port(Port::B)
#define PortC Port(Port::C)
#define PortD Port(Port::D)
#define PortE Port(Port::E)
#define PortF Port(Port::F)


class IOConf_ {
public:
    enum class OE{
        In = 0,  Out = 1
    };
    enum class Func{
        Port = 0,  Basic = 1, Alternated = 2, Overridden = 3
    };
    enum class Analog{
        Analog = 0,  Digital = 1
    };
    enum class PullUp{
        Off = 0,  On = 1
    };
    enum class PullDown{
        Off= 0,  On = 1
    };
    enum class Driver{
        On = 0,  OpenDrain = 1
    };
    enum class Schmitt{
        Off = 0,  On = 1
    };
    enum class PWR{
        Reserved = 0,  Slow = 1, Fast = 2, MaxFast = 3
    };
    enum class GF{
        Off = 0,  On = 1
    };
    class Conf {
        //pack config into bits
        // port | pin   | gf | pwr | schm | drv | pd | pu | ana | fn  | oe |
        //23-20 | 19-16 | 10 | 9-8 |   7  |  6  |  5 |  4 |  3  | 2-1 | 0  |
    public:
        uint32_t value;
        constexpr Conf(const int port, const int pin, const OE oe, const Func func, const Analog analog,
                       const PullUp pullup, const PullDown pulldown, const Driver driver,
                       const Schmitt schmitt, const PWR pwr, const GF gf) :
            value( ((port & 0b111) << 20) | ((pin & 0b1111) << 16) |
                   ((int)oe << 0) | ((int)func << 1) | ((int)analog << 3) | ((int)pullup << 4) |
                   ((int)pulldown << 5) | ((int)driver << 6) | ((int)schmitt << 7) | ((int)pwr << 8) | ((int)gf << 10) )
        {}
        constexpr Conf(const uint32_t v) : value(v){}
        constexpr uint32_t port() {return (value >> 20) & 0b111;}
        constexpr uint32_t pin() {return (value >> 16) & 0b1111;}
        constexpr uint32_t gf() {return (value >> 10) & 0b1;}
        constexpr uint32_t pwr() {return (value >> 8) & 0b11;}
        constexpr uint32_t schmitt() {return (value >> 7) & 0b1;}
        constexpr uint32_t driver() {return (value >> 6) & 0b1;}
        constexpr uint32_t pulldown() {return (value >> 5) & 0b1;}
        constexpr uint32_t pullup() {return (value >> 4) & 0b1;}
        constexpr uint32_t analog() {return (value >> 3) & 0b1;}
        constexpr uint32_t func() {return (value >> 1) & 0b11;}
        constexpr uint32_t oe() {return (value >> 0) & 0b1;}
    };
    static void config(Conf & conf) {
        int pin = conf.pin();
        uint32_t mask = 0b1 << pin;
        Reg32 t; //temporary register value
        t = Port(conf.port()).oe;
        Port(conf.port()).oe = (t & ~mask) | (conf.oe() ? mask : 0);
//        PortX(conf.port()).oe = ((int)PortX(conf.port()).oe & ~mask) | (conf.oe() ? mask : 0);
        t = Port(conf.port()).func;
        Port(conf.port()).func = (t & ~(0b11 << (pin * 2))) | (conf.func() << (pin * 2));
        t = Port(conf.port()).analog;
        Port(conf.port()).analog = (t & ~mask) | (conf.analog() ? mask : 0);
        t = Port(conf.port()).pull;
        Port(conf.port()).pull = (t & ~(mask | (mask << 16))) | (conf.pulldown() ? mask : 0) | (conf.pullup() ? (mask << 16) : 0);
        t = Port(conf.port()).pd;
        Port(conf.port()).pd = (t & ~(mask | (mask << 16))) | (conf.driver() ? mask : 0) | (conf.schmitt() ? (mask << 16) : 0);
        t = Port(conf.port()).pwr;
        Port(conf.port()).pwr = (t & ~(0b11 << (pin * 2))) | (conf.pwr() << (pin * 2));
        t = Port(conf.port()).gfEn;
        Port(conf.port()).gfEn = (t & ~mask) | (conf.gf() ? mask : 0);
    }
    static void Config(Conf conf) {
        Conf cnf(conf);
        config(cnf);
    }
//    static void Config1(Conf const &conf) {
//        int pin = conf.pin();
//        uint32_t mask = 0b1 << pin;
//        uint32_t t; //temporary register value
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->OE;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->OE = (t & ~mask) | (conf.oe() ? mask : 0);
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->FUNC;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->FUNC = (t & ~(0b11 << (pin * 2))) | (conf.func() << (pin * 2));
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->ANALOG;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->ANALOG = (t & ~mask) | (conf.analog() ? mask : 0);
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PULL;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PULL = (t & ~(mask | (mask << 16))) | (conf.pulldown() ? mask : 0) | (conf.pullup() ? (mask << 16) : 0);
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PD;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PD = (t & ~(mask | (mask << 16))) | (conf.driver() ? mask : 0) | (conf.schmitt() ? (mask << 16) : 0);
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PWR;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->PWR = (t & ~(0b11 << (pin * 2))) | (conf.pwr() << (pin * 2));
//        t = ((MDR_PORT_TypeDef*) PortBase(conf.port()))->GFEN;
//        ((MDR_PORT_TypeDef*) PortBase(conf.port()))->GFEN = (t & ~mask) | (conf.gf() ? mask : 0);
//    }
};

struct IOConf : IOConf_{
    struct SSP1 {
        struct RX {
            static constexpr auto B14 = Conf(Port_::B, 14, OE::In, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D11 = Conf(Port_::D, 11, OE::In, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E12 = Conf(Port_::E, 12, OE::In, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F3 = Conf(Port_::F, 3, OE::In, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TX {
            static constexpr auto B15 = Conf(Port_::B, 15, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D12 = Conf(Port_::D, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F0 = Conf(Port_::F, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CLK {
            static constexpr auto B13 = Conf(Port_::B, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D10 = Conf(Port_::D, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F1 = Conf(Port_::F, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct FSS {
            static constexpr auto B12 = Conf(Port_::B, 12, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D9 = Conf(Port_::D, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E13 = Conf(Port_::E, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F2 = Conf(Port_::F, 2, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct SSP2 {
        struct RX {
            static constexpr auto B14 = Conf(Port_::B, 14, OE::In, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C2 = Conf(Port_::C, 2, OE::In, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C15 = Conf(Port_::C, 15, OE::In, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D2 = Conf(Port_::D, 2, OE::In, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F14 = Conf(Port_::F, 14, OE::In, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TX {
            static constexpr auto B15 = Conf(Port_::B, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C3 = Conf(Port_::C, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D6 = Conf(Port_::D, 6, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F15 = Conf(Port_::F, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CLK {
            static constexpr auto B13 = Conf(Port_::B, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C1 = Conf(Port_::C, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D5 = Conf(Port_::D, 5, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F13 = Conf(Port_::F, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct FSS {
            static constexpr auto B12 = Conf(Port_::B, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C0 = Conf(Port_::C, 0, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C14 = Conf(Port_::C, 14, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D3 = Conf(Port_::D, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F12 = Conf(Port_::F, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct Timer1 {
        struct CH1 {
            static constexpr auto F6 = Conf(Port_::D, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C4 = Conf(Port_::C, 4, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D1 = Conf(Port_::D, 1, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A1 = Conf(Port_::A, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH1N {
            static constexpr auto F7 = Conf(Port_::F, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C5 = Conf(Port_::C, 5, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D0 = Conf(Port_::D, 0, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A2 = Conf(Port_::A, 2, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2 {
            static constexpr auto F8 = Conf(Port_::F, 8, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C6 = Conf(Port_::C, 6, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D10 = Conf(Port_::D, 10, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A3 = Conf(Port_::A, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2N {
            static constexpr auto F9 = Conf(Port_::F, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C7 = Conf(Port_::C, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D11 = Conf(Port_::D, 11, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A4 = Conf(Port_::A, 4, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3 {
            static constexpr auto F10 = Conf(Port_::F, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C8 = Conf(Port_::C, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D12 = Conf(Port_::D, 12, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A5 = Conf(Port_::A, 5, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3N {
            static constexpr auto F11 = Conf(Port_::F, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C9 = Conf(Port_::C, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D13 = Conf(Port_::D, 13, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A8 = Conf(Port_::A, 8, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4 {
            static constexpr auto F12 = Conf(Port_::F, 12, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C10 = Conf(Port_::C, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D14 = Conf(Port_::D, 14, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A9 = Conf(Port_::A, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4N {
            static constexpr auto F13 = Conf(Port_::F, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C11 = Conf(Port_::C, 11, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D8 = Conf(Port_::D, 8, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A13 = Conf(Port_::A, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct ETR {
            static constexpr auto F14 = Conf(Port_::F, 14, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C12 = Conf(Port_::C, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D4 = Conf(Port_::D, 4, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A15 = Conf(Port_::A, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct BLK {
            static constexpr auto F15 = Conf(Port_::F, 15, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C13 = Conf(Port_::C, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D7 = Conf(Port_::D, 7, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto A14 = Conf(Port_::A, 14, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct Timer2 {
        struct CH1 {
            static constexpr auto A1 = Conf(Port_::A, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D8 = Conf(Port_::D, 8, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E0 = Conf(Port_::E, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH1N {
            static constexpr auto A2 = Conf(Port_::A, 2, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D9 = Conf(Port_::D, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E1 = Conf(Port_::E, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2 {
            static constexpr auto A3 = Conf(Port_::A, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D10 = Conf(Port_::D, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E9 = Conf(Port_::E, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2N {
            static constexpr auto A4 = Conf(Port_::A, 4, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D11 = Conf(Port_::D, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E10 = Conf(Port_::E, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3 {
            static constexpr auto A5 = Conf(Port_::A, 5, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D12 = Conf(Port_::D, 12, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E2 = Conf(Port_::E, 2, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3N {
            static constexpr auto A8 = Conf(Port_::A, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D13 = Conf(Port_::D, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E3 = Conf(Port_::E, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4 {
            static constexpr auto A9 = Conf(Port_::A, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D14 = Conf(Port_::D, 14, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E8 = Conf(Port_::E, 8, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4N {
            static constexpr auto A10 = Conf(Port_::A, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E4 = Conf(Port_::E, 4, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct ETR {
            static constexpr auto A12 = Conf(Port_::A, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D5 = Conf(Port_::D, 5, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E14 = Conf(Port_::E, 14, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct BLK {
            static constexpr auto A11 = Conf(Port_::A, 11, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D6 = Conf(Port_::D, 6, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E5 = Conf(Port_::E, 5, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct Timer3 {
        struct CH1 {
            static constexpr auto B0 = Conf(Port_::B, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C2 = Conf(Port_::C, 2, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D0 = Conf(Port_::D, 0, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E2 = Conf(Port_::E, 2, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F7 = Conf(Port_::F, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH1N {
            static constexpr auto B1 = Conf(Port_::B, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C3 = Conf(Port_::C, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D1 = Conf(Port_::D, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E3 = Conf(Port_::E, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F8 = Conf(Port_::F, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2 {
            static constexpr auto B2 = Conf(Port_::B, 2, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C4 = Conf(Port_::C, 4, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D2 = Conf(Port_::D, 2, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E4 = Conf(Port_::E, 4, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F9 = Conf(Port_::F, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH2N {
            static constexpr auto B3 = Conf(Port_::B, 3, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C5 = Conf(Port_::C, 5, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D3 = Conf(Port_::D, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E5 = Conf(Port_::E, 5, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F10 = Conf(Port_::F, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3 {
            static constexpr auto B5 = Conf(Port_::B, 5, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C6 = Conf(Port_::C, 6, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E6 = Conf(Port_::E, 6, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH3N {
            static constexpr auto B6 = Conf(Port_::B, 6, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C7 = Conf(Port_::C, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E7 = Conf(Port_::E, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4 {
            static constexpr auto B7 = Conf(Port_::B, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C8 = Conf(Port_::C, 8, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E8 = Conf(Port_::E, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CH4N {
            static constexpr auto B8 = Conf(Port_::B, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C9 = Conf(Port_::C, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E9 = Conf(Port_::E, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct ETR {
            static constexpr auto B4 = Conf(Port_::B, 4, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C10 = Conf(Port_::C, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E10 = Conf(Port_::E, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F11 = Conf(Port_::F, 11, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct BLK {
            static constexpr auto B4 = Conf(Port_::B, 4, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C11 = Conf(Port_::C, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D4 = Conf(Port_::D, 4, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E11 = Conf(Port_::E, 11, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct ExtInt1 {
        static constexpr auto A0 = Conf(Port_::A, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        static constexpr auto B11 = Conf(Port_::B, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
    };
    struct ExtInt2 {
        static constexpr auto B10 = Conf(Port_::B, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        static constexpr auto C12 = Conf(Port_::C, 12, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
    };
    struct ExtInt3 {
        static constexpr auto D15 = Conf(Port_::D, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        static constexpr auto E15 = Conf(Port_::E, 15, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
    };
    struct ExtInt4 {
        static constexpr auto B9 = Conf(Port_::B, 9, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        static constexpr auto C13 = Conf(Port_::C, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
    };
    struct UART1 {
        struct RXD {
            static constexpr auto A6 = Conf(Port_::A, 6, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto B6 = Conf(Port_::B, 6, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D7 = Conf(Port_::D, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E12 = Conf(Port_::E, 12, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TXD {
            static constexpr auto A7 = Conf(Port_::A, 7, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto B0 = Conf(Port_::B, 0, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto B5 = Conf(Port_::B, 5, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D8 = Conf(Port_::D, 8, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E13 = Conf(Port_::E, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct SIR {
            struct In {
                static constexpr auto B9 = Conf(Port_::B, 9, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
                static constexpr auto E11 = Conf(Port_::E, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            };
            struct Out {
                static constexpr auto B7 = Conf(Port_::B, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
                static constexpr auto B10 = Conf(Port_::B, 10, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            };
        };
        struct DTR {
            static constexpr auto A10 = Conf(Port_::A, 10, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct RTS {
            static constexpr auto A11 = Conf(Port_::A, 11, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct RI {
            static constexpr auto A12 = Conf(Port_::A, 12, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct DCD {
            static constexpr auto A13 = Conf(Port_::A, 13, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct DSR {
            static constexpr auto A14 = Conf(Port_::A, 14, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct CTS {
            static constexpr auto A15 = Conf(Port_::A, 15, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct UART2 {
        struct RXD {
            static constexpr auto B1 = Conf(Port_::B, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D0 = Conf(Port_::D, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F0 = Conf(Port_::F, 0, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TXD {
            static constexpr auto D1 = Conf(Port_::D, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F1 = Conf(Port_::F, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct SIR {
            struct In {
                static constexpr auto D7 = Conf(Port_::D, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            };
            struct Out {
                static constexpr auto D4 = Conf(Port_::D, 4, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            };
        };
    };
    struct CAN1 {
        struct RX {
            static constexpr auto A7 = Conf(Port_::A, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto B3 = Conf(Port_::B, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C9 = Conf(Port_::C, 9, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D14 = Conf(Port_::D, 14, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E0 = Conf(Port_::E, 0, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TX {
            static constexpr auto A6 = Conf(Port_::A, 6, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto B2 = Conf(Port_::B, 2, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto C8 = Conf(Port_::C, 8, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D13 = Conf(Port_::D, 13, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E1 = Conf(Port_::E, 1, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct CAN2 {
        struct RX {
            static constexpr auto C14 = Conf(Port_::C, 14, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D15 = Conf(Port_::D, 15, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E6 = Conf(Port_::E, 6, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F2 = Conf(Port_::F, 2, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct TX {
            static constexpr auto C15 = Conf(Port_::C, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto D9 = Conf(Port_::D, 9, OE::Out, Func::Basic, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E7 = Conf(Port_::E, 7, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto F3 = Conf(Port_::F, 3, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
    struct IIC {
        struct SCL {
            static constexpr auto C0 = Conf(Port_::C, 0, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E14 = Conf(Port_::E, 14, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
        struct SDA {
            static constexpr auto C1 = Conf(Port_::C, 1, OE::Out, Func::Alternated, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
            static constexpr auto E15 = Conf(Port_::E, 15, OE::Out, Func::Overridden, Analog::Digital, PullUp::On, PullDown::Off, Driver::On, Schmitt::Off, PWR::MaxFast, GF::Off);
        };
    };
};

//static auto SSP1 = SSP(0);
//using alias SSP1 = SSP(0);
