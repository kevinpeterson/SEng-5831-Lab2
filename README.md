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

I started with Kp and Kd being high (5.0 and 5.0), and attempted to rotate the wheel at 255 counts/sec (which is the max speed of the system). There was fast rise time and fairly minimal oscillation. The main observation was that the measured speed did not ever reach the desired speed. After some experimentation, I found that any setting over 240 counts/sec or so would give the same measured velocity, so I assume that I am hitting the max speed of the motor.
![Kp5.0Kd5.0F50S255.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp5.0Kd5.0F50S255.png)


Using the same values for Kp and Kd, I nexed tried to rotate the motor at 120 counts/sec. The motor oscillated out of control and quickly became unstable. Because of this, I tried lowering byt Kp and Kd to 0.5. The rise time was about the same as the above graph, but there was considerable overshoot an oscillation. It appears that Kp and Kd have different effects based on the motor speed.
![Kp0.5Kd0.5F50S120.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.5F50S120.png)


To account for the oscillation of the previous example, I tried lowering both Kp and Kd to 0.2. This seemed to help the oscillation, but the rise time was slower -- which makes sense, as Kp is not increasing the torque as aggressively as before.
![Kp0.2Kd0.2F50S120.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50S120.png)


Next, I tried to operate the motor very slowly. The slowest speed I could achieve was around 10-20 counts/second. I could get it lower (around 8 counts/second was the absolute minimum), but it was very jerky. With both gains very low (both at 0.1), the rise time was slow, but speed seemed fairly stable. The graph indicates a lot of oscillation, but I think a large portion of that is inaccuracies of measuring velocity. When the motor is moving very slowly, it is much harder to get an accurate measurement of the velocity, because there are less counts happening in the measurement time period.
![Kp0.1Kd0.1F50S20.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.1Kd0.1F50S20.png)


For comparison sake, I used the same Kp and Kd values that I used for turning the motor at 255 counts/second to turn it at 20 counts/second. The result was large oscillation, and an uncontrollable system. There appears to be a large difference in how Kp and Kd behave when combined with motor speed.
![Kp5.0Kd5.0F50S20.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp5.0Kd5.0F50S20.png)

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




