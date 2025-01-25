
import CJuceTracktion
import JuceTracktionKit
import XCTest

final class JuceTracktionKitTests: XCTestCase {
    func testTracktionEngineInitialization() {
        juce.initialiseJuce_GUI()
        let engine = AudioEngine.create("Test")
        XCTAssertNotNil(engine)
        juce.shutdownJuce_GUI()
    }
}
