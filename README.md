# Build:
```bash
mkdir build
cd build
cmake ..
make
```  

# Run:
```bash
./frame_gen w h d p r g b f fp
```  
w = width (pixels)  
h = height (pixels)  
d = depth (bits)  
p = pattern  
	0 solid fill  
	1 checker fill  
	2 horizontal ramp  
	3 vertical ramp  
	4 diagonal ramp  
r g b = red, green and blue values to pass to pattern generators  
f = output path of unpacked frame  
fp = output path of packed frame  

Example:
```bash
./frame_gen 1920 1080 12 1 2048 0 2048 1920x1080_purple_checker.bin 1920x1080_purple_checker.r12b
```  
Generates a purple checker frame 1920 x 1080  
Outputs an unpacked frame: 1920x1080_purple_checker.bin  
And a packed frame: 1920x1080_purple_checker.r12b  
