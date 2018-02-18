# CoSuOf <img align="right" src="images/cosuof_100.png">
... is a 2HP Comparator / Substractor / Offsetter

## Details
inspired by the D-A167

### I/O
POS Input  
NEG Input  
  
SUM Output : POS IN - NEG IN + OFFSET (!)  
GATE(/NOT) Outputs : GATE is high when SUM is over 0, low when 0 or below.  

(!) in extreme cases this function can lead to voltages up/down to +/-30V.
so tune the attenuators and offset with a meter.

### Controls

Input Attenuators: Input x [0..1]  
Sum Offset: [-10..10]V  
Gap: Hysteresis Control for the Gate 

## Changes
0.5.8 - initial version  
