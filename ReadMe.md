# What is this for project?
This project is my collection of the results of control engineering. From math book to real hardware with advanced algorithms. 
I have been study control theory by my self for a very long time. Most theory all the way. I have been reading lots of papers about
system identification and learn which algorhtm or method works best for different problems. I have been reading lots of books 
about adaptive, optimal, predictive and iterative control, and also I have been doing lots of Java applications for GUI 
and C applications for embedded hardware. 

Why not combine all these things and build a controller for mehanical, fluids, temperature and electrical systems, I thought. 
Well, that's what I have been starting with. In this project I will upload pictures, software and theory about each project I have
made. The reason for that is that I'm a pratical man and I don't like theory on theory. The problem with current control theorist
is they only focusing on control theory and not control engineering. There is a reason why PID-controller still dominates. It's not
because they are the best controllers and do the best job. It's because they are easy to use. Companies don't want to pay employees
to tune in expensive controllers if a propotional controller with small integral action will do the job, even if an advanced controller
is much better to use. 

So I will upload projects where I apply theory to the real world. I will use the tools I have by my self built. 


  * [JUSBPlotter](https://github.com/DanielMartensson/JUSBPlotter "JUSBPlotter") - Logger software for measurements
  * [Mataveid](https://github.com/DanielMartensson/Mataveid "Mataveid") - System identification algorithms
  * [Matavecontrol](https://github.com/DanielMartensson/Matavecontrol "Matavecontrol") - Control theory algorithms for simulation and tuning
  * [EmbeddedLapack](https://github.com/DanielMartensson/EmbeddedLapack "EmbeddedLapack") - C library for matrix algebra


I will upload the following projects:
```
  * Linear Quadratic Gaussian Integral Controller
  * Model Reference Adaptive Control
  * Linear Quadratic Gaussian Self Tuning Controller
  * Adaptive Generalized Predictive Control
  * Iterative Learning Control
  * Model Predictive Control
```

# Linear Quadratic Gaussian Integral Controller
This is a fan-controller that rotates the fan by chaning a PWM-frequency from the STM32 Nucleo board. The measurement is the MCP9700A sensor with TO-92 package. If the sensor become warmer, then the 16x2 LCD will display the temperature and send it to the controller. The controller will compute optimal input signals for the fan to rotate. The meaning with this controller is that the sensor should feel the air from the fan. Behind the fan, it should be like a element or something warm. We tune in the reference set point with the potentiometer e.g 25 degrees C. Then the fan will rotates and blow warm air onto the temperature sensor so it will hold 25 degrees C. If the sensor becomes too warm, the fan will rotates slower, if the sensor become too cold, the fan will rotates faster. This controller have a integral action and the system runs on 12 volt.
![a](https://raw.githubusercontent.com/DanielMartensson/Practical-Control-Engineering/master/Linear%20Quadratic%20Gaussian%20Integral%20Control/Picture%20of%20the%20system.jpg)
