
/** @file PID.h
 *  @author Andrey N. Zabegaev <speench@gmail.com>
 */

#ifndef _pid_h_
#define _pid_h_

#include <chrono>

class PID {
public:

    /** @brief Constructor
     */
    PID();
    
    /** @brief Destructor
     */
    virtual ~PID();
    
    /** @brief Initialize PID.
     *  @param[in] Kp proportional parameter
     *  @param[in] Ki integral parameter
     *  @param[in] Kd derivative parameter
     */
    void Init(double Kp, double Ki, double Kd);
    
    /** @brief Update the PID error variables given cross track error
     *  @param[in] cte cross track error
     */
    void UpdateError(double cte);
    
    /** @brief Calculate the total PID error
     *  @return control value
     */
    double TotalError();
    
    /** @brief Get accumulated error from last init
     *  @return square sum of cte from last init
     */
    double AccumError();

protected:
    
    double									_p_error;				//!< last sross track error
    double									_i_error;				//!< integral of cte
    double									_d_error;				//!< derivation of cte
    double									_Kp;					//!< proportional PID coefficient
    double									_Ki;;					//!< integral PID coefficient
    double									_Kd;;				//!< derivative PID coefficient
    double									_prev_cte;			//!< previouse value of cte
    bool									_prev_cte_init;		//!< flag of saving previouse cte
    double									_accumErr;			//!< accumulated square cte error
    std::chrono::steady_clock::time_point			_prevT;				//!< timestamp of previouse run
    
};

#endif /* _pid_h_ */
