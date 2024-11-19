
import XCTest
import JuceTracktionKit 
import CJuceTracktion

@available(macOS 14.0, iOS 17.0, *)
final class JuceTracktionKitTests: XCTestCase {
    func testTracktionEngineInitialization() {
        juce.initialiseJuce_GUI()
        let engine = TracktionEngine()
        XCTAssertNotNil(engine)
        juce.shutdownJuce_GUI()
    }
}

