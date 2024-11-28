import CJuceTracktion
import Foundation

public class StepSequencerDemo {
    private let engine: TracktionEngine
    private var edit: Edit?
    private var transport: Transport?
    private var stepClip: StepClip?
    private var sampler: SamplerPlugin?

    public init() {
        juce.initialiseJuce_GUI()
        engine = TracktionEngine.getInstance()
    }

    deinit {
        juce.shutdownJuce_GUI()
    }

    public func setup() {
        print("Setting up Step Sequencer Demo...")
        engine.initialize()

        // Create edit and get transport
        createEdit()
        createStepClip()
        createSamplerPlugin(defaultSampleFiles())
    }

    public func shutdown() {
        transport?.stop(false, false)
        engine.shutdown()
    }

    // MARK: - Core Setup Methods

    private func createEdit() {
        edit = Edit.create(engine)
        transport = edit?.getTransport()

        // Set initial tempo
        // if let tempo = edit?.getTempoSequence().pointee.tempoSequence.getTempo(0) {
        //     tempo.setBpm(120.0)
        // }
    }

    private func createStepClip() {
        guard let edit = edit else { return }

        // Get or create first audio track
        guard let track = edit.getOrInsertAudioTrackAt(0) else { return }

        // Find length of 1 bar
        let oneBar = edit.getTempoSequence().barsBeatsToTime(1, 0.0)

        // Create step clip
        var timeRange = EditTimeRange()

        track.insertNewClip(ClipType.step, "Step Clip", &timeRange)

        // Get the created clip and enable looping
        // if let clip = track.pointee.getClips().first as? StepClip {
        //     stepClip = clip
        //     clip.setLoopRange(timeRange)
        // }
    }

    private func createSamplerPlugin(_ sampleFiles: [URL]) {
        guard let track = edit?.getOrInsertAudioTrackAt(0),
              //   var sampler = edit?.getPluginCache().pointee.createPlugin(.sampler) as? SamplerPlugin,
              var stepClip = stepClip else { return }

        // Add sampler to track
        // track.pluginList.insertPlugin(sampler, 0)

        // Add samples to channels
        for (index, file) in sampleFiles.enumerated() {
            if index < stepClip.getNumChannels() {
                // Add sound to sampler
                // sampler.addSound(file.path, name: file.lastPathComponent, 0, 0, 1.0)

                // Set sound parameters to match step clip channel
                let soundIndex = sampler?.getNumChannels() ?? 0 - 1
                let channel = index // Assuming channel index matches file index
                // sampler.setSoundParams(soundIndex, noteNumber: Int32(channel),
                //                        minNote: Int32(channel), maxNote: Int32(channel))

                // Randomize initial pattern for this channel
                stepClip.pattern(0)?.pointee.randomiseSteps()
            }
        }
    }

    // MARK: - Transport Controls

    public func play() {
        transport?.play(false)
    }

    public func stop() {
        transport?.stop(false, false)
    }

    public func setTempo(_: Double) {
        // edit?.getTempoSequence().pointee.getTempo(0)?.setBpm(bpm)
    }

    // MARK: - Pattern Management

    public func randomisePattern() {
        stepClip?.pattern(0).pointee.randomiseSteps()
    }

    public func clearPattern() {
        stepClip?.pattern(0)?.pointee.clear()
    }

    public func setNote(channel _: Int, step _: Int, enabled _: Bool) {
        // stepClip?.pattern(0)?.setNote(channel, step, enabled)
    }

    // MARK: - Sample Management

    public func loadSampleToChannel(_ channel: Int, file: URL) {
        guard var sampler = sampler,
              let stepClip = stepClip,
              channel < stepClip.getNumChannels() else { return }

        sampler.setSoundMedia(Int32(channel), file.path)

        // Update sound parameters to match channel
        let soundIndex = channel
        // sampler.setSoundParams(soundIndex, noteNumber: Int32(channel),
        //                        minNote: Int32(channel), maxNote: Int32(channel))
    }

    // MARK: - Helper Methods

    private func defaultSampleFiles() -> [URL] {
        // In a real implementation, you would return URLs to default sample files
        // For now, returning an empty array
        return []
    }
}
