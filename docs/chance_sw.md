# Heads & Tails <img align="right" src="images/tails_100.png"> <img align="right" src="images/heads_100.png">
... is a set two 4HP A/B switches. a 2-1 (Heads) and a 1-2 (Tails) variant.
switching can be done directly or on a coin toss chance.

## Details
Based on an idea by Joop van der Linden (@joopvl) to combine a bernoulli gate with an audio path.  
Heads routes either input A or B to its output.  
Tails routes its input to either output A or B.  
Changing routes can either be done manually by clicking the labels or remotely by using the CV trigger inputs.

### Random Route Changes
TOSS changes the route based on the outcome of a coin toss.  
In DIR mode the route is directly chosen by the outcome while in FLP mode the outcome decides whether the current path is changed or not.  
The PROB knob and CV influence the chance of the toss result being heads or tails.

### Direct Route Changes
FLIP - switches routes from one to the other
A - sets route to A
B - sets route to B

### Gate and Trigger Outs
G outs are high when their route is active.
The active T out passes the triggers of any route change event.

## Changes
0.6.0 - ui tweaks, name change  
0.5.8 - initial version  

