import SwiftUI

public struct ContentView: View {
    @EnvironmentObject var audioEngineManager: AudioEngineManager

    public init() {}

    public var body: some View {
        VStack {
            Text("Juce TracktionKit Runner")
                .font(.largeTitle)
                .padding()

            HStack {
                Button("Start Playback") {
                    audioEngineManager.start()
                }
                Button("Stop Playback") {
                    audioEngineManager.stop()
                }
            }
            .padding()

            Text("Is Playing: \(audioEngineManager.isPlaying ? "Yes" : "No")")
                .padding()

            Slider(value: Binding(
                get: { audioEngineManager.tempo },
                set: { audioEngineManager.setTempo($0) }
            ), in: 40 ... 200, label: {
                Text("Tempo")
            })
            .padding()

            Text("Tempo: \(Int(audioEngineManager.tempo)) BPM")
                .padding()
        }
        .frame(width: 400, height: 300)
    }
}
