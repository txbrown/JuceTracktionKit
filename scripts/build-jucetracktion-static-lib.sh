
#!/bin/bash
set -e

BUILD_DIR="Sources/CJuceTracktion/extra/JuceTracktion/Builds/MacOSX/build"

echo "Building Debug configuration..."
xcodebuild -project Sources/CJuceTracktion/extra/JuceTracktion/Builds/MacOSX/JuceTracktion.xcodeproj \
    -scheme "JuceTracktion - Static Library" \
    -configuration Debug \
    -sdk macosx \
    SYMROOT="$BUILD_DIR"

echo "Building Release configuration..."
xcodebuild -project Sources/CJuceTracktion/extra/JuceTracktion/Builds/MacOSX/JuceTracktion.xcodeproj \
    -scheme "JuceTracktion - Static Library" \
    -configuration Release \
    -sdk macosx \
    SYMROOT="$BUILD_DIR"

echo "Builds completed. Check $BUILD_DIR/Debug and $BUILD_DIR/Release."
