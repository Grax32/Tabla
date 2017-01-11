# Application

## Major Bugs
- Projector window sometimes comes up blank.
	- Seems like full-screening that second window sometimes results in two zombie windows for the projector (a fullscreen one, and a second windowed one). Probably a libcinder issue, or a misuse of libcinder; perhaps a simple change of flags on that window will do the trick? 
	- Known workaround: disable <HasConfigWindow> in app.xml.
- When table is blank, OTSU algorithm freaks out.
	- Known workarounds:
		- Set thresholding constant in <Vision> to something between 0..255 (-1 or undefined means OTSU).
		- Put blank paper on the table
	- Notes on proper solution: If we did thresholding before clipping to the table we'd probably get better results. Or analyze full histogram and manually set the threshold. (Avoid doing 2x clipping transforms of the full image and also the thresholded image--that would be lame.)

# Game Worlds

## General
- No way to define and save orientation (which way is up?) for various modes. (Pinball, Music, Animation, ...).
- mDrawMouseDebugInfo not being respected in BallWorld -- drawMouseDebugInfo still getting called for some reason.

## BallWorld
- Detect, flag, and possibly stabilize or fix stuck balls (in spaces that are too small). Debounce ball velocities and collisions in ball world (track old vels x3 could do the trick.)

## Music
- Robit serial connection isn't robust; sometimes need to restart computer to get it to see the arduino.
- Character wrangling can be hard. Proper solution probably involves tokens for instruments placed next to scores.

## Pinball
- Ball could sometimes tunnel through tip of flipper. Flipper rotation done in BallWorld (for finer integration and less tunneling). Proper fix (and architecture) would be to allow a parallel index of rotational transforms for contours that BallWorld does itself (and can do finer integrations of). 
- Small spurious awkward spaces trap the ball (eg groups of white go stones, your hands, etc...). Solution: reject these spaces, perhaps comparing to perimeter/area of a circle that comfortably fits a ball (or more).
- Flippers penetrate walls. Fix: dynamically size them! Only respond to walls, not parts--or things could get crazy.
- Pinball "palm" rejection. People sometimes rest against table. Detect these awkward edge spaces and reject. Probably need a mask array for contours that defines who is a wall; could have another for who is a part. Or unify these into one list, with types (ignored, wall, part)--i like.