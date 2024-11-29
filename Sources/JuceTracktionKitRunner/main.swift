import CJuceTracktion
import JuceTracktionKit
import SwiftUI

@main
struct TracktionKitApp: App {
    @StateObject private var audioEngineManager = AudioEngineManager(name: "JuceTracktionKitRunner")

    init() {
        // Initialize JUCE
        juce.initialiseJuce_GUI()
    }

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(audioEngineManager)
                .onDisappear {
                    juce.shutdownJuce_GUI()
                }
        }
    }
}
