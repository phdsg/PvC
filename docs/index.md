<!-- Version and License Badges -->
![Version](https://img.shields.io/badge/version-0.5.6-green.svg?style=flat-square)
![License](https://img.shields.io/badge/license-BSD3-blue.svg?style=flat-square)
![Language](https://img.shields.io/badge/language-C++-yellow.svg?style=flat-square)


... is a set of [VCVRack](https://www.vcvrack.com) modules created mainly for my own use.
Most are modified and/or slimmified versions of others or just simple tools/toys
i wanted to have and enjoyed learning to program.


## Disclaimer

I'm new to the modular synthesis world, new to programming, new to github.  
**... expect bugs, beginner mistakes, etc.**

[Any and all feedback is welcome.](https://github.com/phdsg/PvC/issues)


## Thanks

First of all: Andrew Belt for making VCVRack and making it open source.  
Strum: his modules and encouragement made me start this whole programming trip.  
All the other module developers for their inspiring modules.



## List of Modules

<img align="right" src="images/AllModules.png">  
  
  _(left to right)_
    
  __Mu\[L\]ty__ - Multiple ([details](multy.md))  
  __VUBar__ - Meter ([details](vubar.md))  
  __vAMPs__ - VCA ([details](vamps.md))  
  __Oomph__ - Distortion ([details](oomph.md))  
  __SumIt__ - Mixer ([details](sumit.md))  
  __ShutIt__ - Switch ([details](shutit.md))  
  __ComPair__ - Comparator ([details](compair.md))  
  [...](plans.md)
    
    
    
    
    
    
    
    
    
## Build Instructions

  To build the modules from source you first have to build rack from sources as described here:  
    https://github.com/VCVRack/Rack#setting-up-your-development-environment  

  Then clone this repository into rack's plugins folder.  
    `$ cd plugins && git clone https://github.com/phdsg/PvC`  
    
  Change directory to PvC and use make to build the plugin.  
    `$ cd PvC && make`  
  
## Contact & Help
I'm almost always online @ #VCVRack (irc.freenode.net). Other module devs are too!  
Also, the [issue tracker](https://github.com/phdsg/PvC/issues) can be used for all questions.

