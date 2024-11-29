import ArgumentParser
import Foundation
import JuceTracktionDemos
import JuceTracktionKit

@main
struct DemosRunner: AsyncParsableCommand {
    static var configuration = CommandConfiguration(
        commandName: "jucetracktiondemos",
        abstract: "Run JuceTracktion demos",
        subcommands: [
            StepSequencer.self,
        ]
    )

    static func main() {}
}

extension DemosRunner {
    struct StepSequencer: AsyncParsableCommand {
        static var configuration = CommandConfiguration(
            commandName: "stepsequencer",
            abstract: "Run the step sequencer demo"
        )

        func run() async throws {}
    }
}
