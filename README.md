# ps1avr
Original PS1 controller interface for AVR microcontrollers

<h2>Circuit</h2><br>
<img src="https://raw.githubusercontent.com/ShrinathN/ps1avr/master/img/circ.png">

<br>
You are free to change the pins (so as long as they remain on the same port on the AVR). Change the pin definitions in the "psx_controller.h" file.<br>
I use a 16MHz crystal with my AVR, but if you use any other crystal, be sure to define the speed in the "main.c" file, it is defined as F_CPU<br>
You can use a 5V power supply as well, but ther original PS1 controllers will <b>only</b> work with 3V3<br>
If you decide to use it with some other AVR, remember to make appropriate changes to the Makefile

<h2>How to compile</h2>
You need
<ul>
<li>A Linux environment (Ubuntu, or Debian will do just fine)
<li>avr-libc</li>
<li>avr-gcc</li>
<li>avr-binutils</li>
<li>avrdude (or anything else you use to flash firmware)</li>
<li>make</li>
</ul>
Unless you're using a really obscure distro, you will probably find everything in your distro's repositories<br>
<br>
Now type, "make", to compile the source code, and obtain the .hex file<br>
Use "sudo make flash" to burn