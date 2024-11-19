// swift-tools-version: 5.9
import PackageDescription

// Shared linker settings
let linkerSettings: [LinkerSetting] = [
    .linkedFramework("Accelerate"),
    .linkedFramework("AudioToolbox"),
    .linkedFramework("Cocoa"),
    .linkedFramework("CoreAudio"),
    .linkedFramework("CoreAudioKit"),
    .linkedFramework("CoreMIDI"),
    .linkedFramework("Foundation"),
    .linkedFramework("IOKit"),
    .linkedFramework("Metal"),
    .linkedFramework("MetalKit"),
    .linkedFramework("QuartzCore"),
    .linkedFramework("Security"),
    .linkedFramework("WebKit"),

    // Dynamically link the correct version of the library based on the configuration
    .unsafeFlags(
        [
            "-L./Sources/CJuceTracktion/extra/JuceTracktion/Builds/MacOSX/build/Debug",
            "-lJuceTracktion"
        ],
        .when(configuration: .debug)
    ),
    .unsafeFlags(
        [
            "-L./Sources/CJuceTracktion/extra/JuceTracktion/Builds/MacOSX/build/Release",
            "-lJuceTracktion"
        ],
        .when(configuration: .release)
    ),
]

// Shared CXX settings
let commonCxxSettings: [CXXSetting] = [
    .headerSearchPath("CJuceTracktion/extra/JuceTracktion/JuceLibraryCode"),
    .headerSearchPath("../../tracktion_engine/modules"),
    .headerSearchPath("../../tracktion_engine/modules/juce/modules"),
    .headerSearchPath("CJuceTracktion"),
    .define("OS_OBJECT_USE_OBJC", to: "0"),
    .define("OS_SIGNPOST_ID_INVALID", to: "0"),
    .define("OS_SIGNPOST_ID_NULL", to: "0"),
    .define("OS_LOG_CATEGORY_POINTS_OF_INTEREST", to: "0"),
    .define("JUCE_STANDALONE_APPLICATION", to: "0"),
    .define("JUCE_USE_DARK_MODE", to: "0"),
    .define("JUCE_DISPLAY_SPLASH_SCREEN", to: "0"),
    .define("JUCE_REPORT_APP_USAGE", to: "0"),
    .unsafeFlags([
        "-x", "objective-c++",
        "-std=c++20",
        "-fno-objc-arc",
        "-stdlib=libc++",
        "-D_LIBCPP_DISABLE_DEPRECATION_WARNINGS",
        "-Wno-deprecated-declarations",
    ]),
    .define("DEBUG", .when(configuration: .debug)),  
    .define("NDEBUG", .when(configuration: .release)), 
]

let runnerCxxSettings: [CXXSetting] = commonCxxSettings + [
]

// Shared Swift settings
let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx)
]

// Package definition
let package = Package(
    name: "JuceTracktionKit",
    platforms: [
        .macOS(.v12),
        .iOS(.v17)
    ],
    products: [
        .executable(name: "JuceTracktionKitRunner", targets: ["JuceTracktionKitRunner"]),
        .library(name: "JuceTracktionKit", targets: ["JuceTracktionKit"]),
    ],
    targets: [
        .executableTarget(
            name: "JuceTracktionKitRunner",
            dependencies: ["JuceTracktionKit"],
            cxxSettings: runnerCxxSettings,
            swiftSettings: swiftSettings
        ),
        .target(
            name: "CJuceTracktion",
            dependencies: [],
            exclude: [
                "extra/JuceTracktion/JuceLibraryCode/include_juce_graphics_Sheenbidi.c",
                "extra/JuceTracktion/JuceLibraryCode/include_juce_graphics.cpp",
                "extra/JuceTracktion/JuceLibraryCode",
            ],
            publicHeadersPath: "include",
            cxxSettings: commonCxxSettings,
            linkerSettings: linkerSettings
        ),
        .target(
            name: "JuceTracktionKit",
            dependencies: ["CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .testTarget(
            name: "JuceTracktionKitTests",
            dependencies: ["JuceTracktionKit", "CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        )
    ],
    cxxLanguageStandard: .cxx20
)
