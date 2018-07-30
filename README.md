# PID Controller project
Self-Driving Car Engineer Nanodegree Program

---

## Overview

In this project I'he utilize PID controller to control a car moving on the track. Controller receives cross track error and produce steering angle.

Also in this project I have implemented twiddle algorithm to get best estimation of PID controller parameters.


## Compile

This project require [uWebSocketIO](https://github.com/uWebSockets/uWebSockets) to be installed. This repository includes two files that can be used to set up and install it for either Linux or Mac systems.

Once the install for uWebSocketIO is complete, the main program can be built by doing the following from the project top directory.

1. mkdir build
2. cd build
3. cmake ..
4. make

Dependencies:

* cmake >= 3.5
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
* gcc/g++ >= 5.4


## Running the Code

This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases)


To run the PID controller algorithm use

```

./build/pid 
```

command in project directory.


## Reflection

Here I'll write some thoughts about PID controller behavior and parameters tuning.

Sadly, I have huge latency in simulator and controller communication. So, I had to decrease car speed to 10 mp/h.

### P, I, D components effect

P component in PID controller is proportional to difference between current and desired value.

D component in PID controller is proportional to speed of current value change and desired value change (desired speed is often 0).

```
    d_error = (cte - prev_cte)/dt;
```

It is a good idea to calculate `dt` and not include it in coefficient as it will make controller to work same in different conditions. This component is used to minimize oscillation.

I component in PID controller is proportional to integral of difference between current and desired value. This component is used to remove constant or slow drift errors in control mechanism.


### Final hyperparameters choice

To choose final parameters I've used twiddle algorithm. I've manually found value `2.5` for proportional component so that car is able to stay on the track for the whole circle. Than I started twiddle with initial values {2.5, 0., 0.} and finished with values {2.86, 0.58, 0.003}.
