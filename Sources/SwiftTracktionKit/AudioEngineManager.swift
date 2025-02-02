import CJuceTracktion
import Foundation

public class AudioEngineManager: ObservableObject {
    private let cxxEngine: AudioEngine
    @Published public var isPlaying = false
    @Published public var tempo: Double = 120.0

    public init(name: String) {
        cxxEngine = AudioEngine.create(std.string(name))
    }

    public func start() {
        cxxEngine.start()
        isPlaying = true
    }

    public func stop() {
        cxxEngine.stop()
        isPlaying = false
    }

    public func setTempo(_ bpm: Double) {
        cxxEngine.tempo = bpm
        tempo = bpm
    }

    public func exportAudio(to url: URL) {
        DispatchQueue.main.async { [weak self] in
            self?.cxxEngine.exportAudio(to: std.string(url.path)) { progress in
                print("Export progress: \(progress)")
            }
        }
    }

    public func getEdit() -> OpaquePointer? {
        return cxxEngine.edit
    }

    public func enableClickTrack() {
        cxxEngine.enableClickTrack()
    }
}
