# BeanCounter

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

BeanCounter is an SMT parts counter that fits in your pocket. It’s battery powered and uses two IR photointerrupters to count parts about as fast as you can pull them through. It works with any opaque, 8-mm-wide carrier tape up to 2 mm in height, which covers most 0805-or-smaller LEDs and passives, as well as SOT23 transistors. It’s an affordable SMT tape counter without unnecessary bells and whistles.

![BeanCounter on white background](https://github.com/NPoole/BeanCounter/blob/main/docs/beancounter-front.jpg)

## Available for Pre-Order Now on [Crowd Supply](https://www.crowdsupply.com/great-big-factory/beancounter)!

To use BeanCounter, simply turn it on and start pulling tape through. It will immediately begin counting your parts using one of two modes:

* Inventory Mode - Using just one sensor, BeanCounter polls at its fastest rate, allowing you to count long tapes and partial reels very quickly.
* Dispense Mode - With both sensors active, BeanCounter can detect the direction in which you are pulling tape, which allows it to count upward in one direction and downward in the other. This feature is useful in kitting contexts where you may be cutting fixed quantities off the end of a full reel.
Part pitch is configurable in either mode, so you can accurately count any part that physically fits through the counter.

Digging a little deeper, what BeanCounter actually does is count feed holes and divide by the part pitch. Because it cannot differentiate between parts and empty pockets, you will need to ensure that it begins counting after empty tape has been pulled through and stops counting before it reaches the tail. To help with this, we’ve added a "pause" button that you can use to freeze the count while pulling empty tape.

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
