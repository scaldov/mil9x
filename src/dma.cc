#include "dma.hh"
#include "ataka.hh"

uint32_t dma_adc_bfr[128];
uint32_t *dma_adc1_bfr_lo = dma_adc_bfr;
uint32_t *dma_adc1_bfr_hi = dma_adc_bfr + 32;
uint32_t *dma_adc2_bfr_lo = dma_adc_bfr + 64;
uint32_t *dma_adc2_bfr_hi = dma_adc_bfr + 96;

uint32_t dma_enable_channel;

DMA_::ControlData DMA_::controlTable[1 + DMA_ALTERNATE_DATA][DMA_ALIGN / sizeof(DMA_::ControlData)] __attribute__((aligned(DMA_ALIGN * 2)));

static int dma_adc1_request() {
    if(DMA().controlTable[0][DMA_::Chn::ADC1].control == DMA_::Ctrl::Cycle::Stop ||
            DMA().controlTable[1][DMA_::Chn::ADC1].control == DMA_::Ctrl::Cycle::Stop) {
        if(DMA().chnPriAltSet & (1<<DMA_::Chn::ADC1))
        {
            //DMA switched to alternative ctrl - prepare primary ctrl
            DMA().controlTable[0][DMA_::Chn::ADC1].control =
                    DMA_::Ctrl::Dst::Inc::Word | DMA_::Ctrl::Dst::Size::Word | DMA_::Ctrl::Src::Inc::None | DMA_::Ctrl::Src::Size::Word |
                    DMA_::Ctrl::Cycle::PingPong | DMA_::Ctrl::RPower(0) | DMA_::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[0][DMA_::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_lo + (32 - 1) * 4;
            DMA().chnPriAltClr = (1<<DMA_::Chn::ADC1);
            DMA().chnEnableSet = (1<<DMA_::Chn::ADC1);
        }  else {
            //DMA switched to primary ctrl - prepare alternative ctrl
            DMA().controlTable[1][DMA_::Chn::ADC1].control =
                    DMA_::Ctrl::Dst::Inc::Word | DMA_::Ctrl::Dst::Size::Word | DMA_::Ctrl::Src::Inc::None | DMA_::Ctrl::Src::Size::Word |
                    DMA_::Ctrl::Cycle::PingPong | DMA_::Ctrl::RPower(0) | DMA_::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[1][DMA_::Chn::ADC1].dstEndPtr = (uint8_t*)dma_adc1_bfr_hi + (32 - 1) * 4;
            DMA().chnPriAltSet = (1<<DMA_::Chn::ADC1);
            DMA().chnEnableSet = (1<<DMA_::Chn::ADC1);
        }
    }
    return (1<<DMA_::Chn::ADC1);
}

static int dma_adc2_request() {
    if(DMA().controlTable[0][DMA_::Chn::ADC2].control == DMA_::Ctrl::Cycle::Stop ||
            DMA().controlTable[1][DMA_::Chn::ADC2].control == DMA_::Ctrl::Cycle::Stop) {
        if(DMA().chnPriAltSet & (1<<DMA_::Chn::ADC2))
        {
            //DMA switched to alternative ctrl - prepare primary ctrl
            DMA().controlTable[0][DMA_::Chn::ADC2].control =
                    DMA_::Ctrl::Dst::Inc::Word | DMA_::Ctrl::Dst::Size::Word | DMA_::Ctrl::Src::Inc::None | DMA_::Ctrl::Src::Size::Word |
                    DMA_::Ctrl::Cycle::PingPong | DMA_::Ctrl::RPower(0) | DMA_::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[0][DMA_::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_lo + (32 - 1) * 4;
            DMA().chnPriAltClr = (1<<DMA_::Chn::ADC2);
            DMA().chnEnableSet = (1<<DMA_::Chn::ADC2);
        }  else {
            //DMA switched to primary ctrl - prepare alternative ctrl
            DMA().controlTable[1][DMA_::Chn::ADC2].control =
                    DMA_::Ctrl::Dst::Inc::Word | DMA_::Ctrl::Dst::Size::Word | DMA_::Ctrl::Src::Inc::None | DMA_::Ctrl::Src::Size::Word |
                    DMA_::Ctrl::Cycle::PingPong | DMA_::Ctrl::RPower(0) | DMA_::Ctrl::NMinus1(32 - 1);
            DMA().controlTable[1][DMA_::Chn::ADC2].dstEndPtr = (uint8_t*)dma_adc2_bfr_hi + (32 - 1) * 4;
            DMA().chnPriAltSet = (1<<DMA_::Chn::ADC2);
            DMA().chnEnableSet = (1<<DMA_::Chn::ADC2);
        }
    }
    return (1<<DMA_::Chn::ADC2);
}

extern "C" {
void DMA_IRQHandler() {
    //dma_active_channel;
    Reg<uint32_t> enable;
    Reg<uint32_t> done;
    enable = DMA().chnEnableSet;
    done = enable ^ dma_enable_channel;
    uint32_t channel_mask = 0;
    if(done & (1<<DMA_::Chn::TIM1)) {
        channel_mask |= ataka::dma_request();
    }
    if(done & (1<<DMA_::Chn::ADC1)) {
        channel_mask |= dma_adc1_request();
    }
    if(done & (1<<DMA_::Chn::ADC2)) {
        channel_mask |= dma_adc2_request();
    }
    DMA().errClr = DMA_::ErrClr;
    if(channel_mask)DMA().cfg |= DMA_::Cfg::MasterEnable;;
    return;
}
}
