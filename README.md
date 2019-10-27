# Smart-Juul
A DIY Juul with Wifi and Bluetooth. I extended this project to include a robot that can be controller with an IR controller


# Requirements
- [x] Work
- [x] Party Mode Enabled
- [x] Connect to Google Home (WiFi) using IFTTT servers
- [ ] Connect to iPhone (Bluetooth)


# Robot (JUUL-E)
<p align="center">
 <img src = "/Assets/ClampDemo.gif" height = "475">  
 <img src = "/Assets/TaserDemo.gif" height = "475"> 
</p>

The robot is called JUUL-E, inspired by the rising popularity of vaping devices. It can be controlled with any IR controller to pick up items such as the JUUL. For fun, I've attached a taser to the clamps of the robot.


# Dog Friendly? Yes!
<p align="center">
 <img src = "/Assets/DogDemo.gif" height = "475">  
 <img src = "/Assets/DogDemo2.gif" height = "475"> 
</p>

Here is JUUL-E playing with my dog. 
*Note: The taser functionality is turned off! It will not operate if the robot is moving for everyone's safety.* 


# Background Information
<p align="center">
 <img src = "/Assets/Problem.jpg" height = "475"> 
</p>

I've come across a few youtube videos of people [smoking a JUUL Pod without a Juul](https://youtu.be/usJT8lpQg4s?t=151). While this may seem like a solution, there is a problem - too much current flowing to the Juul Pod will give a burnt taste when inhaling. Here is an example of the pod [overheating](https://www.youtube.com/watch?v=5GlCHSosfs4&feature=youtu.be&t=380). Why does this happen? Using a 12W USB Power adapter and a stripped USB cable wire, we determine the surplus of current is the problem. 

<p align="center">
 <img src = "/Assets/VoltageReading.jpg" height = "475">  <img src = "/Assets/CurrentReading.jpg" height = "475"> 
</p>

There must be a better way to smoke without a Juul, right?

# Q&A

What is the resistance of the Juul coil? **1.8-1.9Ω [±(0.8% Reading)](https://images-na.ssl-images-amazon.com/images/I/B1L2Wl6BZLS.pdf)**

<p align="center">
 <img src = "/Assets/Teardown.jpg" height = "380"> <img src = "/Assets/CoilMeasurement.jpg" height = "380">  <img src = "/Assets/Coil.jpg" height = "380"> 
</p>


What is the output voltage? With full charge, the voltage measured was **2.985V ±(0.5%)**
<p align="center">
 <img src = "/Assets/VoltageOutput.jpg" height = "475"> 
</p>

What is the ouput current? **15.72 μA ±(0.8%)**
<p align="center">
 <img src = "/Assets/OutputCurrent.jpg" height = "475"> 
</p>



