
# PvC Modules for VCV Rack

I'm not a coder, but VCV Rack got me interested in c++.
So, as a learning experiment i'm making some modules.
Most of them are really very simple or modifications of existing modules.
I'm also new to all this github stuff, so ...


**... expect bugs, beginner mistakes, etc.**

![Any and all feedback is welcome.](https://github.com/phdsg/PvC/issues)



![All Modules](/images/AllModules.png?raw=true "All Modules")

Ports on all modules are color coded.
* Blue: Audio IN
* Red: Audio OUT
* Yellow: Control IN

#### MU[L]TY

1 x 6 Multiple with a mute button on each output. (Based on Fundamental Mutes).


#### VAMPS

Modified version of the Fundamental VCA.
Has 2 Ins/Outs per VCA.
Upper CVIn is exponential, the other one is linear.


#### VUBAR

Very simple VU Meter that uses rack's vumeter widget.
2dB per LED, top one is a clip light.


## Contact

you can find me (and other Rack Module devs) on IRC: freenode #VCVRack


## Build

First, build the latest version of [Rack](https://github.com/VCVRack/Rack), then clone this repository inside the `plugins` folder of Rack and use `make`.

