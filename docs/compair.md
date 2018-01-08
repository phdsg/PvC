# ComPair <img align="right" src="images/compair_200.png">
... is a 6 rack unit wide Dual Window Comparator.

## Details
**ComPair** checks if an input is within a specified voltage range. 

### Display
Compare lights next to each input indicate whether the signal is inside, below or above the window limits.  
**BLUE** - below  
**RED** - above  
**WHITE** - inside  

### I/O
_per channel (2)_  
1 Input (blue) - the signal that's compared, Channel B normalized to A  
2 Control Inputs (yellow) - one for each of the controls (P)osition and (W)idth, Channel B normalized to A  
2 Outputs (red) - (G)ate and not (!G)ate  

_logic Section_  
4 Outputs (red) - results of boolean comparison of A vs B outputs. (A)nd, (O)r, (X)or and (F)lipFlop triggered by the Xor rises  

### Controls
The compare window is controlled by two parameters.  
  
**POSITION** knob sets the center of the window to a value between -5V and +5V (0V default).  
**WIDTH** knob sets the size of the window to values between 0.01V and 10V (5V default).  
  
CV inputs for each parameter are added to the voltage set by the knobs.  

Compare lights function as toggles inverting their channel's output to the logic section.  

## Changes
0.5.7 - some fixes  
0.5.6 - new compare lights, bi-polar output option  
0.5.5 - inverters for the internal logic inputs  
0.5.4 - added 1st version of the lights, new panel, fixes  
0.5.3 - initial version  

### little backstory
...
