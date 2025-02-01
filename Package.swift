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
]

// Shared CXX settings
let commonCxxSettings: [CXXSetting] = [
    .headerSearchPath("../../tracktion_engine/modules"),
    .headerSearchPath("../../tracktion_engine/modules/juce/modules"),
    .headerSearchPath("Sources/CJuceTracktion/include"),
    .headerSearchPath("../../tracktion_engine/modules/tracktion_engine/model/clips"),
    .headerSearchPath("Sources/CJuceTracktion"),
    .unsafeFlags([
        "-I", "tracktion_engine/modules",
        "-I", "tracktion_engine/modules/juce/modules",
        "-I", "Sources/CJuceTracktion/include",
        "-I", "tracktion_engine/modules/tracktion_engine/model/clips",
        "-I", "Sources/CJuceTracktion",
        "-x", "objective-c++",
        "-std=c++20",
        "-fno-objc-arc",
        "-stdlib=libc++",
        "-D_LIBCPP_DISABLE_DEPRECATION_WARNINGS",
        "-Wno-deprecated-declarations",
        "-fvisibility=hidden",
        "-fvisibility-inlines-hidden",
    ]),
    .define("JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED", to: "1"),
    .define("JUCE_APP_CONFIG_HEADER", to: "\"TracktionConfig.h\""),
    .define("JUCE_STANDALONE_APPLICATION", to: "0"),
    .define("JUCE_USE_DARK_MODE", to: "0"),
    .define("JUCE_DISPLAY_SPLASH_SCREEN", to: "0"),
    .define("JUCE_REPORT_APP_USAGE", to: "0"),
    .define("JUCE_DONT_DECLARE_PROJECTINFO", to: "1"),
    .define("JUCE_MAC", to: "1"),
    .define("JUCE_IOS", to: "0"),
    .define("JUCE_SILENCE_XCODE_15_LINKER_WARNING", to: "1"),
    .define("JUCE_HEADLESS_PLUGIN_CLIENT", to: "1"),
    .define("JUCE_MODAL_LOOPS_PERMITTED", to: "1"),
    .define("TRACKTION_DISABLE_PERFORMANCE_MEASUREMENT", to: "1"),
    .define("TRACKTION_DISABLE_SIGNPOST", to: "1"),
    .define("TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH", to: "1"),
    .define("JUCE_CHECK_MEMORY_LEAKS", to: "0"),
    .define("TRACKTION_UNIT_TESTS", to: "0"),
    .define("TRACKTION_LOG_ENABLED", to: "0"),
    .define("OS_LOG_CATEGORY_POINTS_OF_INTEREST", to: "0"),
    .define("OS_OBJECT_USE_OBJC", to: "1"),
    .define("OS_SIGNPOST_IS_SWIFT", to: "0"),
    .define("OS_LOG_TARGET_HAS_10_14_FEATURES", to: "1"),
    .define("OS_SIGNPOST_IS_IOS_OR_MAC", to: "1"),
    .unsafeFlags([
        "-x", "objective-c++",
        "-std=c++20",
        "-fno-objc-arc",
        "-stdlib=libc++",
        "-D_LIBCPP_DISABLE_DEPRECATION_WARNINGS",
        "-Wno-deprecated-declarations",
        "-fvisibility=hidden",
        "-fvisibility-inlines-hidden",
    ]),
    .define("DEBUG", .when(configuration: .debug)),
    .define("NDEBUG", .when(configuration: .release)),
]

// Shared Swift settings
let swiftSettings: [SwiftSetting] = [
    .interoperabilityMode(.Cxx),
]

let package = Package(
    name: "SwiftTracktionKit",
    platforms: [
        .macOS(.v14),
        .iOS(.v17),
    ],
    products: [
        .library(name: "SwiftTracktionKit", type: .static, targets: ["SwiftTracktionKit", "CJuceTracktion"]),
    ],
    dependencies: [
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.3.0"),
    ],
    targets: [
        .executableTarget(
            name: "SwiftTracktionKitRunner",
            dependencies: [
                "SwiftTracktionKit",
                .product(name: "ArgumentParser", package: "swift-argument-parser"),
            ],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .binaryTarget(name: "Tracktion", path: "build/xcframework/Tracktion.xcframework"),
        .target(
            name: "CJuceTracktion",
            dependencies: ["Tracktion"],
            publicHeadersPath: "include",
            cxxSettings: commonCxxSettings + [
                .define("JUCE_DLL_BUILD", to: "1"),
                .define("JUCE_SHARED_CODE", to: "1"),
                .define("CJUCETRACKTION_EXPORTS", to: "1"),
            ],
            linkerSettings: linkerSettings
        ),
        .target(
            name: "SwiftTracktionKit",
            dependencies: ["CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .target(
            name: "SwiftTracktionKitDemos",
            dependencies: ["SwiftTracktionKit"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
        .testTarget(
            name: "SwiftTracktionKitTests",
            dependencies: ["SwiftTracktionKit", "CJuceTracktion"],
            cxxSettings: commonCxxSettings,
            swiftSettings: swiftSettings
        ),
    ],
    cxxLanguageStandard: .cxx20
)
