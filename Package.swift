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

    .unsafeFlags(
        [
            "-L./build/lib/Debug",
            "-ltracktion_static_d",
        ],
        .when(configuration: .debug)
    ),
    .unsafeFlags(
        [
            "-L./build/lib/Release",
            "-ltracktion_static",
        ],
        .when(configuration: .release)
    ),
]

// Shared CXX settings
let commonCxxSettings: [CXXSetting] = [
    // .headerSearchPath("Sources/CJuceTracktion/extra/JuceTracktion/JuceLibraryCode"),
    .headerSearchPath("../../tracktion_engine/modules"),
    .headerSearchPath("../../tracktion_engine/modules/juce/modules"),
    .headerSearchPath("Sources/CJuceTracktion"),
    .headerSearchPath("../../tracktion_engine/modules/tracktion_engine/model/clips/"),
    .define("JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED", to: "1"),
    .define("JUCE_STANDALONE_APPLICATION", to: "0"),
    .define("JUCE_USE_DARK_MODE", to: "0"),
    .define("JUCE_DISPLAY_SPLASH_SCREEN", to: "0"),
    .define("JUCE_REPORT_APP_USAGE", to: "0"),
    .define("JUCE_DONT_DECLARE_PROJECTINFO", to: "1"),
    .define("TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH", to: "1"),
    .define("JUCE_CHECK_MEMORY_LEAKS", to: "0"),
    .define("OS_OBJECT_USE_OBJC", to: "0"),
    .define("OS_SIGNPOST_ID_INVALID", to: "0"),
    .define("OS_SIGNPOST_ID_NULL", to: "0"),
    .define("OS_LOG_CATEGORY_POINTS_OF_INTEREST", to: "0"),
    .define("JUCE_HEADLESS_PLUGIN_CLIENT", to: "1"),

    .unsafeFlags([
        "-x", "objective-c++",
        "-std=c++20",
        "-fno-objc-arc",
        "-stdlib=libc++",
        "-D_LIBCPP_DISABLE_DEPRECATION_WARNINGS",
        "-Wno-deprecated-declarations",
        "-fvisibility=hidden", // Hide symbols by default
        "-fvisibility-inlines-hidden", // Hide inline symbols
    ]),
    .define("DEBUG", .when(configuration: .debug)),
    .define("NDEBUG", .when(configuration: .release)),
]

// Shared Swift settings
let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx),
]

let package = Package(
    name: "JuceTracktionKit",
    platforms: [
        .macOS(.v14),
        .iOS(.v17),
    ],
    products: [
        .library(name: "JuceTracktionKit", targets: ["JuceTracktionKit"]),
    ],
    dependencies: [
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.3.0"),
    ],
    targets: [
        .executableTarget(
            name: "JuceTracktionDemosRunner",
            dependencies: [
                "JuceTracktionDemos",
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
            ],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .executableTarget(
            name: "JuceTracktionKitRunner",
            dependencies: [
                "JuceTracktionKit",
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
            ],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .target(
            name: "CJuceTracktion",
            dependencies: [],
            exclude: [
                "../CJuceTracktion/extra",
                "../../tracktion_engine/modules/juce /modules/juce_graphics/unicode/sheenbidi/Source/SheenBidi.c",
                "../../tracktion_engine/modules/juce/modules/include_juce_graphics.cpp",
                "../../tracktion_engine/modules/juce/modules",
                "../../tracktion_engine/modules/juce/modules/RecentFilesMenuTemplate.nib",
            ],
            publicHeadersPath: "include",
            cxxSettings: commonCxxSettings + [
                .define("JUCE_DLL_BUILD", to: "1"), // Build as shared library
                .define("JUCE_SHARED_CODE", to: "1"),
            ],
            linkerSettings: linkerSettings
        ),
        .target(
            name: "JuceTracktionKit",
            dependencies: ["CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .target(
            name: "JuceTracktionDemos",
            dependencies: ["CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .testTarget(
            name: "JuceTracktionKitTests",
            dependencies: ["JuceTracktionKit", "CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
    ],
    cxxLanguageStandard: .cxx20
)
