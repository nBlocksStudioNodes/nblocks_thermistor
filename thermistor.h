#ifndef __NB_THERMISTOR
#define __NB_THERMISTOR

#include "nworkbench.h"

class nBlock_Thermistor: public nBlockSimpleNode<1> {
public:
    nBlock_Thermistor(PinName pinAdc, 
        float pullUpResistance = 10000.0, 
        float refResistance = 10000.0, 
        float refTemperature = 25.0, 
        float beta = 4000.0);
    void triggerInput(nBlocks_Message message);
    void endFrame();
private:
    AnalogIn _adc;
    float _pullup;
    float _ref_res;
    float _ref_temp;
    float _beta;
    float _inv_ref_temp;
    float _inv_beta;
    
    uint32_t _read_requested;
};





#endif
