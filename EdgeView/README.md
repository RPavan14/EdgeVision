# EdgeVision Pro

A real-time edge detection viewer combining Android native OpenCV processing with TypeScript web visualization.

## Architecture

```
EdgeVision Pro/
├── app/                    # Android application
│   ├── src/main/
│   │   ├── java/          # Java/Kotlin UI and camera logic
│   │   ├── cpp/           # Native C++ OpenCV processing
│   │   └── res/           # Android resources
│   ├── build.gradle
│   └── CMakeLists.txt
├── jni/                   # JNI bridge definitions
├── gl/                    # OpenGL ES 2.0 shaders and utilities
├── web/                   # TypeScript web viewer
├── docs/                  # Documentation and screenshots
└── opencv/                # OpenCV integration scripts
```

## Features

### Android Native App
- **Real-time camera capture** using Camera2 API with TextureView
- **Native OpenCV processing** in C++ via JNI
- **Canny edge detection** with grayscale conversion option
- **OpenGL ES 2.0 rendering** for high-performance texture display
- **Toggle between raw/processed** output
- **FPS counter** and performance metrics
- **Modular architecture** with clean separation of concerns

### TypeScript Web Viewer
- **Sample frame display** with base64 encoded processed images
- **Interactive overlays** showing FPS, resolution, and processing stats
- **Responsive design** with modern UI components
- **Mock WebSocket endpoint** for real-time data simulation

## Prerequisites

### Android Development
- Android Studio Arctic Fox or later
- Android NDK r25c or later
- CMake 3.18+
- OpenCV 4.8+ for Android

### Web Development
- Node.js 18+
- TypeScript 5.3+
- Modern web browser with WebGL support

## Setup Instructions

### 1. OpenCV Installation

```bash
# Download OpenCV Android SDK
wget https://github.com/opencv/opencv/releases/download/4.8.0/opencv-4.8.0-android-sdk.zip
unzip opencv-4.8.0-android-sdk.zip -d opencv/
```

### 2. Android Build Configuration

Update `app/build.gradle` with your OpenCV path:
```gradle
android {
    ...
    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
            version "3.18.1"
        }
    }
}
```

### 3. Web Viewer Setup

```bash
npm install
npm run build-web
npm run serve-web
```

## Building and Running

### Android App
1. Open project in Android Studio
2. Sync Gradle files
3. Connect Android device or start emulator
4. Run the app

### Web Viewer
```bash
npm run dev-web
# Open http://localhost:8080
```

## Performance Targets

- **Android**: ≥15 FPS real-time processing
- **Memory**: <100MB peak usage
- **Latency**: <50ms frame processing
- **Web**: 60 FPS UI animations

## Git Workflow

This project follows atomic commits:
- `feat:` New features
- `fix:` Bug fixes
- `perf:` Performance improvements
- `docs:` Documentation updates
- `refactor:` Code restructuring

## Screenshots

![Android App](docs/screenshots/android-app.png)
![Web Viewer](docs/screenshots/web-viewer.png)
![Edge Detection](docs/screenshots/edge-detection.png)

## License

MIT License - see LICENSE file for details.
