public struct MIDIEvent {
    public let noteNumber: Int
    public let velocity: Int
    public let startTime: Double
    public let length: Double

    public init(noteNumber: Int, velocity: Int, startTime: Double, length: Double) {
        self.noteNumber = noteNumber
        self.velocity = velocity
        self.startTime = startTime
        self.length = length
    }
}
