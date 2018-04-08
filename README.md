# PvC Modules for VCVRack

__git-pages website is disabled until i find time to do it properly__

## About
this is my collection of diy modules for vcvrack.
i'm not a programmer, so most of them are simple utilities that i had fun learning to realize.

## Installation

--- update for SDK and RACK_DIR usage ---

### the easy way
(not always the latest version)
via the Plugin Manager: [here](https://vcvrack.com/plugins.html#pvc)

### the nerdy way

#### set up a build environment

  In order to build the plugin you need a RACK_DIR that holds the sources/libs/deps of Rack itself.
  You can either use the Rack source repo or the recently added Rack-SDK.

##### using a rack source repo

  How to build rack from sources: https://github.com/VCVRack/Rack#setting-up-your-development-environment  
  
##### using the Rack-SDK

  --- RACK-SDK howto goes here ---
  
#### building the plugin
  
  Clone this repository into rack's plugins folder.  
    `$ cd plugins && git clone https://github.com/phdsg/PvC`  
    
  Change directory to PvC and use make to build the plugin.  
    `$ cd PvC && make` 

