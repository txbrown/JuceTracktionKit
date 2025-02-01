
import CJuceTracktion
import SwiftTracktionKit
import XCTest

final class SwiftTracktionKitTests: XCTestCase {
    func testTracktionEngineInitialization() {
        juce.initialiseJuce_GUI()
        let engine = AudioEngine.create("Test")
        XCTAssertNotNil(engine)
        juce.shutdownJuce_GUI()
    }
}
