#include "thermistor.h"

#define T_K  273.0

nBlock_Thermistor::nBlock_Thermistor(PinName pinAdc, 
        float pullUpResistance, 
        float refResistance, 
        float refTemperature, 
        float beta): _adc(pinAdc) {
    outputType[0] = OUTPUT_TYPE_FLOAT;
    _pullup = pullUpResistance;
    _ref_res = refResistance;
    _ref_temp = refTemperature + T_K;
    _beta = beta;
    
    _inv_ref_temp = 1.0/_ref_temp;
    _inv_beta = 1.0/_beta;
    
    _read_requested = 0;
    return;
}

void nBlock_Thermistor::triggerInput(nBlocks_Message message) {
    _read_requested = 1;
}
    

void nBlock_Thermistor::endFrame() {
    if (_read_requested) {
        _read_requested = 0;
        
        // This is a normalized value respective to Vdd
        // (in other words, adc_read=0.3 means 0.3*Vdd)
        float adc_read = _adc.read();
        // Relative current = I / Vdd (normalized to Vdd)
        // It is the current in [A] if Vdd was 1.0V
        float relative_current = (1.0-adc_read)/_pullup;
        float therm_R = adc_read / relative_current;
        
        // This node uses the beta equation:
        // 1/T = 1/TO + (1/β) . ln (R/RO)
        float inv_T = _inv_ref_temp + _inv_beta * log(therm_R / _ref_res);
        float result = (1.0/inv_T) - T_K;
        
        output[0] = PackFloat(result);
        available[0] = 1;
    }
}

