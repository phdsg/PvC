# PvC Modules for VCVRack

__git-pages website is disabled until i find time to do it properly__

## Index
- About
  - Disclaimer
  - Latest Changes
  - Versions
- Installation
  - Easy
  - Nerdy
- Manuals
  - this
  - that
- Contact/Feedback/Help

## About
this is my collection of diy modules for vcvrack.

i'm not a programmer, so most of the modules are simple utilities that i had fun learning to realize.

vcvrack is an awesome platform for musical creativity but it's openness also makes it easy (even for beginners like me) to look behind the panels and tweak a few things here and there to just personalize the looks or go even further and change the plugin's behavior.

it all started with wanting a different panel-color on some modules, so i edited some svgs.  
then came the famous "what if this module could also do X"? modded some existing modules.  
watched some demos of eurorack toys: the "i want this in vcv" moment. so some modules are inspired by manuals or demos of hardware.

long story short: vcv has been awesome ride for me so far and i wish i could dedicate more time to it.


### Disclaimer
- this is not production level software...
- created mainly for my own use and for the most part i just assume i'm the only user, but...
- of course other users' feedback is very welcome and will be considered...
 
### Latest Changes
- plugins updated to Rack 0.6 API (0.5.8 -> 0.6.0)

### Versions
- Binary (Plugin Manager): 0.6.0
- Source: release (default branch): 0.6.0
- Source: master (branch): 0.6.0

master branch holds the latest code that's working on my main machine (windows).  
release branch gets updated from master when the code builds on all platforms.  
the plugin manager version will then be built from the release branch.

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

## Manuals

### Bang Da Button
  a button to trigger and switch things
### Heads&Tails
  two A/B switches with a random touch
### Compair
  dual window comparator
### ...

### ... ...

## Contact/Feedback/Help
- Issue Tracker
- IRC
- Discord
- (mastodon)
