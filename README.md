# EdgeVision
Real-time Android edge-detection app using Camera2, OpenCV C++ (NDK), JNI, and OpenGL ES 2.0. Captures camera frames, processes them natively with Canny/Grayscale, and renders results as GL textures. Includes modular native pipeline and a TypeScript web viewer for displaying sample processed frames with FPS and resolution overlays.

📝 Overview

This project demonstrates a real-time computer vision pipeline running natively on Android using OpenCV C++ for image processing and OpenGL ES 2.0 for efficient rendering. Camera frames are captured using the Camera2 API, passed to native code using JNI, processed using Canny edge detection, then rendered on screen as a GPU texture.

A small TypeScript web viewer is included to preview sample processed frames and display FPS + resolution overlays.

🚀 Features
Real-time camera feed using Camera2 + SurfaceTexture
Frame delivery to native via JNI bridge
Native image processing using OpenCV C++
Grayscale
Canny edge detection
GPU rendering using OpenGL ES 2.0
10–15+ FPS performance target on mid-range devices
Modular architecture:
/app – Android app
/jni – C++ native code
/gl – OpenGL renderer
/web – TypeScript viewer
Architecture
A simplified view of the processing pipeline:
Camera2 (YUV_420_888)
│
▼
Java/Kotlin: Convert to NV21 & pass to JNI
│
▼
C++ OpenCV (NDK):
NV21 → RGB
RGB → Grayscale
Grayscale → Canny Edges
│
▼
OpenGL ES 2.0:
Upload RGBA texture
Render to screen

📁 Repository Structure
/ (root)
├─ app/ # Android UI + Camera2 + Java/Kotlin
├─ jni/ # OpenCV C++ + JNI bridge
├─ gl/ # OpenGL ES 2.0 renderer
├─ web/ # TypeScript minimal viewer
├─ docs/ # screenshots & design notes
└─ README.md # this file

🖼 Screenshots

<img width="452" height="773" alt="frame0" src="https://github.com/user-attachments/assets/2011c3c7-4364-4bb3-885b-629bb8f53282" />
<img width="407" height="777" alt="frame 2" src="https://github.com/user-attachments/assets/51f8cc0e-cfd5-4e44-9860-a09ee63bfc7b" />
<img width="372" height="787" alt="frame 3" src="https://github.com/user-attachments/assets/265ac097-870d-435a-a450-e731ff6d7084" />

📱 Android Build Instructions
Requirements:
Android Studio Flamingo or newer
Android SDK 24+ (minSdk)
Android NDK (r25+ recommended)
CMake 3.18+
OpenCV Android SDK (4.x)

📦 Dependencies:
Android
OpenCV 4.x (C++ only)
Android NDK
CMake
GLES 2.0
Camera2 API
