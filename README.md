# Smart-Juul
A DIY Juul with Wifi and Bluetooth 


# Requirements
- [ ] Work
- [ ] Party Mode Enabled
- [ ] Connect to Google Home (WiFi)
- [ ] Connect to iPhone (Bluetooth)

# Background Information
<p align="center">
 <img src = "/Assets/Problem.jpg" height = "475"> 
</p>

I've come across a few youtube videos of people [smoking a JUUL Pod without a Juul](https://youtu.be/usJT8lpQg4s?t=151). While this may seem like a solution, there is a problem - too much current flowing to the Juul Pod will give a burnt taste when inhaling. Here is an example of the pod [overheating](https://www.youtube.com/watch?v=5GlCHSosfs4&feature=youtu.be&t=380). Why does this happen? Using a 12W USB Power adapter and a stripped USB cable wire, we determine the surplus of current is the problem. 

<p align="center">
 <img src = "/Assets/VoltageReading.jpg" height = "475">  <img src = "/Assets/CurrentReading.jpg" height = "475"> 
</p>

There must be a better way to smoke without a Juul, right?

What is the resistance of the Juul coil? **1.8-1.9Ω [±(0.8% Reading)](https://images-na.ssl-images-amazon.com/images/I/B1L2Wl6BZLS.pdf)**

<p align="center">
 <img src = "/Assets/Teardown.jpg" height = "380"> <img src = "/Assets/CoilMeasurement.jpg" height = "380">  <img src = "/Assets/Coil.jpg" height = "380"> 
</p>


What is the output voltage? With full charge, the voltage measured was **2.985V [±(0.5% Reading)](https://images-na.ssl-images-amazon.com/images/I/B1L2Wl6BZLS.pdf)**
<p align="center">
 <img src = "/Assets/VoltageOutput.jpg" height = "475"> 
</p>




