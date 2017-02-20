#include "dma.hh"
#include "ataka.hh"

uint32_t dma_adc_bfr[128];
uint32_t *dma_adc1_bfr_lo = dma_adc_bfr;
uint32_t *dma_adc1_bfr_hi = dma_adc_bfr + 32;
uint32_t *dma_adc2_bfr_lo = dma_adc_bfr + 64;
uint32_t *dma_adc2_bfr_hi = dma_adc_bfr + 96;

uint32_t dma_enable_channel;

DMA::ControlData DMA::controlTable[1 + DMA_ALTERNATE_DATA][DMA_ALIGN / sizeof(DMA_::ControlData)] __attribute__((aligned(DMA_ALIGN * 2)));

static int dma_adc1_request() {
    if(DMA().controlTable[0][DMA::Chn::ADC1].control == DMA::Ctrl::Cycle::Stop ||
            DMA().controlTable[1][DMA::Chn::ADC1].control == DMA::Ctrl::Cycle::Stop) {
        if((int)DMA().chnPriAltSet & (1<<DMA::Chn::ADC1))
        {
            //DMA switched to alternative ctrl - prepare primary ctrl
            DMA().controlTable[0][DMA::Chn::ADC1].control =
                    DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                    DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[0][DMA::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_lo + (32 - 1) * 4;
            DMA().chnPriAltClr = (1<<DMA::Chn::ADC1);
            DMA().chnEnableSet = (1<<DMA::Chn::ADC1);
        }  else {
            //DMA switched to primary ctrl - prepare alternative ctrl
            DMA().controlTable[1][DMA::Chn::ADC1].control =
                    DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                    DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[1][DMA::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_hi + (32 - 1) * 4;
            DMA().chnPriAltSet = (1<<DMA::Chn::ADC1);
            DMA().chnEnableSet = (1<<DMA::Chn::ADC1);
        }
    }
    return (1<<DMA::Chn::ADC1);
}

static int dma_adc2_request() {
    if(DMA().controlTable[0][DMA::Chn::ADC2].control == DMA::Ctrl::NMinus1(0))
    {
        //DMA switched to alternative ctrl - prepare primary ctrl
        DMA().controlTable[0][DMA::Chn::ADC2].control =
                DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[0][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_lo + (32 - 1) * 4;
//        DMA().chnPriAltClr = (1<<DMA::Chn::ADC2);
        DMA().chnEnableSet = (1<<DMA::Chn::ADC2);
    }
    if(DMA().controlTable[1][DMA::Chn::ADC2].control == DMA::Ctrl::NMinus1(0)){
        //DMA switched to primary ctrl - prepare alternative ctrl
        DMA().controlTable[1][DMA::Chn::ADC2].control =
                DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
        DMA().controlTable[1][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_hi + (32 - 1) * 4;
//        DMA().chnPriAltSet = (1<<DMA::Chn::ADC2);
        DMA().chnEnableSet = (1<<DMA::Chn::ADC2);
    }
    return (1<<DMA::Chn::ADC2);
}

static int dma_adc2_requestz() {
    if(DMA().controlTable[0][DMA::Chn::ADC2].control == DMA::Ctrl::Cycle::Stop ||
            DMA().controlTable[1][DMA::Chn::ADC2].control == DMA::Ctrl::Cycle::Stop) {

        //if((int)DMA().chnPriAltSet & (1<<DMA::Chn::ADC2))
        if(MDR_DMA->CHNL_PRI_ALT_SET & (1<<DMA::Chn::ADC2))
        {
            //DMA switched to alternative ctrl - prepare primary ctrl
            DMA().controlTable[0][DMA::Chn::ADC2].control =
                    DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                    DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[0][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_lo + (32 - 1) * 4;
//            DMA().chnPriAltClr = (1<<DMA::Chn::ADC2);
            DMA().chnEnableSet = (1<<DMA::Chn::ADC2);
        }  else {
            //DMA switched to primary ctrl - prepare alternative ctrl
            DMA().controlTable[1][DMA::Chn::ADC2].control =
                    DMA::Ctrl::Dst::Inc::Word | DMA::Ctrl::Dst::Size::Word | DMA::Ctrl::Src::Inc::None | DMA::Ctrl::Src::Size::Word |
                    DMA::Ctrl::Cycle::PingPong | DMA::Ctrl::RPower(0) | DMA::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[1][DMA::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_hi + (32 - 1) * 4;
//            DMA().chnPriAltSet = (1<<DMA::Chn::ADC2);
            DMA().chnEnableSet = (1<<DMA::Chn::ADC2);
        }
    }
    return (1<<DMA::Chn::ADC2);
}

//typedef struct{
//    uint32_t stat;
//    uint32_t adc2l;
//    uint32_t adc2h;
//    uint32_t tmr1;
//} dma_stat;

dma_stat dma_log[32];

extern "C" {
void DMA_IRQHandler() {
    static int ix = 1;
    //dma_active_channel;
    Reg32 enable;
    Reg32 done;
    enable = DMA().chnEnableSet;
    if (ix < 32) {
        uint32_t l = (int)DMA().controlTable[0][DMA::Chn::ADC2].control;
        uint32_t h = (int)DMA().controlTable[1][DMA::Chn::ADC2].control;
        if((l & 7) != (dma_log[ix-1].adc2l & 7) || (h & 7) != (dma_log[ix-1].adc2h & 7)){
            dma_log[ix].stat = (int)enable;
            dma_log[ix].adc2l = l;
            dma_log[ix].adc2h = h;
            dma_log[ix].tmr1 = (int)DMA().chnEnableSet;
            ix ++;
        }
    }
    done = enable ^ dma_enable_channel;
    uint32_t channel_mask = 0;
    if(done & (1<<DMA::Chn::TIM1)) {
        channel_mask |= ataka::dma_request();
    }
    if((done & (1<<DMA::Chn::ADC1)) | 1) {
        channel_mask |= dma_adc1_request();
    }
    if((done & (1<<DMA::Chn::ADC2)) | 1) {
        channel_mask |= dma_adc2_request();
    }
    DMA().errClr = DMA::ErrClr;
    if(channel_mask)DMA().cfg |= DMA::Cfg::MasterEnable;;
    return;
}
}
