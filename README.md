Lab Assignment 2 : PD Motor Control
===================================


Project Structure
-----------------

**lab2.c/h** - Where the cyclic executive is, and where the various experiment conditions are set.

**logger.c/h** - Logging/debug functionality (printing to the LCD).

**motor.c/h** - Basic motor control functionality.

**motor_encoder.c/h** - An interface used to read motor counts via an encoder.

**pd_controller.c/h** - A Proportional Derivative motor controller.

**command_line.c/h** - A command line user interface using serial commands

**command_parse.c/h** - A command registration component that allows new command line commands to be registered in the system. This component is inspired by the Python argparse library - http://docs.python.org/3.4/library/argparse.html

**serial.c/h** - Centrailized serial communication functionality.

**scheduler.c/h** - A simple non-preemptive task scheduler.

**neural_net.c** - An attempt to apply machine learing to the PD controller. Currently not functional and not included in the build. See [here](#explorations) for more information.

**controller.py** - A Python interface into the system using the serial interface. This was used to conduct the experiments and to plot the graphs.



Variable Definitions
--------------------
* T = Output motor signal (torque)
* Pr = Desired motor position or speed
* Pm = Current motor position or speed
* Vr = Desired motor velocity
* Vm = Current motor velocity
* Kp = Proportional gain
* Kd = Derivative gain


Command Line UI Controls
------------------------

The system uses a serial command line interface for user interaction. The following commands are available:
```
h - Display the help menu.
e - Toggle serial echo back.
S/s - Set the reference speed (counts/sec) - ex. s90
R/r - Set the reference position - ex. r45
L/l - Toggle logging of Pr, Pm, and T
P/p - Set Kp - ex. p0.2
D/d - Set Kd - ex. d0.15
V/v - View the current values Kd, Kp, Vm, Pr, Pm, and T
F/f - Set the PD Controller frequency (in ms period) - ex. f200
I/i - Execution Interpolator ex. is90,w10,s0
* NOTE: The interpolator can accept a comma-delimited series of any 
  of the above commands, with the addition of a "wait" (w) command. 
  This pauses the interpoloator for a time (x 100ms).
```

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

The highest gain that was stable was Kp = 0.5 and Kd = 0.1. Note that the rise time was fast, but there was alot of oscillation. I hypothesize that this was because of the Kd value being low.

![Kp0.5Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.1F20P50.png)


To check the above hypothesis, I set Kd = 0.4. This decreased oscillation to be sure, but also had a negative effect on rise time -- as you can see in the graph, motor velocity stays low. Perhaps there is something in the middle that is optimal...

![Kp0.4Kd0.4F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.4F50P50.png)


Next, I explored some different controller frequencies -- 20 Khz and 50 Khz. There was some differences between results, but overall, they were comparable. Interestingly, going much outside this range led to the system becoming quicky unstable

The following are various settings of both 20 Khz and 50 Khz.

![Kp0.5Kd0.2F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.5Kd0.2F50P50.png)

![Kp0.4Kd0.1F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.1F50P50.png)


![Kp0.2Kd0.2F50P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50P50.png)

![Kp0.4Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.4Kd0.1F20P50.png)


![Kp0.25Kd0.1F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.25Kd0.1F20P50.png)


Finally, by setting both constants very low, I noticed that the motor never did reach the desired position.

![Kp0.15Kd0.15F20P50.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.15Kd0.15F20P50.png)


For some of these graphs, I noticed that my measurements reported motor velocity even while the measure motor position remained flat. I attempted to find the root cause of this, but I could not pinpoint a cause. It may have been very slight oscillations, or noise in the wheel encoder.

### 3. Trajectory
Determine optimally tuned values for the PD positional controller (i.e. those that achieve good control while maintaining good speed) and the optimal frequency of the controller based on above experiment. Implement the interpolator and execute the trajectory: rotate the motor forward 360 degrees, hold for .5 seconds (have the system precisely measure this time period), then rotate backwards for 360 degrees, hold for .5 seconds, rotate forwards for 10 degrees. Graph Pm, Pr and T while executing the trajectory. Be sure to graph the entire trajectory. 


#### Results

For the trajectory, the original intent was to hold position for .5 seconds after the desired position was reached. This turned out to be a problem as I experimented, as for some gain settings, the desired position was never reached (e.g., the motor would stop just below it). Because of this, I changed the 'hold' time to a finite 2.5 seconds -- so the trajectory was input as "move x, wait 2.5s, move x, wait 2.5s" etc.

First, I started with low values for Kp and Kd (0.2 for both). The motor was very stable and smooth, but slow. I never did reach the desired position in the allotted time. This would be ok if very smooth movement was needed, of if overshoot was bad for some reason... but I think for our purposes these values are not optimal.

![Kp0.2Kd0.2F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.2Kd0.2F50T.png)


To try to add some speed to the motor, I moved Kp up to 0.3. This helped rise time, and torque is shown to be much higher -- so to motor is receiving stronger signals, and the faster movement reflects that.

![Kp0.3Kd0.2F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.3Kd0.2F50T.png)

For comparision sake, I decreased Kp slightly and increased Kd slightly. I expected rise time to decrease slightly (which it did), and increasing Kd allowed the final trajectory position to be fully reached.

![Kp0.25Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.25Kd0.25F50T.png)


Kd seemed to be ok for the previous experiment (0.25), so I left that as-is. I still wanted to try to get some more motor speed in the movements, so I set Kp = 0.35 and Kd = 0.25. This was ok, but the motor moved slower than probably was necessary. To help speed things up, I will probably need to increase Kp.

![Kp0.35Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.35Kd0.25F50T.png)


Based on the last experiment, I increased Kp to 0.45 (while leaving Kd the same). After trying many different combinations, this seemed to give the best mix of speed and stability.

![Kp0.45Kd0.25F50T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.45Kd0.25F50T.png)

### 4. Trajectory (part two)
Execute the same trajectory described above, except run your PD controller at 50Hz and then at 5Hz while graphing the same variables. Discuss the results.

#### Results

First, I set the controller to 5Hz. The system quickly becaome unstable. Notice that in the graph below, the Measured Position at times has a saw-tooth pattern. This is because the position is moving from 127 (which is the max) to 0. The measured position resets to 0 at 128, which is why that pattern is seen. Basically, be means that the motor was continuing to spin instead of stopping at the desired position. I assume this is because the the controller is operating slowing and it has to make much coarser-grained movements. The velocity messurement would also be a problem, as in the time of 5Hz, the velocity is not necessarily constant -- it may be increasing or decreasing during that time. This is problematic because the controller will only act on the last know velocity. The alternative is to average the velocity over the time span, but this is problematic also, as this will make velocity appear constant for each interval between controler cycles, with a moment of instantaneous velocity change as the new average is calculated.

![Kp0.45Kd0.25F200T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.45Kd0.25F200T.png)

Conversely, speeding up controller frequency (to 50Hz) caused the system to become unstable due to oscillation. I assume that this is due to the fact that we cannot measure velocity precisely enough at that speed to accurately inform controller decisions. If velocity could be measured with more precision, I think a higher controller frequency would be helpful -- but as shown, currently it degrades system performance.

![Kp0.15Kd0.25F20T.png](https://raw.githubusercontent.com/kevinpeterson/SEng-5831-Lab2/master/output/Kp0.15Kd0.25F20T.png)


Discussion
----------
Adjusting Kp for the most part had the expected effect -- the motor responded faster, whether that be moving to a new set position, or a new speed. I found that the range I could set it to was smaller than I expected, as moving it too much caused the system to become unstable.

Kd did have some positve effects on settling time, but it was hard to predict what effect it would have. When tuning this, it was important to consider what Kd was, what the desired speed was, or how far the motor was moving (if moving to set positions). Because of this it was hard to tune this constant correctly.

System instability was a problem -- as moving the values of the constants too much would cause so much oscillation that the system would reset itself. I looked into this, and it seems that I was hitting a "brownout" reset, or too much current being drawn through the motor while it is trying to keep up. I wonder how systems deal with this, as it was quite hard to tune the controller to have good response in all possible situations, and I'm sure that most systems can't afford to become so unstable that they reset. I suppose there are checks and balances to this... it would be interesting to see how that is done.

I struggled the most with measuring velocity. I tried averaging several previous results, but this led to sluggish behavior. I assume that it was because the more averaging is done, the more old, outdated data is included in decision. I tried a sliding window, where I populated a rotating ring buffer, and always read head and tail of the buffer queue -- but this wasn't much better, and it was difficult to do this correctly as the controller changed frequency. I tried measuing system time between encoder interrupts. I could measure velocity very accurately then, but that had challenges as well. I implemented this as when a encoder interrupt happened, I would store the system clock time (in microseconds), an then when the next interrupt happened I would subtract the stored time with the current time. I would then know that 1 count was traversed in N microseconds. The problem was that often when the motor was stopping, there would be an encoder interrupt, and then the motor would stop, and I would never have the next interrupt (and time) to compare it to. I'm sure there would be a way to implement this, but it wasn't straightforward. Plus, there were lots of floating point calculations and divisions to normalize the velocity to counts/second, which seemed problematic. In the end, I went with a simple polling strategy -- at a constant rate I compared motor position with previous position. This was simple, and worked (for the most part). Overall, velocity was a challenge, and it showed in some of the instabillity that I saw.


<a name="explorations"></a>A little off topic...
------------------------------------------------
I thought it would be interesting for the system to 'learn' what the optimal values of Kp and Kd were, instead of trying to guess. My idea was to incorporate a neural network into the system, where optial Kp and Kd values could be found by trying values and feeding them back into the system, learning as time went on.

In short, I never quite got it to work. It did seem promising, however. I think this is an interesting application of some sort of learning algorithm. There is some code in ```neural_net.c``` -- but don't expect it to work correctly. I used http://galaxy.agh.edu.pl/~vlsi/AI/backp_t_en/backprop.html as a reference, along with quite a few papers on the topic. It is pretty interesting stuff... anyway, as I said, a little off topic.
