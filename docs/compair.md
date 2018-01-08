# ComPair <img src="images/Compair.png">
... is a Dual Window Comparator (inspired by the joranalogue compare2).

## What does it do?
**ComPair** checks if an input is within a specified voltage range. 

## Controls

The compare window is controlled by two parameters.
* **POSITION** knob sets the center of the window to a value between -5V and +5V (0V default).
* **WIDTH** knob sets the size of the window to values between 0.01V and 10V (5V default).

CV inputs for each parameter are added to the voltage set by the knobs.

Inputs on channel B are normalized to their counterpart on channel A.

Compare lights beside each input indicate whether the signal is inside, below or above the window limits.
* **BLUE** - below
* **RED** - above
* **WHITE** - inside

The lights also function as toggles inverting their channel's output to the logic section.

## Outputs

### Channels
* **G** - GATE goes high (+5V) if the input is within the window
* **!G** - NOT goes high (+5V) if the input is outside the window

### Logic Section
* **A** - AND goes high (+5V) if both inputs are in their windows
* **O** - OR  goes high (+5V) if either input is within its window
* **X** - XOR goes high (+5V) if one input is in its window and the other one is not
* **F** - FLIP FLOP changes state between 0V and +5V whenever XOR goes high

## So, what is it good for!?
*... i'm currently working on some example patches. the ones i already uploaded are still a work in progress and might change. in their final version each patch will have some note modules that explain a little bit and give some hints.
feel free to submit patch-ideas to the issue tracker.*
