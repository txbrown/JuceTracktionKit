@testable import SwiftTracktionKit
import XCTest

final class EngineTests: XCTestCase {
    func testEngineInitialization() {
        let engine = AudioEngineManager(name: "Test")
        XCTAssertFalse(engine.isPlaying)
    }

    func testEditCreation() {
        let engine = AudioEngineManager(name: "Test")
        let edit = engine.getEdit()
        XCTAssertNotNil(edit)
    }
}
