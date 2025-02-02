#!/bin/sh

set -e  # Exit on error

ROOT_PATH=$(cd "$(dirname "$0")/.."; pwd -P)
pushd $ROOT_PATH > /dev/null

BUILD_DIR="build"
OUTPUT_PATH="${BUILD_DIR}/xcframework"

echo "Cleaning build directory..."
rm -rf "${BUILD_DIR}"
mkdir -p "${OUTPUT_PATH}"

build_configuration() {
    local PLATFORM=$1
    local IS_IOS=$2
    local CONFIG=$3
    local BUILD_PATH="${BUILD_DIR}/${PLATFORM}-${CONFIG}"
    
    echo "Building ${PLATFORM} ${CONFIG} in ${BUILD_PATH}..."
    
    # CMake configuration
    local CMAKE_EXTRA_ARGS=""
    if [ "$IS_IOS" = "true" ]; then
        CMAKE_EXTRA_ARGS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=17.0 -DCMAKE_OSX_ARCHITECTURES=arm64"
    else
        CMAKE_EXTRA_ARGS="-DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 -DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
    fi

    # Convert config to proper case for CMake
    local CMAKE_CONFIG="Debug"
    if [ "${CONFIG}" = "release" ]; then
        CMAKE_CONFIG="Release"
    fi
    
    echo "Configuring CMake..."
    cmake -B "${BUILD_PATH}" -G Xcode \
        ${CMAKE_EXTRA_ARGS} \
        -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO \
        -DCMAKE_BUILD_TYPE="${CMAKE_CONFIG}" \
        -DCMAKE_CONFIGURATION_TYPES="${CMAKE_CONFIG}"
    
    echo "Building with xcodebuild..."
    xcodebuild build \
        -project "${BUILD_PATH}/tracktion_static.xcodeproj" \
        -scheme tracktion_static \
        -configuration "${CMAKE_CONFIG}" \
        -destination "generic/platform=${PLATFORM}" \
        BUILD_LIBRARY_FOR_DISTRIBUTION=YES \
        SKIP_INSTALL=NO
}

echo "Building all configurations..."

# Build all configurations
build_configuration "iOS" true "debug"
build_configuration "iOS" true "release"
build_configuration "macOS" false "debug"
build_configuration "macOS" false "release"

echo "Creating XCFramework..."

# Remove any existing XCFramework
rm -rf "${OUTPUT_PATH}/Tracktion-debug.xcframework"
rm -rf "${OUTPUT_PATH}/Tracktion-release.xcframework"

# Create Debug XCFramework
xcodebuild -create-xcframework \
    -library "${BUILD_DIR}/iOS-debug/lib/Debug/libtracktion_static.a" \
    -library "${BUILD_DIR}/macOS-debug/lib/Debug/libtracktion_static.a" \
    -output "${OUTPUT_PATH}/Tracktion-debug.xcframework"

# Create Release XCFramework
xcodebuild -create-xcframework \
    -library "${BUILD_DIR}/iOS-release/lib/Release/libtracktion_static.a" \
    -library "${BUILD_DIR}/macOS-release/lib/Release/libtracktion_static.a" \
    -output "${OUTPUT_PATH}/Tracktion-release.xcframework"

echo "Creating zip archive..."
pushd "${OUTPUT_PATH}" > /dev/null
zip -r Tracktion-debug.xcframework.zip Tracktion-debug.xcframework
zip -r Tracktion-release.xcframework.zip Tracktion-release.xcframework
popd > /dev/null

echo "Computing checksum..."
DEBUG_CHECKSUM=$(swift package compute-checksum "${OUTPUT_PATH}/Tracktion-debug.xcframework.zip")
RELEASE_CHECKSUM=$(swift package compute-checksum "${OUTPUT_PATH}/Tracktion-release.xcframework.zip")
echo "Debug Checksum: ${DEBUG_CHECKSUM}"
echo "Release Checksum: ${RELEASE_CHECKSUM}"

# Save checksums to file
cat > "${ROOT_PATH}/checksums.txt" << EOF
DEBUG_CHECKSUM=${DEBUG_CHECKSUM}
RELEASE_CHECKSUM=${RELEASE_CHECKSUM}
EOF
echo "Checksums saved to checksums.txt"

echo "Verifying final XCFramework structure:"
ls -la "${OUTPUT_PATH}/Tracktion-debug.xcframework"
ls -la "${OUTPUT_PATH}/Tracktion-release.xcframework"

popd > /dev/null

echo "Successfully built Tracktion.xcframework"