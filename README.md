# SwiftTracktionKit

SwiftTracktionKit is a Swift package that provides a Swift-friendly wrapper around the Tracktion Engine, enabling audio processing and sequencing capabilities in Swift using C++ interoperability.

## Features

- Seamless integration with Tracktion Engine in Swift.
- Utilizes Swift's C++ interop for efficient performance.
- Simplifies Tracktion Engine usage within Swift projects.

## Requirements

- Xcode 15+
- macOS 12+
- Swift Package Manager (SPM)

## Installation

### Repository

SwiftTracktionKit repository: [GitHub - SwiftTracktionKit](https://github.com/txbrown/SwiftTracktionKit)

### Using Swift Package Manager (SPM)

1. Open your Xcode project.
2. Go to **File > Add Packages**.
3. Enter the repository URL of `SwiftTracktionKit`.

#### Adding as a Dependency in a Swift Package

If you are using SwiftTracktionKit in another Swift package, add the following to your `Package.swift` dependencies:

```swift
dependencies: [
    .package(url: "https://github.com/txbrown/SwiftTracktionKit.git", from: "1.0.0")
]
```

Then, add it as a dependency in your target:

```swift
targets: [
    .target(
        name: "YourTargetName",
        dependencies: ["SwiftTracktionKit"]
    )
]
```

## Setup

### Header Search Paths

In your Xcode project, navigate to **Build Settings > Header Search Paths**, and add:

```sh
$(BUILD_DIR)/../SourcePackages/checkouts/SwiftTracktionKit/tracktion_engine/modules
$(BUILD_DIR)/../SourcePackages/checkouts/SwiftTracktionKit/tracktion_engine/modules/juce/modules
```

### Preprocessor Definitions

You need to add the following preprocessor definitions in your Xcode project's build settings.

#### Steps:

1. Select your target in Xcode.
2. Go to the **Build Settings** tab.
3. Search for **Preprocessor Macros** or **GCC_PREPROCESSOR_DEFINITIONS**.
4. Add these definitions for both Debug and Release configurations:

```sh
OS_LOG_CATEGORY_POINTS_OF_INTEREST=0
TRACKTION_DISABLE_PERFORMANCE_MEASUREMENT=1
TRACKTION_DISABLE_SIGNPOST=1
TRACKTION_LOG_ENABLED=0
TRACKTION_ENABLE_GRAPH_NATIVE_PERFORMANCE_COUNTERS=0
TRACKTION_ENABLE_GRAPH_NATIVE_SIGNPOSTS=0
TRACKTION_ENABLE_GRAPH_PERFORMANCE_COUNTERS=0
TRACKTION_ENABLE_PERFORMANCE_MEASUREMENT=0
TRACKTION_ENABLE_SIGNPOSTS=0
JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
```

## Usage

Once the package is integrated and the setup is complete, you can start using `SwiftTracktionKit` in your Swift code to access Tracktion Engine functionality. Example usage documentation will be added soon.

## Contributing

Contributions are welcome! Please submit pull requests or issues via the repository.

## License

`SwiftTracktionKit` is distributed under the MIT License; however, it depends on `tracktion_engine`, which is licensed under the **GPLv3**.

This means that if you use `SwiftTracktionKit`, your project must comply with the **GPLv3** license unless you obtain a commercial license from Tracktion Corporation. For more details, refer to the [Tracktion Engine License](https://github.com/Tracktion/tracktion_engine#license) and consult legal advice if necessary.
`SwiftTracktionKit` is licensed under the MIT License. See `LICENSE` for details.
