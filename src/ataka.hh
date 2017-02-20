#ifndef __ATAKA_HH__
#define __ATAKA_HH__

#include "hw.hh"
#include "os.hh"

#include "pin.hh"
#include "timer.hh"

#include <string.h>

extern lk::pin led4;

/*
    * PA0  LINE0
    * PA1  LINE1
    * PA2  CHNWRK
    * PA3  WRDSTART
    * PA4  WRDSTOP
    * PA5  XCHGSTOP
    * PA6  RDY
*/

class ataka {
public:
    static ataka *intance;
    static xSemaphoreHandle sem_isr;
    static xSemaphoreHandle sem_res;
    enum signals {
        LINE0 = 0, LINE1, CHNWRK, WRDSTART,
        WRDSTOP, XCHGSTOP, RDY, MAX_SIGNALS
    };
    static constexpr auto dmaGPIOConfig = DMA::Ctrl::Dst::Inc::None | DMA::Ctrl::Dst::Size::Byte | DMA::Ctrl::Src::Inc::Byte | DMA::Ctrl::Src::Size::Byte |
            DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0);
    static constexpr auto dmaADCConfig = DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
            DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0);
    struct DMAbuffer {
        uint8_t buffer[40 * 4];
        int len;
        void init(){
            for(unsigned int i = 0; i < sizeof(buffer) / 4; i ++) ((int*)buffer)[i] = 0b0;
            len = sizeof(buffer);
        }
        //flags 1<<CHNWRK 1<<XCHGSTOP
        void encode(uint32_t word, int flag) {
            int p = 0;
            if(flag & (1 << CHNWRK)) {
                buffer[p++] = (1 << CHNWRK);
                buffer[p++] = 0;
                buffer[p++] = 0;
                buffer[p++] = 0;
            }
            buffer[p++] = (1 << WRDSTART);
            buffer[p++] = 0;
            buffer[p++] = 0;
            buffer[p++] = 0;
            auto c = word;
            //feed word bits
            for(int i = 0; i < 32; i ++) {
                if(c & 0x00000001) buffer[p++] = (1 << LINE1);
                else buffer[p++] = (1 << LINE0);
                buffer[p++] = 0;
                buffer[p++] = 0;
                buffer[p++] = 0;
                c = (c >> 1);
            }
            c = word % 3;
            //feed mod3 checksum
            for(int i = 0; i < 3; i ++) {
                if(c & 0x00000001) buffer[p++] = (1 << LINE1);
                else buffer[p++] = (1 << LINE0);
                buffer[p++] = 0;
                buffer[p++] = 0;
                buffer[p++] = 0;
                c = (c >> 1);
            }
            buffer[p++] = (1 << WRDSTOP);
            if(flag & (1 << XCHGSTOP)) {
                buffer[p++] = (1 << XCHGSTOP);
                buffer[p++] = 0;
                buffer[p++] = 0;
                buffer[p++] = 0;
            }
            len = p;
        }
    };
    DMAbuffer bfr_lo;
    DMAbuffer bfr_hi;
    DMAbuffer *bfr_front, *bfr_back;
    ataka() {
        intance = this;
    }
    ~ataka() {

    }
    static void task_ataka_emu(void *param) {
        ataka *_this = (ataka*) param;
        uint32_t msg1[] = {0x31323334};
        uint32_t msg2[] = {0x55AA55AA, 0x00FF00FF};
        uint32_t msg3[] = {1, 2, 3, 4};
        for(;;) {
            _this->send(msg1, 1);
            _this->send(msg2, 2);
            _this->send(msg3, 4);
        }
    }
    static int dma_request() {
        static BaseType_t task;
        if(DMA().controlTable[0][DMA::Chn::TIM1].control == DMA::Ctrl::Cycle::Stop ||
                DMA().controlTable[1][DMA::Chn::TIM1].control == DMA::Ctrl::Cycle::Stop) {
            if(DMA().chnPriAltSet & (1<<DMA::Chn::TIM1))
            {
                //DMA switched to alternative ctrl - prepare primary ctrl
                DMA().controlTable[0][DMA::Chn::TIM1].control = dmaGPIOConfig | DMA::Ctrl::NMinus1(ataka::intance->bfr_back->len - 1);
                DMA().controlTable[0][DMA::Chn::TIM1].dstEndPtr = &MDR_PORTA->RXTX;
                DMA().controlTable[0][DMA::Chn::TIM1].srcEndPtr = (uint8_t*)ataka::intance->bfr_back + (ataka::intance->bfr_back->len - 1) * 1;
                DMA().chnPriAltClr = (1<<DMA::Chn::TIM1);
                DMA().chnEnableSet = (1<<DMA::Chn::TIM1);
                //switch buffer pointers;
                DMAbuffer *tmp_bfr = ataka::intance->bfr_front;
                ataka::intance->bfr_front = ataka::intance->bfr_back;
                ataka::intance->bfr_back = tmp_bfr;
            }  else {
                //DMA switched to primary ctrl - prepare alternative ctrl
                DMA().controlTable[1][DMA::Chn::TIM1].control = dmaGPIOConfig | DMA::Ctrl::NMinus1(ataka::intance->bfr_back->len - 1);
                DMA().controlTable[1][DMA::Chn::TIM1].dstEndPtr = &MDR_PORTA->RXTX;
                DMA().controlTable[1][DMA::Chn::TIM1].srcEndPtr = (uint8_t*)ataka::intance->bfr_back->buffer + (ataka::intance->bfr_back->len - 1) * 1;
                DMA().chnPriAltSet = (1<<DMA::Chn::TIM1);
                DMA().chnEnableSet = (1<<DMA::Chn::TIM1);
                //switch buffer pointers;
                DMAbuffer *tmp_bfr = ataka::intance->bfr_front;
                ataka::intance->bfr_front = ataka::intance->bfr_back;
                ataka::intance->bfr_back = tmp_bfr;
            }
        }
        xSemaphoreGiveFromISR(sem_isr, &task);
        if(task) {
            portYIELD_FROM_ISR(&task);
        }
        return (1<<DMA::Chn::TIM1);
    }

    void init() {
        vSemaphoreCreateBinary(sem_isr);
        vSemaphoreCreateBinary(sem_res);
        // initialize PORTA as output for ATAKA signals
//        PortXX[Port::A].pull = 0;
//        PortXX[0].pull = 0;
//        PortXX[0].analog = 0xFFFF;
        PORT_InitTypeDef port;
        PORT_StructInit(&port);
        port.PORT_Pin   = (1 << MAX_SIGNALS) - 1;
        port.PORT_OE    = PORT_OE_OUT;
        port.PORT_FUNC  = PORT_FUNC_PORT;
        port.PORT_MODE = PORT_MODE_DIGITAL;
        port.PORT_PULL_UP = PORT_PULL_UP_OFF;
        port.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        port.PORT_PD_SHM = PORT_PD_SHM_OFF;
        port.PORT_PD = PORT_PD_DRIVER;
        port.PORT_GFEN = PORT_GFEN_OFF;
        port.PORT_SPEED = PORT_SPEED_MAXFAST;
        PORT_Init(MDR_PORTA, &port);
        // initialize PORTC 10 as PWM T1CH4 output for debug purpose
        port.PORT_Pin   = PORT_Pin_10;
        port.PORT_OE    = PORT_OE_OUT;
        port.PORT_FUNC  = PORT_FUNC_OVERRID;
        port.PORT_PULL_UP = PORT_PULL_UP_ON;
        port.PORT_PD = PORT_PD_DRIVER;
        port.PORT_MODE = PORT_MODE_DIGITAL;
        MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTD;
        PORT_Init(MDR_PORTC, &port);
        port.PORT_Pin   = 0b1000001111111111;
        port.PORT_OE    = PORT_OE_IN;
        port.PORT_MODE = PORT_MODE_ANALOG;
        port.PORT_FUNC  = PORT_FUNC_MAIN;
        port.PORT_PULL_UP = PORT_PULL_UP_OFF;
        port.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
        port.PORT_PD = PORT_PD_DRIVER;
        PORT_Init(MDR_PORTD, &port);
//        IOConf::Config1(IOConf::SSP1::MISO::B14);
        IOConf::Config(IOConf::SSP1::RX::B14);
        //enable SSP and disable their DMA requests due to error
        MDR_RST_CLK->PER_CLOCK |= (RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2);
//        SSP1.dmacr &= ~(SSP_::DMACR::RXDMAE | SSP_::DMACR::TXDMAE);
        SSP(1).dmacr &= ~(SSP_::DMACR::RXDMAE | SSP_::DMACR::TXDMAE);
        SSP(2).dmacr &= ~(SSP_::DMACR::RXDMAE | SSP_::DMACR::TXDMAE);
        //initialize ADC
        MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_ADC;
        MDR_RST_CLK->ADC_MCO_CLOCK= (0b10 << RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Pos) |
                (0b10 << RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Pos) |
                (0b1111 << RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Pos) |
                RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN;
        ADC12.chSel[0] = 0b11000000000000000000000000000000;
        ADC12.chSel[1] = 0b00000000000000001111111111111111;
        ADC12.cfg[0] = ADC_::CFG1::Go | ADC_::CFG1::ClkS | ADC_::CFG1::Sample |
                ADC_::CFG1::ChCh | ADC_::CFG1::TSEn | ADC_::CFG1::TSBufEn | ADC_::CFG1::SelTS |
                ADC_::CFG1::DelayGo(1) | ADC_::CFG1::DelayAdc(1) | ADC_::CFG1::DivClk::HCLK16;
        ADC12.cfg[1] = ADC_::CFG2::Go | ADC_::CFG2::ClkS | ADC_::CFG2::Sample |
                ADC_::CFG2::ChCh | ADC_::CFG2::ADC1OP | ADC_::CFG2::ADC2OP |
                ADC_::CFG2::DelayGo(1) | ADC_::CFG2::DivClk::HCLK16;
        //initialize DMA
        MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_DMA;
        DMA().chnUseBurstSet = ~0;
        DMA().chnReqMaskSet = ~0;
        DMA().chnEnableClr = ~0;
        DMA().chnPriorityClr = ~0;
        //init timer DMA channel
        DMA().ctrlBasePtr = (uint32_t)DMA().controlTable;
        DMA().chnSwRequest = 0;
        //init DMA timer buffers
        bfr_lo.init();
        bfr_hi.init();
        bfr_front = &bfr_lo;
        bfr_back = &bfr_hi;
        //init ctrl structs for TIMER1 channel
        //primary
        DMA().controlTable[0][DMA::Chn::TIM1].control = dmaGPIOConfig | DMA::Ctrl::NMinus1(bfr_lo.len - 1);
        DMA().controlTable[0][DMA::Chn::TIM1].dstEndPtr = &MDR_PORTA->RXTX;
        DMA().controlTable[0][DMA::Chn::TIM1].srcEndPtr = (uint8_t*)bfr_lo.buffer + (bfr_lo.len - 1) * 1;
        //alternative
        DMA().controlTable[1][DMA::Chn::TIM1].control = dmaGPIOConfig | DMA::Ctrl::NMinus1(bfr_hi.len - 1);
        DMA().controlTable[1][DMA::Chn::TIM1].dstEndPtr = &MDR_PORTA->RXTX;
        DMA().controlTable[1][DMA::Chn::TIM1].srcEndPtr = (uint8_t*)bfr_hi.buffer + (bfr_hi.len - 1) * 1;
        //init ADC DMA channels
        //ADC1
        //primary
        DMA().controlTable[0][DMA::Chn::ADC1].control = dmaADCConfig | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[0][DMA::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_lo + (32 - 1) * 4;
        DMA().controlTable[0][DMA::Chn::ADC1].srcEndPtr = &MDR_ADC->ADC1_RESULT;
        //alternative
        DMA().controlTable[1][DMA::Chn::ADC1].control = dmaADCConfig | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[1][DMA::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_hi + (32 - 1) * 4;
        DMA().controlTable[1][DMA::Chn::ADC1].srcEndPtr = &MDR_ADC->ADC1_RESULT;
        //ADC2
        //primary
        DMA().controlTable[0][DMA::Chn::ADC2].control = dmaADCConfig | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[0][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_lo + (32 - 1) * 4;
        DMA().controlTable[0][DMA::Chn::ADC2].srcEndPtr = &MDR_ADC->ADC2_RESULT;
        //alternative
        DMA().controlTable[1][DMA::Chn::ADC2].control = dmaADCConfig | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[1][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_hi + (32 - 1) * 4;
        DMA().controlTable[1][DMA::Chn::ADC2].srcEndPtr = &MDR_ADC->ADC2_RESULT;
        //
        DMA().chnUseBurstClr = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        DMA().chnReqMaskClr = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        DMA().chnEnableSet = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        DMA().chnPriAltClr = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        DMA().chnPrioritySet = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        dma_enable_channel = (1<<DMA::Chn::ADC1) | (1<<DMA::Chn::ADC2) | (1<<DMA::Chn::TIM1);
        //initialize timer to feed DMA
        //timer 1
        MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1;
        MDR_RST_CLK->TIM_CLOCK = (MDR_RST_CLK->TIM_CLOCK & ~RST_CLK_TIM_CLOCK_TIM1_BRG_Msk) | (0 << RST_CLK_TIM_CLOCK_TIM1_BRG_Pos) | RST_CLK_TIM_CLOCK_TIM1_CLK_EN;
        Timer(0).cntrl = 0;
        Timer(0).cnt = 0;
        Timer(0).psg = 0;
        Timer(0).arr = 128_MHz / 4_MHz - 1;
        Timer(0).ccr[3][0] = 128_MHz / 4_MHz / 8;
        Timer(0).ccr[3][1] = 128_MHz / 4_MHz / 8 * 3;
        Timer(0).chCntrl[3][0] = Timer::ChCntrl::OCCM::PosPWM;
        Timer(0).chCntrl[3][1] = Timer::ChCntrl1::NSELO::Ref | Timer::ChCntrl1::NSELOE::One | Timer::ChCntrl1::SELO::Ref | Timer::ChCntrl1::SELOE::One;
        Timer(0).chCntrl[3][2] = Timer::ChCntrl2::CCR1En::Enable;
        Timer(0).chCntrl[0][0] = 0;
        Timer(0).chCntrl[0][1] = 0;
        Timer(0).chCntrl[1][0] = 0;
        Timer(0).chCntrl[1][1] = 0;
        Timer(0).chCntrl[2][0] = 0;
        Timer(0).chCntrl[2][1] = 0;
        Timer(0).dmaRe = Timer_::IE::CntARR(1);
        Timer(0).ie = 0;
        Timer(0).cntrl = Timer_::Cntrl::CntEn::Enable;
        NVIC->ICPR[0] = 0xFFFFFFFF;
        NVIC->ICER[0] = 0xFFFFFFFF;
        NVIC_ClearPendingIRQ(DMA_IRQn);
        NVIC_SetPriority(DMA_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
        NVIC_EnableIRQ(DMA_IRQn);
        DMA().cfg |= DMA::Cfg::MasterEnable;
        ADC12.cfg[0] |= ADC_::CFG1::AdOn;
        ADC12.cfg[1] |= ADC_::CFG2::AdOn;
    }
    void send(uint32_t *bfr, int cnt) {
        xSemaphoreTake(sem_res , portMAX_DELAY);
        //transmit data
        for(int i = 0; i < cnt; i ++) {
            xSemaphoreTake(sem_isr, portMAX_DELAY);
            led4.toggle();
            bfr_back->encode(bfr[i], i == 0? (1<<CHNWRK) : i == (cnt - 1) ? (1<<XCHGSTOP) : 0);
        }
        //flush front and back buffers after transmitting data
        xSemaphoreTake(sem_isr, portMAX_DELAY);
        led4.toggle();
        bfr_back->init();
        xSemaphoreTake(sem_isr, portMAX_DELAY);
        led4.toggle();
        bfr_back->init();
        xSemaphoreGive(sem_res);
    }
};


#endif // __ATAKA_HH__
