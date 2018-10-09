# NoiseGate effect
This is a noise gate effect demo. It contains the basic noise gate algorithm which references from the paper:
[*Digital Dynamic Range Compressor Design——A Tutorial and Analysis*](http://eecs.qmul.ac.uk/~josh/documents/2012/GiannoulisMassbergReiss-dynamicrangecompression-JAES2012.pdf). By the way, audio file read/write methods are also involved.

## Usage
**NoiseGate User manual**  [中文版](https://github.com/jagger2048/NoiseGate/blob/master/NoiseGate%20User%20manual.md)

## To Do lists
- Advanced Noise Gate with Hysteresis function,based on the Dafx.
- Time-frequency based noise suppression algorithm,different from  the Time-domain level detectors based noise gate. [Done]
  - The famous Open Source noise suppression algorithm is Speex ,Opus and WebRtc‘s  Noise suppression in AGC model, I have captured the function from WebRtc Project to use alone and learning , please  find  [WebRtc noise suppression demo](https://github.com/jagger2048/WebRtc_noise_suppression).  In fact, this algorithm a bit complicated, and generally  it can be used to suppress the noise in voice system.

