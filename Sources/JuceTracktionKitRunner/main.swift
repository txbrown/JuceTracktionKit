import CJuceTracktion
import JuceTracktionKit
import SwiftUI

class Demo: ObservableObject {
    public let audioEngineManager: AudioEngineManager
    private var trackManager: TrackManager
    private var midiClipManager: MidiClipManager

    public init() {
        audioEngineManager = AudioEngineManager(name: "JuceTracktionKitRunner")
        trackManager = TrackManager(audioEngineManager.getEdit())
        midiClipManager = MidiClipManager(audioEngineManager.getEdit())
        setup()
    }

    private func setup() {
        audioEngineManager.enableClickTrack()
        let midiTrackID: Int32 = trackManager.createAudioTrack(name: "MIDI Track")
        print("Midi Track ID: \(midiTrackID)")

        let midiClipID = midiClipManager.createMidiClip(trackID: midiTrackID, name: "MIDI Clip - Track 1", startBar: 0, lengthInBars: 8)
        print("The new midi clip id is \(midiClipID)")

        trackManager.createSamplerPlugin(config: .init(
            name: "Sampler 1",
            trackID: Int(midiTrackID),
            samples: [
                .init(
                    filePath: "/users/ricardo.abreu/developer/personal/midicircuit-macos/sounds/default soundbanks/shy kit/SNS_TD_kick_premium.wav",
                    noteNumber: 36
                ),
                .init(
                    filePath: "/users/ricardo.abreu/developer/personal/midicircuit-macos/sounds/default soundbanks/shy kit/SNS_TD_snare_boom_bap.wav",
                    noteNumber: 37
                ),
                .init(
                    filePath: "/users/ricardo.abreu/developer/personal/midicircuit-macos/sounds/default soundbanks/shy kit/sns_td_hihat_punchy.wav",
                    noteNumber: 38
                ),
            ]
        ))

        for start in 0 ... 7 {
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 36, startBeat: Double(start), lengthInBeats: 1, velocity: 117, color: 0, mute: false))
            if (start % 2) != 0 {
                midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 37, startBeat: Double(start), lengthInBeats: 1, velocity: 117, color: 0, mute: false))
            }
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 38, startBeat: Double(start), lengthInBeats: 0.5, velocity: 88, color: 0, mute: false))
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 38, startBeat: Double(start) + 0.5, lengthInBeats: 0.5, velocity: 92, color: 0, mute: false))
        }

        for start in 8 ... 900 {
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 36, startBeat: Double(start), lengthInBeats: 1, velocity: 117, color: 0, mute: false))
            if (start % 2) != 0 {
                midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 37, startBeat: Double(start), lengthInBeats: 1, velocity: 117, color: 0, mute: false))
            }
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 38, startBeat: Double(start), lengthInBeats: 0.5, velocity: 88, color: 0, mute: false))
            midiClipManager.addNote(clipID: midiClipID, note: .init(noteNumber: 38, startBeat: Double(start) + 0.5, lengthInBeats: 0.5, velocity: 92, color: 0, mute: false))
        }

        print(midiClipManager.getNotes(clipID: midiClipID))

        audioEngineManager.exportAudio(to: URL(string: "./audio.wav")!)
    }
}

@main
struct TracktionKitApp: App {
    @StateObject private var demo = Demo()

    init() {
        // We must initialise juce
        juce.initialiseJuce_GUI()
    }

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(demo.audioEngineManager)
                .onDisappear {
                    juce.shutdownJuce_GUI()
                }
        }
    }
}
