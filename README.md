Lab Assignment 2 : PD Motor Control
===================================

Variables
---------
* T = Output motor signal (torque)
* Pr = Desired motor position or speed
* Pm = Current motor position or speed
* Vr = Desired motor velocity
* Vm = Current motor velocity
* Kp = Proportional gain
* Kd = Derivative gain

Deliverables
------------

### 1. Speed
Implement the controller to maintain speed. The pieces to experiment with are the sampling rate and the range of reference speed. Note that you can only achieve about 5 degrees of accuracy due to the sensor, thus you will need to calculate speed using a sliding window of averaged measurements or slow your sampling rate. Attempt to maintain a very high speed (I think 1.5 rotations/second is about maximum) and a very low speed (how slow can you go??) and some values in between. Start by tuning your gains for the very fast. Try those same gains for the very slow. Record the measured position, measured speed, reference speed, and error as the motor is moving. Report the gains and the sampling rate. Graph the variables and describe and explain your findings. Can you use the same gains for the full range of reference speed, or is there a point at which you have to change gains?


#### Results

![Kp5.0Kd5.0F50S20.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp5.0Kd5.0F50S20.png)

![Kp5.0Kd5.0F50S255.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp5.0Kd5.0F50S255.png)

![Kp0.5Kd0.5F50S120.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.5F50S120.png)

![Kp0.2Kd0.2F50S120.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50S120.png)

![Kp0.1Kd0.1F50S20.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.1Kd0.1F50S20.png)


### 2. Position
Implement the controller to maintain position. Using a good sampling rate, experiment with speed and range of reference position. Use high gains for positional control, so that the motor mostly runs at full speed. Observe the overshoot and see if you can adjust the gain to maintain both a high speed and not overshoot. Then use very low gains so that the motor runs very slowly. Observe for undershoot and see if you can adjust the gains to maintain both low speed and reach the target position. For each, record the measured position, measured speed, reference position, nd error; and record your gains. Graph the variables and describe and explain your findings.


#### Results

![Kp0.5Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.1F20P50.png)

![Kp0.5Kd0.2F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.2F50P50.png)

![Kp0.4Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.1F20P50.png)

![Kp0.4Kd0.1F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.1F50P50.png)

![Kp0.4Kd0.4F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.4F50P50.png)


![Kp0.25Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.25Kd0.1F20P50.png)

![Kp0.2Kd0.2F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50P50.png)

![Kp0.15Kd0.15F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.15Kd0.15F20P50.png)


### 3. Trajectory
Determine optimally tuned values for the PD positional controller (i.e. those that achieve good control while maintaining good speed) and the optimal frequency of the controller based on above experiment. Implement the interpolator and execute the trajectory: rotate the motor forward 360 degrees, hold for .5 seconds (have the system precisely measure this time period), then rotate backwards for 360 degrees, hold for .5 seconds, rotate forwards for 10 degrees. Graph Pm, Pr and T while executing the trajectory. Be sure to graph the entire trajectory. 


#### Results

![Kp0.35Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.35Kd0.25F50T.png)

![Kp0.45Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.45Kd0.25F50T.png)

![Kp0.3Kd0.2F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.3Kd0.2F50T.png)

![Kp0.25Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.25Kd0.25F50T.png)

![Kp0.2Kd0.2F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50T.png)


### 3. Trajectory (part two)
Execute the same trajectory described above, except run your PD controller at 50Hz and then at 5Hz while graphing the same variables. Discuss the results.

#### Results

![Kp0.45Kd0.25F200T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.45Kd0.25F200T.png)


![Kp0.15Kd0.25F20T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.15Kd0.25F20T.png)




