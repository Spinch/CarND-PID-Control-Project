
/** @file PID.cpp
 *  @author Andrey N. Zabegaev <speench@gmail.com>
 */

#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    _Kp = Kp;
    _Kd = Kd;
    _Ki = Ki;
    
    _i_error = 0;
    
    _prev_cte_init = false;
    
    _accumErr = 0;
    
    _prevT = std::chrono::steady_clock::now();
    
    return;
}

void PID::UpdateError(double cte) {
    _p_error = cte;
    
    if (!_prev_cte_init) {
	_prev_cte = cte;
	_prev_cte_init = true;
    }
    
    std::chrono::steady_clock::time_point currentT = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentT-_prevT);
    _prevT = currentT;
    double dt = elapsed.count() / 1000.;
    
    _d_error = (cte - _prev_cte)/dt;
    _prev_cte = cte;
    
    _i_error += cte*dt;
    
    _accumErr += cte*cte;
    
    return;
}

double PID::TotalError() {
    return -_Kp*_p_error - _Kd*_d_error - _Ki*_i_error;
}

double PID::AccumError() {
    return _accumErr;
}
