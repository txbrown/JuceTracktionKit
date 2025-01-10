@testable import JuceTracktionKit
import XCTest

final class EngineTests: XCTestCase {
    func testEngineInitialization() {
        let engine = Engine()
        XCTAssertFalse(engine.version.isEmpty)
    }

    func testEditCreation() {
        let engine = Engine()
        let edit = engine.createEdit()
        XCTAssertNotNil(edit)

        edit.tempo = 120.0
        XCTAssertEqual(edit.tempo, 120.0)
    }
}
