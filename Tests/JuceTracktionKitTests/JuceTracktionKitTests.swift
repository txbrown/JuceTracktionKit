
import CJuceTracktion
import JuceTracktionKit
import XCTest

final class JuceTracktionKitTests: XCTestCase {
    func testTracktionEngineInitialization() {
        juce.initialiseJuce_GUI()
        let engine = TracktionEngine.getInstance()
        XCTAssertNotNil(engine)
        juce.shutdownJuce_GUI()
    }
}
