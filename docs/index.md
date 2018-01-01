<!-- Version and License Badges -->
![Version](https://img.shields.io/badge/version-0.5.6-green.svg?style=flat-square)
![License](https://img.shields.io/badge/license-BSD3-blue.svg?style=flat-square)
![Language](https://img.shields.io/badge/language-C++-yellow.svg?style=flat-square)


... is a set of [VCVRack](https://www.vcvrack.com) modules created mainly for my own use.
Most are modified and/or slimmified versions of others or just simple tools/toys
i wanted to have and enjoyed learning to program.

<img align="right" src="images/AllModules.png">

## Disclaimer

I'm new to the modular synthesis world, new to programming, new to github.  
**... expect bugs, beginner mistakes, etc.**

[Any and all feedback is welcome.](https://github.com/phdsg/PvC/issues)


## Thanks

First of all: Andrew Belt for making VCVRack and making it open source.  
Strum: his modules and encouragement made me start this whole programming trip.  
All the other module developers for their inspiring modules.


## List of Modules
  [ComPair](compair.md) - dual window comparator (6U)  
  [Mu\[L\]ty](multy.md) - multiple with per-channel mutes (2U)  
  [Oomph](oomph.md) - primitive waveshaping distortion (2U)  
  [ShutIt](shutit.md) - eight channel triggerable mutes (4U)  
  [SumIt](sumit.md) - twelve into one mixer (2U)  
  [vAMPs](vamps.md) - dual stereo voltage controlled amplifier (2U)  
  [VUBar](vubar.md) - twelve light vu-meter (2U)  
  [...](plans.md)


## Build Instructions

  To build the modules from source you first have to build rack from sources as described here:  
    (link to tutorial)  

  Then clone this repository into rack's plugins folder.  
    `$ cd plugins && git clone https://github.com/phdsg/PvC`  
    
  Change directory to PvC and use make to build the plugin.  
    `$ cd PvC && make`  
  
## Contact & Help
I'm almost always online @ #VCVRack (irc.freenode.net). Other module devs are too!  
Also, the [issue tracker](https://github.com/phdsg/PvC/issues) can be used for all questions.


## Support PvC
"Buy" a copy of the modules from my [gumroad](https://gumroad.com/l/kXPIO) account for 0+ bucks ;)
