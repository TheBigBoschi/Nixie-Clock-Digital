# Nixie-Clock-Digital
this is my attempt at creating a nixie clock, as every electronic hobbyst should do at least once. timekeeping its by itself a very intresting subject, pair it with 60's vibes and hundreds of volts and you get a intresting piece to spark conversations (or a nice gift in my case).

here i have posted the schematics and the firmare of the clock. i wont release the pcb because there are some errors and at the time of the writings the project is finished, and i dont want to go back in the rabbit hole to fix and polish evrything again. 

at the time of this writing Bob (the clock) has been ticking for 3 months without any problem, so im pretty happy with it. 

BEFORE GOING ANY FURTHER: this device (as already mentioned) employ quite some high voltages, whatever you do its at your own risk, i dont take any responsability for your actions.

im going to comment each page of the pdf in order to make evrything more clear.

#1: this is the driving section. it employs 3 74HC595 (shift register) in order to drive 2 nixies, there are 2 of this module to drive all four nixie, one module for the hours, one for the minutes. the shift registers are daisy chained in order to use fewer pin of the micro controller, specifically just 2 to drive all 40 chatodes. the label "DOT1" and "DOT2" are used to drive the glowing lamp that represent the colons between the minutes and the hours.

#2: this is the HV PSU. it uses a older ic, but is enough for this application. the coil is driven up to a certain current, wich is helpful in this case because allow us to make evrything work, with a normal (more recent) ic this wouldnt be possible (or not as simple). the efficiency is acceptable, i didnt measure it but during normal use at full brightness the whole sistem draws around 150mA at 12V, more than adequate. i want to point out a detail about POT1, sometimes potentiometers fails, and the wiper wont make contact with the internal resistor, which is really bad news, especially if you just connect the wiper directly to your feedback pin, because the ic feel that there is no voltage so it tries to rise its voltage, giving out higher and higher tension till something  starts smoking. the way i connected the potentiometer, if this appen (or if the whole pot open) the voltage lower, posing no risk for the circuit.
