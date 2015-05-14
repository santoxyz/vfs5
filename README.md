# vfs5

/**
*
* This firmware is a software implementation of VOX vfs5 footswitch, 
* compatible with the VT/VT+ series of guitar amplifiers. 
* it's tested on a VT20+.
*
* Have a look to http://www.whitelamp.com/public/vox_vfs5.html for the 
* analog circuit reverse engineered from the original VOX footswitch.
*
* we generate a PWM on arduino's digital pin 5,
* setting the port to the highest possible frequency (~60kHz)
* and apply to it a simple dual pole low-pass filter to convert 
* the PWM signal in a continous analog voltage:
*
*	pin5 >------- 1.2KOhm ------ 1.2KOhm ------> out 
*	                         |              |
*	                        47nF           47nF
*	                        _|_            _|_
*                           ///            ///
*
*
* TODO: use 4 input switches for channel selection
* TODO: use a led (or two) to signal currently selected bank
* WARNING: the voltage provided by the amp drops to 4.25V when arduino is connected... see MAX_OUTV const
*
**/

