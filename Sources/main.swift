
import SwiftTracktionKit

@main
struct SwiftTracktionKit {
    static func main() {
        // Create engine instance
        let engine = Engine()
        print("Using Tracktion Engine version: \(engine.version)")

        // Create an edit
        let edit = engine.createEdit()
        edit.tempo = 120.0

        // Create transport
        let transport = edit.createTransport()

        // Basic transport control
        transport.play()
        Thread.sleep(forTimeInterval: 2.0)
        transport.stop()
    }
}
