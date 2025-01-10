public struct Duration {
    public let beats: Double
    public let seconds: Double

    public init(beats: Double, bpm: Double = 60) {
        self.beats = beats
        seconds = beats / bpm * 60.0
    }
}
