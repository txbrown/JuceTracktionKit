import Foundation

public struct SoundBank {
    public let name: String
    public let presetURL: URL?

    public init(name: String, presetURL: URL?) {
        self.name = name
        self.presetURL = presetURL
    }
}
