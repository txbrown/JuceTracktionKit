import ArgumentParser
import CJuceTracktion
import Foundation
import JuceTracktionKit

@main
struct TracktionKitRunner: AsyncParsableCommand {
    static func main() {
        print("Running TracktionKitRunner!")
        juce.initialiseJuce_GUI()
        let engine = TracktionEngine.getInstance()
        engine.initialize()
        engine.shutdown()
        juce.shutdownJuce_GUI()
        print("Shutdown!")
    }
}
