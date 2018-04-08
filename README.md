# PvC Modules for VCVRack

__git-pages website is disabled until i find time to do it properly__

## About
this is my collection of diy modules for vcvrack.
i'm not a programmer, so most of them are simple utilities that i had fun learning to realize.

## Installation

--- update for SDK and RACK_DIR usage ---

### The Easy Way

  via [Plugin Manager](https://vcvrack.com/plugins.html#pvc) (not always the latest version)

### The Nerdy Way

#### Set Up A Build Environment

  In order to build the plugin you need a RACK_DIR that holds the sources/libs/deps of Rack itself.
  You can either use the Rack source repo or the recently added Rack-SDK.

##### Install The Rack-SDK

  --- RACK-SDK howto goes here ---

##### Install The Rack Source Repo

  How to build rack from sources: https://github.com/VCVRack/Rack#setting-up-your-development-environment  
  
  
#### Building The Plugin
  
  Clone this repository into rack's plugins folder.  
    `$ cd plugins && git clone https://github.com/phdsg/PvC`  
    
  Change directory to PvC and use make to build the plugin.  
    `$ cd PvC && make dist` 


#### Installing The Plugin
