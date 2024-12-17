import CJuceTracktion

public struct Sample {
    let filePath: String
    let noteNumber: Int

    public init(
        filePath: String,
        noteNumber: Int
    ) {
        self.filePath = filePath
        self.noteNumber = noteNumber
    }
}

public struct SamplerPluginConfig {
    let name: String
    let trackID: Int
    let clipID: Int
    let samples: [Sample]

    public init(
        name: String,
        trackID: Int,
        clipID: Int,
        samples: [Sample]
    ) {
        self.name = name
        self.trackID = trackID
        self.clipID = clipID
        self.samples = samples
    }
}

public extension MidiClipManager {
    mutating func createSamplerPlugin(config: SamplerPluginConfig) {
        let defaultSampleFiles = config.samples.map { std.string($0.filePath) }
        createSamplerPlugin(trackID: Int32(config.trackID), clipID: Int32(config.clipID), defaultSampleFiles: .init(defaultSampleFiles))
    }
}
